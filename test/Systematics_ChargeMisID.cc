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
std::vector<TLepton*> makeLeptons(std::vector<TElectron*> electrons, std::string ID);
bool matchToGenLep(TElectron* el, std::vector<TGenParticle*> genParticles);
TGenParticle* getMatchedGenParticle(TElectron* el, std::vector<TGenParticle*> genParticles);
//this script will produce histograms for charge misID for ttjets and DYjetss when ignoring the Z window and just matching tight electrons to 

int main(int argc, char* argv[]){

  //read in ID
  if(argc!=3){
    std::cout<<"Need to specify sample and electron ID. Exiting..."<<std::endl;
    return 0;
  }
  std::string sample(argv[1]);
  std::string elID(argv[2]);
  bool mc = true;
  //make treereader
  std::string filename;
  if(sample=="DYJets_pt0") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_DYJets_pt0.root";
  else if(sample=="DYJets_pt1") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_DYJets_pt1.root";
  else if(sample=="DYJets_pt2") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_DYJets_pt2.root";
  else if(sample=="DYJets_pt3") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_DYJets_pt3.root";
  else if(sample=="DYJets_pt4") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_DYJets_pt4.root";
  else if(sample=="DYJets_pt5") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_DYJets_pt5.root";
  else if(sample=="DYJets_pt6") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_DYJets_pt6.root";
  else if(sample=="DYJets_pt7") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_DYJets_pt7.root";
  else if(sample=="DYJets_pt8") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_DYJets_pt8.root";
  else if(sample=="DYJets_pt9") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_DYJets_pt9.root";
  else if(sample=="TTbar_pt0") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_TTbar_pt0.root";
  else if(sample=="TTbar_pt1") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_TTbar_pt1.root";
  else if(sample=="TTbar_pt2") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_TTbar_pt2.root";
  else if(sample=="TTbar_pt3") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_TTbar_pt3.root";
  else if(sample=="TTbar_pt4") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_TTbar_pt4.root";
  else if(sample=="TTbar_pt5") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_TTbar_pt5.root";
  else if(sample=="TTbar_pt6") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_TTbar_pt6.root";
  else if(sample=="TTbar_pt7") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_TTbar_pt7.root";
  else if(sample=="TTbar_pt8") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_TTbar_pt8.root";
  else if(sample=="TTbar_pt9") filename = "/eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/ljmet_TTbar_pt9.root";



  else {std::cout<<"You didn't pick a valid sample! Exiting..."<<std::endl; return 0;}
  TreeReader* tr = new TreeReader(filename.c_str(),mc);
  TTree* t = tr->tree;

  //make outfile
  std::string outname;
  if(sample=="DYJets_pt0") outname = "Systematics_ChargeMisID_DYJets_pt0.root";
  else if(sample=="DYJets_pt1") outname = "Systematics_ChargeMisID_DYJets_pt1.root";
  else if(sample=="DYJets_pt2") outname = "Systematics_ChargeMisID_DYJets_pt2.root";
  else if(sample=="DYJets_pt3") outname = "Systematics_ChargeMisID_DYJets_pt3.root";
  else if(sample=="DYJets_pt4") outname = "Systematics_ChargeMisID_DYJets_pt4.root";
  else if(sample=="DYJets_pt5") outname = "Systematics_ChargeMisID_DYJets_pt5.root";
  else if(sample=="DYJets_pt6") outname = "Systematics_ChargeMisID_DYJets_pt6.root";
  else if(sample=="DYJets_pt7") outname = "Systematics_ChargeMisID_DYJets_pt7.root";
  else if(sample=="DYJets_pt8") outname = "Systematics_ChargeMisID_DYJets_pt8.root";
  else if(sample=="DYJets_pt9") outname = "Systematics_ChargeMisID_DYJets_pt9.root";
  else if(sample=="TTbar_pt0") outname = "Systematics_ChargeMisID_TTbar_pt0.root";
  else if(sample=="TTbar_pt1") outname = "Systematics_ChargeMisID_TTbar_pt1.root";
  else if(sample=="TTbar_pt2") outname = "Systematics_ChargeMisID_TTbar_pt2.root";
  else if(sample=="TTbar_pt3") outname = "Systematics_ChargeMisID_TTbar_pt3.root";
  else if(sample=="TTbar_pt4") outname = "Systematics_ChargeMisID_TTbar_pt4.root";
  else if(sample=="TTbar_pt5") outname = "Systematics_ChargeMisID_TTbar_pt5.root";
  else if(sample=="TTbar_pt6") outname = "Systematics_ChargeMisID_TTbar_pt6.root";
  else if(sample=="TTbar_pt7") outname = "Systematics_ChargeMisID_TTbar_pt7.root";
  else if(sample=="TTbar_pt8") outname = "Systematics_ChargeMisID_TTbar_pt8.root";
  else if(sample=="TTbar_pt9") outname = "Systematics_ChargeMisID_TTbar_pt9.root";

  TFile* fout= new TFile(outname.c_str(),"RECREATE");


  //histos eta all
  TH1F* etaNumHist = new TH1F("etaNumHist","#eta of electrons",15,-3,3);
  TH1F* etaDenHist = new TH1F("etaDenHist","#eta of electrons",15,-3,3);
  //histos eta low pt
  TH1F* etaNumHist_lpt = new TH1F("etaNumHist_lpt","#eta of electrons",15,-3,3);
  TH1F* etaDenHist_lpt = new TH1F("etaDenHist_lpt","#eta of electrons",15,-3,3);
  //histos for hight pt eta
  TH1F* etaNumHist_hpt = new TH1F("etaNumHist_hpt","#eta of electrons",15,-3,3);
  TH1F* etaDenHist_hpt = new TH1F("etaDenHist_hpt","#eta of electrons",15,-3,3);

  TH1F* ptNumHist = new TH1F("ptNumHist","p_{T} of electrons",20,0,600);
  TH1F* ptDenHist = new TH1F("ptDenHist","p_{T} of electrons",20,0,600);


  int nEntries = t->GetEntries();

  for( int ient=0; ient < nEntries; ient++){

    tr->GetEntry(ient);
    
    if(ient % 100000 ==0) std::cout<<"Completed "<<ient<<" out of "<<nEntries<<" events"<<std::endl;

    //load in electrons
    std::vector<TElectron*> electrons = tr->allElectrons;
    
    for(std::vector<TElectron*>::size_type i =0; i < electrons.size(); i++){
      TElectron* iel = electrons.at(i);
      //skip if not tight
      bool tight = false;
      if(elID=="MVA2016TightRC"){
	if(iel->mva2016TightRCIso()) tight = true;
      }
      if(!tight) continue;
      
      //check ability to match to gen lepton
      bool matched = matchToGenLep(iel, tr->genParticles);
      if(!matched) continue;
      //get matching gen particle
      TGenParticle* gp = getMatchedGenParticle(iel,tr->genParticles);
      //skip if genParticle not close or prompt
      float drtemp = pow( pow( iel->eta - gp->eta, 2 ) + pow( iel->phi - gp->phi, 2), 0.5);
      if(drtemp > 0.3 || !(gp->isPrompt)) continue;
      //fill denominator hists
      etaDenHist->Fill(iel->eta,tr->MCWeight);
      ptDenHist->Fill(iel->pt,tr->MCWeight);
      //fill numerator if charges aren't equal
      if(iel->charge != gp->charge){
	etaNumHist->Fill(iel->eta,tr->MCWeight);
	ptNumHist->Fill(iel->eta,tr->MCWeight);
	if(iel->pt >100) etaNumHist_hpt->Fill(iel->eta,tr->MCWeight);
       	else 	         etaNumHist_lpt->Fill(iel->eta,tr->MCWeight);
      }
      if(iel->pt >100) etaDenHist_hpt->Fill(iel->eta,tr->MCWeight);
      else 	       etaDenHist_lpt->Fill(iel->eta,tr->MCWeight);
      
    }//end loop over electrons
  }// end event loop

  //write output file
  fout->Write();

  return 0;
}



