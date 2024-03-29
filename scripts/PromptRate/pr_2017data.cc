#include <stdio.h>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLatex.h"
#include "TGraphAsymmErrors.h"
#include <string>
using namespace std;

TGraphAsymmErrors* getEtaGraph(TTree* t, float looseMiniIsoCut,float tightMiniIsoCut,bool mu){

  string cutstring = Form("LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("LepMiniIso < %f",looseMiniIsoCut);
  //float muEtabins[11]={-2.4,-2.1,-1.2,-0.9,-0.4,0.0,0.4,0.9,1.2,2.1,2.4}; //2016 binning
  //float elEtabins[11]={-2.4,-1.566,-1.4442,-0.8,-0.4,0.0,0.4,0.8,1.4442,1.566,2.4}; //2016 binning
  float muEtabins[10]={-2.4,-2.1,-1.2,-0.9,-0.4,0.4,0.9,1.2,2.1,2.4};
  float elEtabins[10]={-2.4,-1.566,-1.4442,-0.8,-0.4,0.4,0.8,1.4442,1.566,2.4};
  
  TH1F* num;
  TH1F* den;
  //if(mu) {num = new TH1F("num","",10,muEtabins);den = new TH1F("den","",10,muEtabins);} //2016 binning
  //else {num = new TH1F("num","",10,elEtabins);den = new TH1F("den","",10,elEtabins);} //2016 binning
  if(mu) {num = new TH1F("num","",9,muEtabins);den = new TH1F("den","",9,muEtabins);}
  else {num = new TH1F("num","",9,elEtabins);den = new TH1F("den","",9,elEtabins);}
  t->Project("num","LepEta",cutstring.c_str());
  t->Project("den","LepEta",loosecut.c_str());

  TGraphAsymmErrors* g = new TGraphAsymmErrors(num,den);
  
  for(int i=0; i< 9;i++){
  	if(mu){
  	std::cout << "eta bin: " << elEtabins[i] << " to " << elEtabins[i+1]  << ", PR: "<< g->GetY()[i] << "+/-" << g->GetErrorY(i) <<std::endl;
  	}
  	else{
  	std::cout << "eta bin: " << muEtabins[i] << " to " << muEtabins[i+1]  << ", PR: "<< g->GetY()[i] << "+/-" << g->GetErrorY(i) <<std::endl;  	
  	}
  }  
  
  delete num; delete den;
  return g;
}

TGraphAsymmErrors* getPtGraph(TTree* t, float looseMiniIsoCut,float tightMiniIsoCut){

  string cutstring = Form("LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("LepMiniIso < %f",looseMiniIsoCut);
//   float ptbins[15] = {30,40,50,60,70,80,90,100,125,150,200,300,400,500,1000};
//   TH1F* num = new TH1F("num","PromptRate",14,ptbins);  
//   TH1F* den = new TH1F("den","den",14,ptbins);
  float ptbins[16] = {20,30,40,50,60,70,80,90,100,125,150,200,300,400,500,1000};
  TH1F* num = new TH1F("num","PromptRate",15,ptbins);  
  TH1F* den = new TH1F("den","den",15,ptbins);
  t->Project("num","LepPt",cutstring.c_str());
  t->Project("den","LepPt",loosecut.c_str());

  TGraphAsymmErrors* g = new TGraphAsymmErrors(num,den);

  for(int i=0; i< 15;i++){
  	std::cout << "pt bin: " << ptbins[i] << "-" << ptbins[i+1] << ", PR: "<< g->GetY()[i] << "+/-" << g->GetErrorY(i) <<std::endl;
  }  


  delete num; delete den;
  return g;
}


std::pair<float,float> getPromptRate(TTree* t,float looseMiniIsoCut,float tightMiniIsoCut){
  string cutstring = Form("LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("LepMiniIso < %f",looseMiniIsoCut);
  TH1F* num = new TH1F("num","",1,0,10000);
  TH1F* den = new TH1F("den","",1,0,10000);
  t->Project("num","LepPt",cutstring.c_str());
  t->Project("den","LepPt",loosecut.c_str());
  TGraphAsymmErrors* g = new TGraphAsymmErrors(num,den);  
  float pr = g->GetY()[0];
  float err = g->GetErrorY(0);
  std::pair<float,float> pair;
  pair.first = pr;
  pair.second = err;
  delete den; delete num;
  return pair;
}

void pr_2017data(TString trig_, int doEl_=1, int doMu_=1){

  std::string elID = "MVA2017TightV2RC";

  //std::string trig = "nonIsoHTTrigs";
  //std::string trig = "IsoTrigs_forTrilep";
  TString trig = trig_;
  
    // Fail safe
  if(trig=="")trig = "nonIsoHTTrig";  //default
  if(trig != "nonIsoHTTrig" && trig != "isoTrig_forTrilep"){
      std::cout<<"Trigger selecton not recognized."<<std::endl;
      gApplication->Terminate();      
    }
  
  std::cout << "Trigger: " << trig << std::endl;
  std::cout << "doEl: " << doEl_ << std::endl;
  std::cout << "doMu: " << doMu_ << std::endl;
  
  bool doEl = doEl_;
  bool doMu = doMu_;
  
  std::vector<TString> eras; 
//   eras.push_back("2017B");
//   eras.push_back("2017C");
//   eras.push_back("2017D");
//   eras.push_back("2017E");
  eras.push_back("2017F");
//   eras.push_back("All");

  // Make output directoty
  TString outDir = "Outputs_06282019_"+trig+"/"; //all era's
  system("mkdir -vp "+outDir);
  
  //Input folder
  TString path = "../../test/PromptRate_"+trig;

  for(unsigned int i=0; i < eras.size(); i++){

    TString era = eras.at(i);
    std::cout<<"working on era: "<<era<<std::endl;

    //make output file to save graph
    TFile* fout = new TFile(outDir+"/"+"PromptRate_Graph_"+era+"_"+elID+"_"+trig+".root","RECREATE");


    /// ELECTRON :
    
    if(doEl){

    //make output file
	TString el_filename;
	el_filename = "/PromptRate_Data_"+era+"_Electrons_"+elID+"_SortByPhi_"+trig+".root";
    TFile* fEle = new TFile(path+el_filename);
    TTree* tEle = (TTree*) fEle->Get("FakeRate");
//     TGraphErrors* gEle = new TGraphErrors(40);
//     gEle->SetName("ElectronPromptRate");
//     for(int i =1; i<41; i++){
//       float cut = 0.01*i;
//       std::pair<float,float> rateAndErr = getPromptRate(tEle,0.4,cut);
//       float fakerate = rateAndErr.first;
//       float err = rateAndErr.second;
//       gEle->SetPoint(i,cut,fakerate);
//       gEle->SetPointError(i,0.,err);
//       std::cout<<"Electron Prompt Rate for miniIso cut < "<<cut<<" is: "<<fakerate<< " +/- "<< err <<std::endl;
//     }
//     
//     gEle->SetMarkerStyle(22);
//     gEle->SetMarkerColor(kBlue);
//     gEle->SetTitle("Electron Prompt Rate vs MiniIsolation Requirement;I_{mini} Requirement;Prompt Rate");
//     TCanvas* c = new TCanvas();
//     gEle->Draw("apl");
//     c->Print(outDir+"ElectronPromptRate-vs-MiniIso_"+era+"_"+elID+"_"+trig+".pdf");

    //getEtagraphs for 0.1 cut
    TGraphAsymmErrors* gEtaEl = getEtaGraph(tEle,0.4,0.1,false);
    TCanvas* cEtaEl = new TCanvas();
    gEtaEl->Draw("apl");
    cEtaEl->Print(outDir+"ElectronPromptRate-vs-Eta_"+era+"_"+elID+"_"+trig+".pdf");
    //get pT graphs for 0.1 cut
    TGraphAsymmErrors* gPtEl = getPtGraph(tEle,0.4,0.1);
    TCanvas* cPtEl = new TCanvas();
    gPtEl->Draw("apl");
    cPtEl->Print(outDir+"ElectronPromptRate-vs-Pt_"+era+"_"+elID+"_"+trig+".pdf");
    
    //save graphs
//     fout->WriteTObject(gEle);
//     fout->WriteTObject(gEtaEl,"ElectronEtaPromptRate");
//     fout->WriteTObject(gPtEl,"ElectronPtPromptRate");

    }
    
    if(doMu){
    /// MUON :

    //make output file
	TString mu_filename;
	mu_filename = "/PromptRate_Data_"+era+"_Muons_CBTightMiniIso_SortByPhi_"+trig+".root";
	std::cout << "Opening file:" << mu_filename << std::endl;	

    TFile* fMu = new TFile(path+mu_filename);
    TTree* tMu = (TTree*) fMu->Get("FakeRate");
//     TGraphErrors* gMu = new TGraphErrors(41);
//     gMu->SetName("MuonPromptRate");
//     for(int i =1; i<41; i++){
//       float cut = 0.01*i;
//       std::pair<float,float> rateAndErr = getPromptRate(tMu,0.4,cut);
//       float fakerate = rateAndErr.first;
//       float err = rateAndErr.second;
//       gMu->SetPoint(i,cut,fakerate);
//       gMu->SetPointError(i,0.,err);
//       std::cout<<"Muon Prompt Rate for miniIso cut < "<<cut<<" is: "<<fakerate << " +/- "<< err <<std::endl;
//     }
//     
//     gMu->SetMarkerStyle(22);
//     gMu->SetMarkerColor(kBlue);
//     gMu->SetTitle("Muon Prompt Rate vs MiniIsolation Requirement;I_{mini} Requirement;Prompt Rate");
//     TCanvas* c1 = new TCanvas();
//     gMu->Draw("apl");
//     c1->Print(outDir+"MuonPromptRate-vs-MiniIso_"+era+"_"+trig+".pdf");
// 
    //getEtagraphs for 0.1 cut
    TGraphAsymmErrors* gEtaMu = getEtaGraph(tMu,0.4,0.1,true);
    TCanvas* cEtaMu = new TCanvas();
    gEtaMu->Draw("apl");
    cEtaMu->Print(outDir+"MuonPromptRate-vs-Eta_"+era+"_"+elID+"_"+trig+".pdf");
    //get pT graphs for 0.1 cut
    TGraphAsymmErrors* gPtMu = getPtGraph(tMu,0.4,0.1);
    TCanvas* cPtMu = new TCanvas();
    gPtMu->Draw("apl");
    cPtMu->Print(outDir+"MuonPromptRate-vs-Pt_"+era+"_"+elID+"_"+trig+".pdf");

// 
//     //save graphs
//     fout->WriteTObject(gMu);
//     fout->WriteTObject(gEtaMu,"MuonEtaPromptRate");
//     fout->WriteTObject(gPtMu,"MuonPtPromptRate");

    }

//     fout->Close();
  }
  
  gApplication->Terminate();

}
