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
#include <math.h>
#include <map>
#include <string>
#include <sstream> 
#include "../plugins/Macros.cc"

//helper functions
std::vector<TLepton*> makeLeptons(std::vector<TMuon*> muons, std::vector<TElectron*> electrons, bool Muons, std::string ID);
bool ZVetoCheck(TLepton* lepton, std::vector<TJet*> jets);
bool AwayJetCheck(TLepton* lepton, std::vector<TJet*> jets);

int main(int argc, char* argv[]){

  //example usage: ./FakeRate.o Data Electrons <ID>
  std::cout<<"dummy"<<std::endl;
  if(argc!=4){
    std::cout<<"Error! You must specify whether running on 'Data' of 'MC' as option as well as electrons or muons and the ID used"<<std::endl;
    return 0;
  }

  std::string argv1 = argv[1];
  std::string argv2 = argv[2];
  std::string ID = argv[3];
  
  //now get boolean for data or mc
  bool data=false;
  if(argv1.find("Data")!=std::string::npos) data=true;

  //boolean for channel
  bool MuonChannel=false;
  if(argv2.find("Muons")!=std::string::npos) MuonChannel=true;


  //make output file
  std::vector<std::string> outname;
  if(data){
    if(MuonChannel) outname.push_back("CrossCheck_FakeRate_Data_Muons_"+ID+".root");
    else outname.push_back("CrossCheck_FakeRate_Data_Electrons_"+ID+".root");
  }

  else{
    if(MuonChannel){
      //outname.push_back("CrossCheck_FakeRate_MC_Muons_HT100To200_"+ID+".root");
      //outname.push_back("CrossCheck_FakeRate_MC_Muons_HT200To300_"+ID+".root");
      //outname.push_back("CrossCheck_FakeRate_MC_Muons_HT300To500_"+ID+".root");
      //outname.push_back("CrossCheck_FakeRate_MC_Muons_HT500To700_"+ID+".root");
      //outname.push_back("CrossCheck_FakeRate_MC_Muons_HT700To1000_"+ID+".root");
      //outname.push_back("CrossCheck_FakeRate_MC_Muons_HT1000To1500_"+ID+".root");
      if(argv1=="TTbar") outname.push_back("CrossCheck_FakeRate_TTbar_Muons_"+ID+".root");
      if(argv1=="TTbar_ext1") outname.push_back("CrossCheck_FakeRate_TTbar_ext1_Muons_"+ID+".root");
      if(argv1=="TTbar_ext2") outname.push_back("CrossCheck_FakeRate_TTbar_ext2_Muons_"+ID+".root");
      if(argv1=="TTbar_ext3") outname.push_back("CrossCheck_FakeRate_TTbar_ext3_Muons_"+ID+".root");
      if(argv1=="TTbar_ext4") outname.push_back("CrossCheck_FakeRate_TTbar_ext4_Muons_"+ID+".root");
      if(argv1=="DYJets") outname.push_back("CrossCheck_FakeRate_DYJets_Muons_"+ID+".root");
      if(argv1=="WJets") outname.push_back("CrossCheck_FakeRate_WJets_Muons_"+ID+".root");
      //      outname.push_back("CrossCheck_FakeRate_MC_Muons_HT1500To2000_"+ID+".root");
      //outname.push_back("CrossCheck_FakeRate_MC_Muons_HT2000ToInf_"+ID+".root");
    }
    else{
      //outname.push_back("CrossCheck_FakeRate_MC_Electrons_HT100To200_"+ID+".root");
      //outname.push_back("CrossCheck_FakeRate_MC_Electrons_HT200To300_"+ID+".root");
      //outname.push_back("CrossCheck_FakeRate_MC_Electrons_HT300To500_"+ID+".root");
      //outname.push_back("CrossCheck_FakeRate_MC_Electrons_HT500To700_"+ID+".root");
      //outname.push_back("CrossCheck_FakeRate_MC_Electrons_HT700To1000_"+ID+".root");
      //outname.push_back("CrossCheck_FakeRate_MC_Electrons_HT1000To1500_"+ID+".root");
      //outname.push_back("CrossCheck_FakeRate_MC_Electrons_HT1500To2000_"+ID+".root");
      //outname.push_back("CrossCheck_FakeRate_MC_Electrons_HT2000ToInf_"+ID+".root");
      if(argv1=="TTbar") outname.push_back("CrossCheck_FakeRate_TTbar_Electrons_"+ID+".root");
      if(argv1=="TTbar_ext1") outname.push_back("CrossCheck_FakeRate_TTbar_ext1_Electrons_"+ID+".root");
      if(argv1=="TTbar_ext2") outname.push_back("CrossCheck_FakeRate_TTbar_ext2_Electrons_"+ID+".root");
      if(argv1=="TTbar_ext3") outname.push_back("CrossCheck_FakeRate_TTbar_ext3_Electrons_"+ID+".root");
      if(argv1=="TTbar_ext4") outname.push_back("CrossCheck_FakeRate_TTbar_ext4_Electrons_"+ID+".root");
      if(argv1=="DYJets") outname.push_back("CrossCheck_FakeRate_DYJets_Electrons_"+ID+".root");
      if(argv1=="WJets") outname.push_back("CrossCheck_FakeRate_WJets_Electrons_"+ID+".root");
    }
  }




  //get correct file
  std::vector<std::string> filenames;
  if(argv1.find("Data")!=std::string::npos) {
    if(MuonChannel) filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Run2015D/FakeRate/Nov17/ljmet_trees/ljmet_FakeRate_Mu.root");
    else if(!MuonChannel) filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Run2015D/FakeRate/Nov17/ljmet_trees/ljmet_FakeRate_El.root");
  }
  else{
    if(argv1=="TTbar") filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Nov17/ljmet_trees/ljmet_FakeRate_TTbar.root");
    if(argv1=="TTbar_ext1") filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Nov17/ljmet_trees/ljmet_FakeRate_TTbar_ext1.root");
    if(argv1=="TTbar_ext2") filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Nov17/ljmet_trees/ljmet_FakeRate_TTbar_ext2.root");
    if(argv1=="TTbar_ext3") filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Nov17/ljmet_trees/ljmet_FakeRate_TTbar_ext3.root");
    if(argv1=="TTbar_ext4") filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Nov17/ljmet_trees/ljmet_FakeRate_TTbar_ext4.root");
    if(argv1=="DYJets") filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Nov17/ljmet_trees/ljmet_FakeRate_DYJets.root");
    if(argv1=="WJets") filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Nov17/ljmet_trees/ljmet_FakeRate_WJets.root");
    //filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Oct08/ljmet_trees/ljmet_QCDHT100To200.root");
    //filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Oct08/ljmet_trees/ljmet_QCDHT200To300.root");
    //filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Oct08/ljmet_trees/ljmet_QCDHT300To500.root");
    //filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Oct08/ljmet_trees/ljmet_QCDHT500To700.root");
    //filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Oct08/ljmet_trees/ljmet_QCDHT700To1000.root");
    //filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Oct08/ljmet_trees/ljmet_QCDHT1000To1500.root");
    //filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Oct08/ljmet_trees/ljmet_QCDHT1500To2000.root");
    //filenames.push_back("/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/FakeRate/Oct08/ljmet_trees/ljmet_QCDHT2000ToInf.root");
  }

  for(unsigned int i =0; i<filenames.size(); i++){
    
    TFile* fout = new TFile(outname.at(i).c_str(),"RECREATE");
    //get tree reader to read in data
    TreeReader* tr= new TreeReader(filenames.at(i).c_str(),!data);
    TTree* t=tr->tree;
    
    
    //initialize needed histograms
    TH1F* ptNumHist = new TH1F("ptNumHist","p_{T} of Tight Leptons",20,0,200);
    TH1F* ptDenHist = new TH1F("ptDenHist","p_{T} of All Leptons",20,0,200);
    TH1F* etaNumHist = new TH1F("etaNumHist","#eta of Tight Leptons",12,-5,5);
    TH1F* etaDenHist = new TH1F("etaDenHist","#eta of All Leptons",12,-5,5);
    
    TH1F* etaHist_all = new TH1F("etaHist_all","#eta of all Loose Leptons",12,-5,5);
    TH1F* etaHist_ZVeto = new TH1F("etaHist_ZVeto","#eta of Leptons after zVeto",12,-5,5);
    TH1F* etaHist_MET = new TH1F("etaHist_MET","#eta of Leptons after MET cut",12,-5,5);
    TH1F* etaHist_MT = new TH1F("etaHist_MT","#eta of Leptons after M_{T} cut",12,-5,5);
    TH1F* etaHist_AwayJet = new TH1F("etaHist_AwayJet","#eta of Leptons after AwayJet cut",12,-5,5);


    //get number of entries and start event loop
    int nEntries = t->GetEntries();
    for(int ient=0; ient<nEntries; ient++){
      
      tr->GetEntry(ient);
      
      if(ient % 100000 ==0) std::cout<<"Completed "<<ient<<" out of "<<nEntries<<" events"<<std::endl;
      
      //make vector of leptons
      std::vector<TLepton*> leptons = makeLeptons(tr->allMuons,tr->allElectrons,MuonChannel, ID);
      float weight = tr->MCWeight > 0 ? 1.0 : -1.0;
      //veto on events with more than one lepton or no leptons
      if((leptons.size()==0) || (leptons.size() >1) ) continue;
      //now just take the lepton remaining
      TLepton* lep = leptons.at(0);
      etaHist_all->Fill(lep->eta,weight);
      //make sure not much met in event to veto on leptons from Ws
      if(tr->MET > 25) continue;
      etaHist_MET->Fill(lep->eta,weight);
      //std::cout<<"lepton eta after MET cut : "<<lep->eta<<std::endl;      
      //check transverse mass is less than 25 GeV
      
      float et = tr->MET+lep->energy;
      float pt1y = lep->pt*sin(lep->phi);
      float pt1x = lep->pt*cos(lep->phi);
      float pt2y = tr->MET*sin(tr->MET_phi);
      float pt2x = tr->MET*cos(tr->MET_phi);
      float mT = pow(et,2) - pow(lep->pt,2) - pow(tr->MET,2) - 2*(pt1y*pt2y - pt1x*pt2x);
      mT = pow(mT, 0.5);

      double dphi = fabs(tr->MET_phi - lep->phi);
      if (dphi > TMath::Pi()) dphi = TMath::TwoPi() - dphi;

      double mt2 = 2 * lep->pt * tr->MET * (1 - cos(dphi));;
      double mt  = sqrt(mt2);

      //std::cout<<"Clint mt: "<<mT<<" Aram mt: "<<mt<<std::endl;

      if(mt>25) continue; //change to Arams value
      etaHist_MT->Fill(lep->eta,weight);      
      //search through jet collection to check for jet mass
      bool Zveto = ZVetoCheck(lep,tr->allAK4Jets);
      if(Zveto) continue;
      etaHist_ZVeto->Fill(lep->eta,weight);

      //check for away jet
      bool awayJet = AwayJetCheck(lep,tr->allAK4Jets);
      if(!awayJet) continue;
      etaHist_AwayJet->Fill(lep->eta,weight);

      //Now just fill histograms
      etaDenHist->Fill(lep->eta,weight); ptDenHist->Fill(lep->pt,weight);
      if(lep->Tight){etaNumHist->Fill(lep->eta,weight);ptNumHist->Fill(lep->pt,weight);}

    }//end event loop
    
    fout->Append(ptNumHist);
    fout->Append(ptDenHist);
    fout->Append(etaNumHist);
    fout->Append(etaDenHist);

    fout->Append(etaHist_all);
    fout->Append(etaHist_MET);
    fout->Append(etaHist_MT);
    fout->Append(etaHist_ZVeto);
    fout->Append(etaHist_AwayJet);

    TGraphAsymmErrors* ptgraph = new TGraphAsymmErrors(ptNumHist,ptDenHist);
    TGraphAsymmErrors* etagraph = new TGraphAsymmErrors(etaNumHist,etaDenHist);
    
    fout->Append(ptgraph);
    fout->Append(etagraph);
    fout->Write();
    fout->Close();

    //delete tree reader to free up space
    delete tr;

  }//end loop over filenames
}

