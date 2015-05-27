#ifndef TLepton_h
#define TLepton_h

#include <TLorentzVector.h>
#include <cmath>
#include <iostream>

class TLepton {
public:
  TLepton(double pttemp, double etatemp, double phitemp, double energytemp, int chargetemp);
  //Four vector
  double pt;
  double eta;
  double phi;
  double energy;

  int charge;

  bool isEl;
  bool isMu;

  bool Tight;
  bool Loose;

  TLorentzVector lv;

  void setLV() {
    lv.SetPxPyPzE(pt*cos(phi), pt*sin(phi), pt*TMath::SinH(eta), energy);
  }

};

#endif