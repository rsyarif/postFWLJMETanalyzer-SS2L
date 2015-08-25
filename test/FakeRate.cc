#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include <vector>
#include "TLorentzVector.h"
#include "TChain.h"
#include "../interface/TreeReader.h"
#include <stdio.h>
#include <assert.h>
#include <map>
#include <string>
#include <sstream> 
#include "../plugins/Macros.cc"

//helper functions
std::vector<TLepton*> makeLeptons(std::vector<TMuon*> muons, std::vector<TElectron*> electrons, bool Muons);
bool ZVetoCheck(TLepton* lepton, std::vector<TJet*> jets);
bool AwayJetCheck(TLepton* lepton, std::vector<TJet*> jets);

int main(int argc, char* argv[]){

  //example usage: ./FakeRate.o Data Electrons

  if(argc!=3){
    std::cout<<"Error! You must specify whether running on 'Data' of 'MC' as option as well as electrons or muons"<<std::endl;
    return 0;
  }

  std::string argv1 = argv[1];
  std::string argv2 = argv[2];

  if(argv1.find("Data")==std::string::npos && argv1.find("MC")==std::string::npos){
    std::cout<<"Error! You must specify whether running on 'Data' of 'MC' as option"<<std::endl;
    return 0;
  }

  //now get boolean for data or mc
  bool data=false;
  if(argv1.find("Data")!=std::string::npos) data=true;

  //boolean for channel
  bool MuonChannel=false;
  if(argv2.find("Muons")!=std::string::npos) MuonChannel=true;


  //make output file
  std::string outname;
  if(data){
    if(MuonChannel) outname = "FakeRate_Data_Muons.root";
    else outname = "FakeRate_Data_Electrons.root";
  }

  else{
    if(MuonChannel) outname = "FakeRate_MC_Muons.root";
    else outname = "FakeRate_MC_Electrons.root";
  }


  TFile* fout = new TFile(outname.c_str(),"RECREATE");

  //get correct file
  std::string filename;
  if(argv1.find("Data")!=std::string::npos) filename="";
  else filename="/eos/uscms/store/user/lpctlbsm/clint/PHYS14/Inclusive_Decays/PU20/ljmet_trees/ljmet_tree_QCD.root";

  //get tree reader to read in data
  TreeReader* tr= new TreeReader(filename.c_str());
  TTree* t=tr->tree;


  //initialize needed histograms
  TH1F* ptNumHist = new TH1F("ptNumHist","p_{T} of Tight Leptons",20,0,600);
  TH1F* ptDenHist = new TH1F("ptDenHist","p_{T} of All Leptons",20,0,600);
  TH1F* etaNumHist = new TH1F("etaNumHist","#eta of Tight Leptons",12,-5,5);
  TH1F* etaDenHist = new TH1F("etaDenHist","#eta of All Leptons",12,-5,5);

  //get number of entries and start event loop
  int nEntries = t->GetEntries();
  for(int ient=0; ient<nEntries; ient++){

    tr->GetEntry(ient);

    if(ient % 1000 ==0) std::cout<<"Completed "<<ient<<" out of "<<nEntries<<" events"<<std::endl;

    //make vector of leptons
    std::vector<TLepton*> leptons = makeLeptons(tr->allMuons,tr->allElectrons,MuonChannel);

    //veto on events with more than one lepton or no leptons
    if((leptons.size()==0) || (leptons.size() >1) ) continue;
    //now just take the lepton remaining
    TLepton* lep = leptons.at(0);

    //make sure not much met in event to veto on leptons from Ws
    if(tr->MET > 25) continue;
    
    //check transvers mass is less than 25 GeV
    

    //search through jet collection to check for jet mass
    bool Zveto = ZVetoCheck(lep,tr->allAK4Jets);
    if(Zveto) continue;

    //check for away jet
    bool awayJet = AwayJetCheck(lep,tr->allAK4Jets);
    if(!awayJet) continue;

    //Now just fill histograms
    etaDenHist->Fill(lep->eta); ptDenHist->Fill(lep->pt);
    if(lep->Tight){etaNumHist->Fill(lep->eta);ptNumHist->Fill(lep->pt);}

  }//end event loop

  fout->WriteTObject(ptNumHist);
  fout->WriteTObject(ptDenHist);
  fout->WriteTObject(etaNumHist);
  fout->WriteTObject(etaDenHist);

  TGraphAsymmErrors* ptgraph = new TGraphAsymmErrors(ptNumHist,ptDenHist);
  TGraphAsymmErrors* etagraph = new TGraphAsymmErrors(etaNumHist,etaDenHist);

  fout->WriteTObject(ptgraph);
  fout->WriteTObject(etagraph);

}

std::vector<TLepton*> makeLeptons(std::vector<TMuon*> muons, std::vector<TElectron*> electrons, bool Muons){

  std::vector<TLepton*> Leptons;

  if(Muons){
    //fill with  muons
    for(unsigned int uimu=0; uimu<muons.size(); uimu++){
      TMuon* imu = muons.at(uimu);
      TLepton* iLep = new TLepton(imu->pt,imu->eta,imu->phi,imu->energy,imu->charge);
      iLep->Tight=imu->cutBasedTight();
      iLep->Loose=imu->cutBasedLoose();
      iLep->isMu = true;
      iLep->isEl = false;
      //only save if at least loose
      if(iLep->Tight || iLep->Loose){
	//apply pt cut
	if(iLep->pt>25 && iLep->pt<35) Leptons.push_back(iLep);
      }
    }
  }

  else{
    //fill with  electrons
    for(unsigned int uiel=0; uiel<electrons.size(); uiel++){
      TElectron* iel = electrons.at(uiel);
      TLepton* iLep = new TLepton(iel->pt,iel->eta,iel->phi,iel->energy,iel->charge);
      iLep->Tight=iel->cutBasedTight();
      iLep->Loose=iel->cutBasedLoose();
      iLep->isMu = false;
      iLep->isEl = true;
      //only save if at least loose
      if(iLep->Tight || iLep->Loose){
	//apply pt cut
	if(iLep->pt>25 && iLep->pt<35) Leptons.push_back(iLep);
      }
    }
  }

  return Leptons;

}

bool ZVetoCheck(TLepton* lep, std::vector<TJet*> jets){



  float diff=999;
  for(std::vector<TJet*>::size_type i=0; i< jets.size(); i++){
    TJet* ijet = jets.at(i);
    float tempmass = (lep->lv + ijet->lv).M();
    if( fabs(tempmass-91.1) <diff){
      diff = fabs(tempmass-91.1);

    }
  }

  bool veto=false;
  if(diff<20) veto=true;

  return veto;

}


bool AwayJetCheck(TLepton* lep, std::vector<TJet*> jets){

  bool awayJet=false;

  for(std::vector<TJet*>::size_type i=0; i<jets.size();i++){
    TJet* ijet = jets.at(i);
    float dr = pow( pow(lep->eta - ijet->eta,2) + pow(lep->phi - ijet->phi,2) , 0.5);
    if(ijet->pt>40 && dr>1.0){ awayJet=true; break;}
  }

  return awayJet;
}