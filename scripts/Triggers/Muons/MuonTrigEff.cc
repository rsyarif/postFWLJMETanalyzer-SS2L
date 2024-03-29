#include "TLegend.h"

/*TH1F* getRatio(TGraphAsymmErrors* g1, TGraphAsymmErrors* g2){

  float* xbins[10]= {30,35,40,50,60,100,150,200,300,500}

  TH1F* h = new TH1F("h","",g1->GetN(),xbins);
  

}
*/
void MuonTrigEff(TString filter="Mu30",TString era = "2016BD"){

  gStyle->SetOptStat(kFALSE);
  

  //load in tnp
  TFile* fTnP = new TFile("TnP_MuonTrig_AbsEta_fitMuonTrig_"+filter+"_"+era+"_WP.root");
  TCanvas* c_eta1 = (TCanvas*) fTnP->Get("tpTree/"+filter+"Pass_eta/fit_eff_plots/pt_PLOT_abseta_bin0");
  TCanvas* c_eta2 = (TCanvas*) fTnP->Get("tpTree/"+filter+"Pass_eta/fit_eff_plots/pt_PLOT_abseta_bin1");
  TCanvas* c_eta3 = (TCanvas*) fTnP->Get("tpTree/"+filter+"Pass_eta/fit_eff_plots/pt_PLOT_abseta_bin2");
  TCanvas* c_eta4 = (TCanvas*) fTnP->Get("tpTree/"+filter+"Pass_eta/fit_eff_plots/pt_PLOT_abseta_bin3");
  TCanvas* c_eta5 = (TCanvas*) fTnP->Get("tpTree/"+filter+"Pass_eta/fit_eff_plots/pt_PLOT_abseta_bin4");

  TGraphAsymmErrors* TnPGraph_eta1 = (TGraphAsymmErrors*) c_eta1->GetPrimitive("hxy_fit_eff");
  TGraphAsymmErrors* TnPGraph_eta2 = (TGraphAsymmErrors*) c_eta2->GetPrimitive("hxy_fit_eff");
  TGraphAsymmErrors* TnPGraph_eta3 = (TGraphAsymmErrors*) c_eta3->GetPrimitive("hxy_fit_eff");
  TGraphAsymmErrors* TnPGraph_eta4 = (TGraphAsymmErrors*) c_eta4->GetPrimitive("hxy_fit_eff");
  TGraphAsymmErrors* TnPGraph_eta5 = (TGraphAsymmErrors*) c_eta5->GetPrimitive("hxy_fit_eff");

  TCanvas* c_new = new TCanvas();  
  c_new->SetLogx();
  TnPGraph_eta1->Draw("ap");
  TnPGraph_eta1->GetYaxis()->SetRangeUser(0,1.1);
  for(int i=0; i<TnPGraph_eta1->GetN();i++){
    std::cout<<"efficiency for 0<eta<0.4 pt bin "<<i<<" is: "<<TnPGraph_eta1->GetY()[i]<<std::endl;
  }


  c_new->Print(""+filter+"_EffTnP_Eta0p0-0p4.pdf");


  TnPGraph_eta2->Draw("ap");
  TnPGraph_eta2->GetYaxis()->SetRangeUser(0,1.1);
  for(int i=0; i<TnPGraph_eta2->GetN();i++){
    std::cout<<"efficiency for 0.4<eta<0.9 pt bin "<<i<<" is: "<<TnPGraph_eta2->GetY()[i]<<std::endl;
  }
  c_new->Print(""+filter+"_EffTnP_Eta0p4-0p9.pdf");

  TnPGraph_eta3->Draw("ap");
  TnPGraph_eta3->GetYaxis()->SetRangeUser(0,1.1);
  for(int i=0; i<TnPGraph_eta3->GetN();i++){
    std::cout<<"efficiency for 0.9<eta<1.2 pt bin "<<i<<" is: "<<TnPGraph_eta3->GetY()[i]<<std::endl;
  }
  c_new->Print(""+filter+"_EffTnP_Eta0p9-1p2.pdf");

  TnPGraph_eta4->Draw("ap");
  TnPGraph_eta4->GetYaxis()->SetRangeUser(0,1.1);
  for(int i=0; i<TnPGraph_eta4->GetN();i++){
    std::cout<<"efficiency for 1.2<eta<2.1 pt bin "<<i<<" is: "<<TnPGraph_eta4->GetY()[i]<<std::endl;
  }

  c_new->Print(""+filter+"_EffTnP_Eta1p2-2p1.pdf");

  TnPGraph_eta5->Draw("ap");
  TnPGraph_eta5->GetYaxis()->SetRangeUser(0,1.1);
  for(int i=0; i<TnPGraph_eta5->GetN();i++){
    std::cout<<"efficiency for 2.1<eta<2.4 pt bin "<<i<<" is: "<<TnPGraph_eta5->GetY()[i]<<std::endl;
  }

  c_new->Print(""+filter+"_EffTnP_Eta2p1-2p4.pdf");



  
}