std::vector<TLepton*> makeLeptons(std::vector<TMuon*> muons, std::vector<TElectron*> electrons, bool Muons, std::string ID){

  std::vector<TLepton*> Leptons;

  if(Muons){
    //fill with  muons
    for(unsigned int uimu=0; uimu<muons.size(); uimu++){
      TMuon* imu = muons.at(uimu);
      TLepton* iLep = new TLepton(imu->pt,imu->eta,imu->phi,imu->energy,imu->charge);
      if(ID=="CBTight"){
	iLep->Tight=imu->cutBasedTight();
	iLep->Loose=imu->cutBasedLoose();
      }
      else if(ID=="CBLoose"){
	iLep->Tight=imu->cutBasedLoose();
	iLep->Loose=true; //in case 'loose ID' is specified as 'tight', take any muon as loose ID
      }
      iLep->isMu = true;
      iLep->isEl = false;
      //only save if at least loose
      if(iLep->Tight || iLep->Loose){
	//apply pt cut
	Leptons.push_back(iLep);
      }
    }
  }

  else{
    //fill with  electrons
    for(unsigned int uiel=0; uiel<electrons.size(); uiel++){
      TElectron* iel = electrons.at(uiel);
      TLepton* iLep = new TLepton(iel->pt,iel->eta,iel->phi,iel->energy,iel->charge);
      if(ID=="CBTight"){
	iLep->Tight=iel->cutBasedTight25nsSpring15MC();
	iLep->Loose=iel->cutBasedLoose25nsSpring15MC();
      }
      if(ID=="CBTightRC"){
	iLep->Tight=iel->cutBasedTight25nsSpring15MCRC();
	iLep->Loose=iel->cutBasedLoose25nsSpring15MCRC();
      }
      else if(ID=="CBLoose"){
	iLep->Tight=iel->cutBasedLoose25nsSpring15MC();
	iLep->Loose=true;
      }
      else if(ID=="MVATightNew"){
	iLep->Tight=iel->mvaTightNew();
	iLep->Loose=iel->mvaLooseNew();
      }
      else if(ID=="MVATightNewRC"){
	iLep->Tight=iel->mvaTightNewRC();
	iLep->Loose=iel->mvaLooseNewRC();
      }
      else if(ID=="MVATight"){
	iLep->Tight=iel->mvaTightIso();
	iLep->Loose=iel->mvaLooseIso();
      }
      else if(ID=="MVATightNoIso"){
	iLep->Tight=iel->mvaTight();
	iLep->Loose=iel->mvaLoose();
      }
      else if(ID=="MVALoose"){
	iLep->Tight=iel->mvaLooseIso();
	iLep->Loose=true;
      }
      else if(ID=="MVALooseNoIso"){
	iLep->Tight=iel->mvaLoose();
	iLep->Loose=true;
      }
      else if(ID=="MVATightCC"){
	iLep->Tight=iel->mvaTightCCIso();
	iLep->Loose=iel->mvaLooseCCIso();
      }
      else if(ID=="MVATightCCNoIso"){
	iLep->Tight=iel->mvaTightCC();
	iLep->Loose=iel->mvaLooseCC();
      }
      else if(ID=="MVALooseCC"){
	iLep->Tight=iel->mvaLooseCCIso();
	iLep->Loose=true;
      }
      else if(ID=="MVALooseCCNoIso"){
	iLep->Tight=iel->mvaLooseCC();
	iLep->Loose=true;
      }
      else if(ID=="MVATightRC"){
	iLep->Tight=iel->mvaTightRCIso();
	iLep->Loose=iel->mvaLooseRCIso();
      }
      else if(ID=="MVALooseRC"){
	iLep->Tight=iel->mvaLooseRCIso();
	iLep->Loose=true;
      }
      else if(ID=="SUSYTight"){
	iLep->Tight=iel->susyTight();
	iLep->Loose=iel->susyLoose();
      }
      else if(ID=="SUSYLoose"){
	iLep->Tight=iel->susyLoose();
	iLep->Loose=true;
      }
      else if(ID=="SUSYTightRC"){
	iLep->Tight=iel->susyTightRC();
	iLep->Loose=iel->susyLooseRC();
      }      

      iLep->isMu = false;
      iLep->isEl = true;
      //only save if at least loose
      if(iLep->Tight || iLep->Loose){
	
	Leptons.push_back(iLep);
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
  if(diff<10) veto=true;

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