bool matchToGenLep(TElectron* el, std::vector<TGenParticle*> genParticles){

  float dR=999;
  //run through gen particle collection:
  for(unsigned int igen=0; igen<genParticles.size(); igen++){
    TGenParticle* genParticle = genParticles.at(igen);
    //only run over electrons from hard scattering
    if( ! (fabs(genParticle->id)==11) ) continue;
    if( ! (genParticle->status==23 || genParticle->status==1) ) continue;

    float drtemp = pow( pow( el->eta - genParticle->eta, 2 ) + pow( el->phi - genParticle->phi, 2), 0.5);
    if(drtemp < dR){
      dR = drtemp;
    }
  }

  //now we have a min deltaR between our electron and all electrons from hard scattering, if it is less than 0.15 let's consider it matched
  if(dR <0.1) return true;
  else return false;
  
}
    
TGenParticle* getMatchedGenParticle(TElectron* el, std::vector<TGenParticle*> genParticles){


  TGenParticle* gp=0;


  float dR=999;
  //run through gen particle collection:
  for(unsigned int igen=0; igen<genParticles.size(); igen++){
    TGenParticle* genParticle = genParticles.at(igen);
    //only run over electrons from hard scattering
    if( ! (fabs(genParticle->id)==11) ) continue;
    if( ! (genParticle->status==23 || genParticle->status==1) ) continue;

    float drtemp = pow( pow( el->eta - genParticle->eta, 2 ) + pow( el->phi - genParticle->phi, 2), 0.5);
    if(drtemp < dR){
      dR = drtemp;
      gp=genParticle;
    }
  }

  return gp;
}

