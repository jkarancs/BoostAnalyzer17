#ifndef WEIGHTING_H
#define WEIGHTING_H

// Private headers
#include "Variables.h"
#include "EventSelections.h"
#include "GluinoXSec.h"
#include "StopXSec.h"
#include "CharginoXSec.h"
#include "NeutralinoXSec.h"

// 3rd party headers
#include "tnm.h" // for getplot
#include "TH2.h"
#include "TH3.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"

// common libraries
#include <iostream>
#include <vector>

class Weighting
{
public:
  Weighting(Variables& var) : v(var) {
    w_nm1.resize(magic_enum::enum_count<EventSelections::Regions>());
    sf_weight.resize(magic_enum::enum_count<EventSelections::Regions>());
  }
  ~Weighting() {}

  void init_weight_histos();

  void init_input();

  void fill_weight_histos(const bool&, const bool&, const unsigned int&, const double&);

  double get_xsec_from_ntuple(const std::vector<std::string>&, const bool&);

  std::pair<double, double> get_xsec_totweight_from_txt_file(const std::string&);

  double get_totweight_from_ntuple(const std::vector<std::string>&, const bool&);

  void calc_signal_weightnorm(const std::vector<std::string>&, const double&, const bool&, TDirectory*, bool);

  double get_signal_weightnorm();

  void init_pileup_reweighting(const std::string&, const bool&, const std::vector<std::string>&);

  double get_toppt_weight(const double&, const unsigned int&, const bool&);

  double get_isr_weight(const double&, const unsigned int&, const bool&);

  double get_pileup_weight(const double, const double&, const unsigned int&, const bool&);

  double get_ht_weight(const double&);

  double get_alphas_weight(const double&, const int&);

  double get_scale_weight(const std::vector<double>&, const double&, const unsigned int&);


  double calc_lostlep_weight(const double&);

  double calc_trigger_efficiency(const double&);

  double weight;
  std::vector<double> all_weights;
  std::vector<double> sf_weight;
  // N-1 weights
  std::vector<std::vector<double> > w_nm1;

  double other_trigger_eff;

  double get_syst_weight(const double&, const double&, const double&, const double&);

  double get_syst_weight(const double&, const double&, const double&);

private:

  Variables& v;

  //_______________________________________________________
  //                Input histograms

  TGraphAsymmErrors* eff_trigger;
  TH3D* eff_3D_trigger_lep;
  TH3D* eff_3D_trigger_lep_up;
  TH3D* eff_3D_trigger_lep_down;
  TH2D* eff_trigger_lep;
  TH2D* eff_trigger_lep_up;
  TH2D* eff_trigger_lep_down;
  TH2D* eff_trigger_pho;
  TH2D* eff_trigger_pho_up;
  TH2D* eff_trigger_pho_down;
  TH2D* eff_trigger_F_met;
  TH2D* eff_trigger_F_mu;
  TH2D* eff_trigger_F_ele;
  TH2D* eff_trigger_F_pho;

  //_______________________________________________________
  //             List of weighting Histograms

  TH1D* h_totweight;
  TH1D* h_totweight_toppt;
  TH1D* h_totweight_pileup;
  std::vector<TH2D*> vh_totweight_signal;
  std::vector<TH2D*> vh_xsec_signal;
  std::vector<TH2D*> vh_weightnorm_signal;
  //std::vector<TH3D*> vh_totweight3D_signal;
  //std::vector<TH3D*> vh_xsec3D_signal;
  //std::vector<TH3D*> vh_weightnorm3D_signal;
  std::map<size_t, double> totweight3D_signal;
  std::map<size_t, double> weightnorm3D_signal;
  TH1D* h_nisrjets;
  TH1D* h_totweight_isr;
  std::vector<TH3D*> vh_nisrjets_signal;
  std::vector<TH3D*> vh_totweight_signal_isr;
  TH1D* h_npvLowHigh_data;
  std::vector<TH3D*> vh_npvLowHigh_signal;
  TH1D* h_pileup_data;
  TH1D* h_pileup_data_down;
  TH1D* h_pileup_data_up;
  TH1D* h_pileup_mc;
  TH1D* h_pileup_weight;
  TH1D* h_pileup_weight_down;
  TH1D* h_pileup_weight_up;
  TH1D* h_nvtx;
  TH1D* h_nvtx_rw;

  TH1D* h_trigger_pass;
  TH1D* h_trigger_total;
  TH2D* h_trigger2d_pass;
  TH2D* h_trigger2d_total;
};


void Weighting::init_input() {
  // 1D Trigger efficiency
  //if (v.year==2018) {
  //} else
  if (v.year==2017) {
    TH1D* pass  = getplot_TH1D("trigger_eff/191029/SingleLepton.root", "h_HT_TrigNoMass_1", "trig01");
    TH1D* total = getplot_TH1D("trigger_eff/191029/SingleLepton.root", "h_HT_TrigNoMass_0", "trig02");
    //TH1D* pass  = getplot_TH1D("trigger_eff/191029/SingleLepton.root", "h_MET_TrigMass_1", "trig01");
    //TH1D* total = getplot_TH1D("trigger_eff/191029/SingleLepton.root", "h_MET_TrigMass_0", "trig02");
    //TH1D* pass  = getplot_TH1D("trigger_eff/191029/SingleLepton.root", "h_AK8JetMass_TrigMass_1", "trig01");
    //TH1D* total = getplot_TH1D("trigger_eff/191029/SingleLepton.root", "h_AK8JetMass_TrigMass_0", "trig02");
    eff_trigger = new TGraphAsymmErrors(pass, total, "cl=0.683 b(1,1) mode");
  }

  // 2D Trigger Efficiency (New) - Use combination of SingleElectron + MET datasets
  TH2D* lep_pass_2d;
  TH2D* lep_total_2d;
  if (v.year==2018) {
    lep_pass_2d  = getplot_TH2D("trigger_eff/SingleMuonTriggerEff2018_v190804.root",   "h_HT_MET_TriggerEff_1",  "trig11");
    lep_total_2d = getplot_TH2D("trigger_eff/SingleMuonTriggerEff2018_v190804.root",   "h_HT_MET_TriggerEff_0",  "trig12");  
  } else if (v.year==2017) {
    lep_pass_2d  = getplot_TH2D("trigger_eff/191029/SingleLepton.root",   "h_HT_MET_TrigNoMass_1",  "trig03");
    lep_total_2d = getplot_TH2D("trigger_eff/191029/SingleLepton.root",   "h_HT_MET_TrigNoMass_0",  "trig04");
  } else {
    std::cout<<"Weighting::init_input(): trigger not yet defined for 2016"<<std::endl;
    std::exit(1);
    lep_pass_2d  = getplot_TH2D("trigger_eff/2016_placeholder",   "h_HT_MET_TrigNoMass_1",  "trig03");
    lep_total_2d = getplot_TH2D("trigger_eff/2016_placeholder",   "h_HT_MET_TrigNoMass_0",  "trig04");
  }

  eff_trigger_lep      = (TH2D*)lep_total_2d->Clone("eff_trigger_lep");      eff_trigger_lep     ->Reset();
  eff_trigger_lep_up   = (TH2D*)lep_total_2d->Clone("eff_trigger_lep_up");   eff_trigger_lep_up  ->Reset();
  eff_trigger_lep_down = (TH2D*)lep_total_2d->Clone("eff_trigger_lep_down"); eff_trigger_lep_down->Reset();

  for (int i=1; i<lep_total_2d->GetNbinsX()+1; i++) for (int j=1; j<lep_total_2d->GetNbinsY()+1; j++) {
    int lep_pass = lep_pass_2d->GetBinContent(i,j), lep_total = lep_total_2d->GetBinContent(i,j);
    if (lep_total>0) {
      TH1D p("p","",1,0,1); p.SetBinContent(1,lep_pass);  p.SetBinError(1,std::sqrt(lep_pass));
      TH1D t("t","",1,0,1); t.SetBinContent(1,lep_total); t.SetBinError(1,std::sqrt(lep_total));
      double eff = 0, err_up = 0, err_down = 0;
      geteff_AE(TGraphAsymmErrors(&p,&t), 0, eff, err_up, err_down);
      //std::cout<<"Trigger efficiency: "<<i<<" "<<j<<" "<<eff-err_down<<" "<<eff<<" "<<eff+err_up<<std::endl;
      eff_trigger_lep     ->SetBinContent(i,j,eff);
      eff_trigger_lep_up  ->SetBinContent(i,j,eff+err_up);
      eff_trigger_lep_down->SetBinContent(i,j,eff-err_down);
      // SPECIAL: Set error to the total counts, so we know if a bin is not empty
      eff_trigger_lep     ->SetBinError(i,j,lep_total);
    }
  }

  TH2D* pho_pass_2d;
  TH2D* pho_total_2d;
  if (v.year==2018) {
    pho_pass_2d  = getplot_TH2D("trigger_eff/EGammaTriggerEff2019_v20190906.root",   "h_HT_MET_TriggerEff_1",   "trig7");
    pho_total_2d = getplot_TH2D("trigger_eff/EGammaTriggerEff2019_v20190906.root",   "h_HT_MET_TriggerEff_0",  "trig8");
  } else if (v.year==2017) {
    pho_pass_2d  = getplot_TH2D("trigger_eff/191029/SinglePhoton.root",   "h_HT_MET_TrigNoMass_1",  "trig11");
    pho_total_2d = getplot_TH2D("trigger_eff/191029/SinglePhoton.root",   "h_HT_MET_TrigNoMass_0",  "trig12");
  } else {
    std::cout<<"Weighting::init_input(): trigger not yet defined for 2016"<<std::endl;
    std::exit(1);
    pho_pass_2d  = getplot_TH2D("trigger_eff/2016_placeholder",   "h_HT_MET_TrigNoMass_1",  "trig11");
    pho_total_2d = getplot_TH2D("trigger_eff/2016_placeholder",   "h_HT_MET_TrigNoMass_0",  "trig12");    
  }

  eff_trigger_pho      = (TH2D*)pho_total_2d->Clone("eff_trigger_pho");      eff_trigger_pho     ->Reset();
  eff_trigger_pho_up   = (TH2D*)pho_total_2d->Clone("eff_trigger_pho_up");   eff_trigger_pho_up  ->Reset();
  eff_trigger_pho_down = (TH2D*)pho_total_2d->Clone("eff_trigger_pho_down"); eff_trigger_pho_down->Reset();

  for (int i=1; i<pho_total_2d->GetNbinsX()+1; i++) for (int j=1; j<pho_total_2d->GetNbinsY()+1; j++) {
    int pho_pass = pho_pass_2d->GetBinContent(i,j), pho_total = pho_total_2d->GetBinContent(i,j);
    if (pho_total>0) {
      TH1D p("p","",1,0,1); p.SetBinContent(1,pho_pass);  p.SetBinError(1,std::sqrt(pho_pass));
      TH1D t("t","",1,0,1); t.SetBinContent(1,pho_total); t.SetBinError(1,std::sqrt(pho_total));
      double eff = 0, err_up = 0, err_down = 0;
      geteff_AE(TGraphAsymmErrors(&p,&t), 0, eff, err_up, err_down);
      //std::cout<<"Trigger efficiency: "<<i<<" "<<j<<" "<<eff-err_down<<" "<<eff<<" "<<eff+err_up<<std::endl;
      eff_trigger_pho     ->SetBinContent(i,j,eff);
      eff_trigger_pho_up  ->SetBinContent(i,j,eff+err_up);
      eff_trigger_pho_down->SetBinContent(i,j,eff-err_down);
      // SPECIAL: Set error to the total counts, so we know if a bin is not empty
      eff_trigger_pho     ->SetBinError(i,j,pho_total);
    }
  }

  // 3D Trigger Efficiency 
  TH3D* lep_pass_3d;
  TH3D* lep_total_3d;
  //if (v.year==2018) {
  //} else 
  if (v.year==2017) {
    lep_pass_3d   = getplot_TH3D("trigger_eff/191029/SingleLepton.root",   "h_HT_MET_AK8JetMass_TrigNoMass_1",  "trig21");
    lep_total_3d  = getplot_TH3D("trigger_eff/191029/SingleLepton.root",   "h_HT_MET_AK8JetMass_TrigNoMass_0",  "trig22");
    
    eff_3D_trigger_lep       = (TH3D*)lep_total_3d ->Clone("eff_3D_trigger_lep");       eff_3D_trigger_lep      ->Reset();
    eff_3D_trigger_lep_up    = (TH3D*)lep_total_3d ->Clone("eff_3D_trigger_lep_up");    eff_3D_trigger_lep_up   ->Reset();
    eff_3D_trigger_lep_down  = (TH3D*)lep_total_3d ->Clone("eff_3D_trigger_lep_down");  eff_3D_trigger_lep_down ->Reset();
    
    for (int i=1; i<lep_total_3d->GetNbinsX()+1; i++) {
      for (int j=1; j<lep_total_3d->GetNbinsY()+1; j++) {
        for (int k=1; k<lep_total_3d->GetNbinsZ()+1; k++) {
          int lep_pass = lep_pass_3d->GetBinContent(i,j,k), lep_total = lep_total_3d->GetBinContent(i,j,k);
          if (lep_total>0) {
            TH1D p("p","",1,0,1); p.SetBinContent(1,lep_pass);  p.SetBinError(1,std::sqrt(lep_pass));
            TH1D t("t","",1,0,1); t.SetBinContent(1,lep_total); t.SetBinError(1,std::sqrt(lep_total));
            double eff = 0, err_up = 0, err_down = 0;
            geteff_AE(TGraphAsymmErrors(&p,&t), 0, eff, err_up, err_down);
            //std::cout<<"Trigger efficiency: "<<i<<" "<<j<<" "<<eff-err_down<<" "<<eff<<" "<<eff+err_up<<std::endl;
            eff_3D_trigger_lep     ->SetBinContent(i,j,k,eff);
            eff_3D_trigger_lep_up  ->SetBinContent(i,j,k,eff+err_up);
            eff_3D_trigger_lep_down->SetBinContent(i,j,k,eff-err_down);
            // SPECIAL: Set error to the total counts, so we know if a bin is not empty
            eff_3D_trigger_lep     ->SetBinError(i,j,k,lep_total);
          }
        }
      }
    }
  }

  // Same trigger efficiencies but in the F region (needed for fake rates)
  const char* fin;
  if (v.year==2018) {
    fin = "trigger_eff/FakeRates2018.root";
  } else if (v.year==2017) {
    fin = "trigger_eff/Dec02_Golden_JSON/F_Region.root";
  }
  eff_trigger_F_met = getplot_TH2D(fin, "met", "trig_f_met");
  eff_trigger_F_mu  = getplot_TH2D(fin, "mu",  "trig_f_mu");
  eff_trigger_F_ele = getplot_TH2D(fin, "ele", "trig_f_ele");
  eff_trigger_F_pho = getplot_TH2D(fin, "pho", "trig_f_pho");

}


//_______________________________________________________
//              Define Histograms here
void
Weighting::init_weight_histos()
{
  // bins
  Double_t gluinoBins[202]; for (int i=0; i<202; ++i) gluinoBins[i] = (i-0.5)*25;
  Double_t stopBins[402];   for (int i=0; i<402; ++i) stopBins[i] = (i-0.5)*5;
  Double_t npvLowHighBins[3] = { 0,20,100 };
  Double_t isrWeightBins[3] = {0,1,2};
  Double_t isrJetBins[17]; for (int i=0; i<17; ++i) isrJetBins[i] = i-0.5;
  // total weight
  h_totweight                     = new TH1D("totweight",           "MC;;Total (generator) event weight", 1,0,1);
  h_totweight_toppt               = new TH1D("totweight_toppt",     "MC;;Total toppt weight",             2,0,2);
  h_totweight_pileup              = new TH1D("totweight_pileup",    "MC;;Total pileup weight",            2,0,2);
  vh_totweight_signal    .push_back(new TH2D("totweight_T1tttt",    "T1tttt or T5ttcc or T5tttt;m_{#tilde{g}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight",        201,gluinoBins, 201,gluinoBins));
  vh_totweight_signal    .push_back(new TH2D("totweight_T2tt",      "T2tt;m_{#tilde{t}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight",        401,stopBins, 401,stopBins));
  vh_totweight_signal    .push_back(new TH2D("totweight_TChiWZ",    "TChiWZ;m_{#tilde{#chi}^{#pm}_{0}=#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight",        401,stopBins, 401,stopBins));
  vh_totweight_signal    .push_back(new TH2D("totweight_TChiHH",    "TChiHH;m_{#tilde{#chi}^{0}_{3}=#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight",        401,stopBins, 401,stopBins));
  vh_totweight_signal    .push_back(new TH2D("totweight_T5qqqqZH",  "T5qqqqZH;m_{#tilde{g}} (GeV);m_{#tilde{#chi}^{0}_{2}} (GeV);Total Weight",        201,gluinoBins, 201,gluinoBins));
  // --> too much memory
  //vh_totweight3D_signal  .push_back(new TH3D("totweight_T6bbZH",    "T6bbZH;m_{#tilde{t}} (GeV);#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight", 401,stopBins, 401,stopBins, 401,stopBins));
  // ISR reweighting
  h_nisrjets                      = new TH1D("nisrjets",            ";N_{ISR jets}", 16,isrJetBins);
  h_totweight_isr                 = new TH1D("totweight_isr",       "MC;;Total (generator) event weight", 2,isrWeightBins);
  // --> too much memory
  //vh_nisrjets_signal     .push_back(new TH3D("nisrjets_T1tttt",     ";m_{#tilde{g}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight;N_{ISR jets}", 201,gluinoBins, 201,gluinoBins, 16,isrJetBins));
  //vh_nisrjets_signal     .push_back(new TH3D("nisrjets_T2tt",       ";m_{#tilde{t}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight;N_{ISR jets}", 401,stopBins,  401,stopBins,    16,isrJetBins));
  //vh_nisrjets_signal     .push_back(new TH3D("nisrjets_TChiWZ",     ";m_{#tilde{#chi}^{#pm}_{0}=#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight;N_{ISR jets}", 401,stopBins,  401,stopBins,    16,isrJetBins));
  //vh_nisrjets_signal     .push_back(new TH3D("nisrjets_TChiHH",     ";m_{#tilde{#chi}^{0}_{3}=#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight;N_{ISR jets}", 401,stopBins,  401,stopBins,    16,isrJetBins));
  //vh_nisrjets_signal     .push_back(new TH3D("nisrjets_T5qqqqZH",   ";m_{#tilde{g}} (GeV);m_{#tilde{#chi}^{0}_{2}} (GeV);Total Weight;N_{ISR jets}", 201,gluinoBins, 201,gluinoBins, 16,isrJetBins));
  //vh_nisrjets_signal     .push_back(new TH3D("nisrjets_T6bbZH",     ";m_{#tilde{t}} (GeV);#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight;N_{ISR jets}", 401,stopBins, 401,stopBins,  401,stopBins,    16,isrJetBins));

  vh_totweight_signal_isr.push_back(new TH3D("totweight_isr_T1tttt","T1tttt or T5ttcc or T5tttt;m_{#tilde{g}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight",        201,gluinoBins, 201,gluinoBins, 2,isrWeightBins));
  vh_totweight_signal_isr.push_back(new TH3D("totweight_isr_T2tt",  "T2tt;m_{#tilde{t}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight",                              401,stopBins,   401,stopBins,   2,isrWeightBins));
  vh_totweight_signal_isr.push_back(new TH3D("totweight_isr_TChiWZ","TChiWZ;m_{#tilde{#chi}^{#pm}_{0}=#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight",                              401,stopBins,   401,stopBins,   2,isrWeightBins));
  vh_totweight_signal_isr.push_back(new TH3D("totweight_isr_TChiHH","TChiHH;m_{#tilde{#chi}^{#pm}_{0}=#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight",                              401,stopBins,   401,stopBins,   2,isrWeightBins));
  vh_totweight_signal_isr.push_back(new TH3D("totweight_isr_T5qqqqZH","T5qqqqZH;m_{#tilde{g}} (GeV);m_{#tilde{#chi}^{0}_{2}} (GeV);Total Weight",        201,gluinoBins, 201,gluinoBins, 2,isrWeightBins));
  //vh_totweight_signal_isr.push_back(new TH3D("totweight_isr_T6bbZH","T6bbZH;m_{#tilde{t}} (GeV);#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Total Weight", 401,stopBins,   401,stopBins,   2,isrWeightBins));
  // signal weight
  vh_xsec_signal         .push_back(new TH2D("xsec_T1tttt",         "T1tttt or T5ttcc or T5tttt;m_{#tilde{g}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Cross-section (pb)",  201,gluinoBins, 201,gluinoBins));
  vh_weightnorm_signal   .push_back(new TH2D("weightnorm_T1tttt",   "T1tttt or T5ttcc or T5tttt;m_{#tilde{g}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);weight norm. factor", 201,gluinoBins, 201,gluinoBins));
  vh_xsec_signal         .push_back(new TH2D("xsec_T2tt",           "T2tt;m_{#tilde{t}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Cross-section (pb)",  401,stopBins, 401,stopBins));
  vh_weightnorm_signal   .push_back(new TH2D("weightnorm_T2tt",     "T2tt;m_{#tilde{t}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);weight norm. factor", 401,stopBins, 401,stopBins));
  vh_xsec_signal         .push_back(new TH2D("xsec_TChiWZ",         "TChiWZ;m_{#tilde{#chi}^{#pm}_{0}=#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Cross-section (pb)",  401,stopBins, 401,stopBins));
  vh_weightnorm_signal   .push_back(new TH2D("weightnorm_TChiWZ",   "TChiWZ;m_{#tilde{#chi}^{#pm}_{0}=#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);weight norm. factor", 401,stopBins, 401,stopBins));
  vh_xsec_signal         .push_back(new TH2D("xsec_TChiHH",         "TChiHH;m_{#tilde{#chi}^{0}_{3}=#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Cross-section (pb)",  401,stopBins, 401,stopBins));
  vh_weightnorm_signal   .push_back(new TH2D("weightnorm_TChiHH",   "TChiHH;m_{#tilde{#chi}^{0}_{3}=#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);weight norm. factor", 401,stopBins, 401,stopBins));
  vh_xsec_signal         .push_back(new TH2D("xsec_T5qqqqZH",       "T5qqqqZH;m_{#tilde{g}} (GeV);m_{#tilde{#chi}^{0}_{2}} (GeV);Cross-section (pb)",  201,gluinoBins, 201,gluinoBins));
  vh_weightnorm_signal   .push_back(new TH2D("weightnorm_T5qqqqZH", "T5qqqqZH;m_{#tilde{g}} (GeV);m_{#tilde{#chi}^{0}_{2}} (GeV);weight norm. factor", 201,gluinoBins, 201,gluinoBins));
  // --> too much memory
  //vh_xsec3D_signal       .push_back(new TH3D("xsec_T6bbZH",         "T6bbZH;m_{#tilde{#chi}^{#pm}_{0} (GeV);#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);Cross-section (pb)",  401,stopBins, 401,stopBins, 401,stopBins));
  //vh_weightnorm3D_signal .push_back(new TH3D("weightnorm_T6bbZH",   "T6bbZH;m_{#tilde{t}} (GeV);#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);weight norm. factor", 401,stopBins, 401,stopBins, 401,stopBins));
  // npv for extrapolations
  h_npvLowHigh_data               = new TH1D("npvLowHigh_data",     "Number of vertices - Data;N_{Vertices}", 2,npvLowHighBins);
  vh_npvLowHigh_signal   .push_back(new TH3D("npvLowHigh_T1tttt",   "T1tttt or T5ttcc or T5tttt;m_{#tilde{g}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);N_{vertex}", 201,gluinoBins, 201,gluinoBins, 2,npvLowHighBins));
  vh_npvLowHigh_signal   .push_back(new TH3D("npvLowHigh_T2tt",     "T2tt;m_{#tilde{t}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);N_{vertex}",                       401,stopBins, 401,stopBins,     2,npvLowHighBins));
  vh_npvLowHigh_signal   .push_back(new TH3D("npvLowHigh_TChiWZ",   "TChiWZ;m_{#tilde{#chi}^{#pm}_{0}=#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);N_{vertex}", 401,stopBins, 401,stopBins,     2,npvLowHighBins));
  vh_npvLowHigh_signal   .push_back(new TH3D("npvLowHigh_TChiHH",   "TChiHH;m_{#tilde{#chi}^{0}_{3}=#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);N_{vertex}", 401,stopBins, 401,stopBins,     2,npvLowHighBins));
  vh_npvLowHigh_signal   .push_back(new TH3D("npvLowHigh_T5qqqqZH", "T5qqqqZH;m_{#tilde{g}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);N_{vertex}", 201,gluinoBins, 201,gluinoBins, 2,npvLowHighBins));
  //vh_npvLowHigh_signal   .push_back(new TH3D("npvLowHigh_T6bbZH",   "T6bbZH;m_{#tilde{t}} (GeV);#tilde{#chi}^{0}_{2}} (GeV);m_{#tilde{#chi}^{0}_{1}} (GeV);N_{vertex}", 201,gluinoBins,201,gluinoBins, 201,gluinoBins, 2,npvLowHighBins));
  // pileup
  h_pileup_data                = new TH1D("pileup_data",        "Pile-up distribution - Data (Nominal);Pile-up", 100,0,100);
  h_pileup_data_down           = new TH1D("pileup_data_down",   "Pile-up distribution - Data (down);Pile-up",    100,0,100);
  h_pileup_data_up             = new TH1D("pileup_data_up",     "Pile-up distribution - Data (up);Pile-up",      100,0,100);
  h_pileup_mc                  = new TH1D("pileup_mc",          "Pile-up distribution - MC;Pile-up",             100,0,100);
  h_pileup_weight              = new TH1D("pileup_weight",      "Pile-up weights - Nominal MB X-sec (69 mb);Pile-up;Weight",    100,0,100);
  h_pileup_weight_down         = new TH1D("pileup_weight_down", "Pile-up weights - MB X-sec up 5% (72.45 mb);Pile-up;Weight",   100,0,100);
  h_pileup_weight_up           = new TH1D("pileup_weight_up",   "Pile-up weights - MB X-sec down 5% (65.55 mb);Pile-up;Weight", 100,0,100);
  h_nvtx                       = new TH1D("nvtx",               "Number of vertices - Nominal;N_{Vertices}",                      101,-0.5,100.5);
  h_nvtx_rw                    = new TH1D("nvtx_rw",            "Number of vertices - Pile-up reweighted (MC only);N_{Vertices}", 101,-0.5,100.5);

  // trigger efficiency
  double htbins[19]  = { 0, 200, 300, 400, 500, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1200, 1500, 2000, 4000, 10000 };
  double HTB[12] = {400, 500, 600, 700, 750, 800, 850, 900, 950, 1000, 1500, 10000};
  double ptB[9]  = {200, 300, 400, 450, 500, 550, 600, 1000, 10000};
  h_trigger_pass                = new TH1D("trigger_pass",    "Pass trigger;H_{T} (GeV)", 18,htbins);
  h_trigger_total               = new TH1D("trigger_total",          "Total;H_{T} (GeV)", 18,htbins);
  h_trigger2d_pass              = new TH2D("trigger2d_pass",  "Pass trigger;H_{T} (GeV);Leading AK8 jet p_{T} (GeV)", 11,HTB, 8,ptB);
  h_trigger2d_total             = new TH2D("trigger2d_total",        "Total;H_{T} (GeV);Leading AK8 jet p_{T} (GeV)", 11,HTB, 8,ptB);

}

//_______________________________________________________
//               Fill Histograms here
void
Weighting::fill_weight_histos(const bool& varySystematics, const bool& runOnSkim, const unsigned int& syst_index, const double& weight)
{
  if (runOnSkim) {
    if (syst_index == 0) {
      // trigger efficiency, measured in single lepton datasets
      // SingleElectron dataset: Pass HLT_Ele27_WPTight_Gsf && 1 Electron
      // SingleMuon     dataset: Pass HLT_IsoMu27 && 1 Muon
      // SinglePhoton   dataset: Pass all single photon triggers && 1 Photon
      // Baseline cuts to be applied: 3 jets, 1 AK8 jet, MR & R^2
      bool pass_aux_trigger = 0;
      int njet = v.Jet.Jet.n;
      double R2 = v.R2;
      double MR = v.MR;
      if (v.sample.Contains("MET")) {
        if (v.HLT_PFMET120_PFMHT120_IDTight==1&&v.nLepVeto==0&&v.Tau.Veto.n==0) pass_aux_trigger = 1;
      } else if (v.sample.Contains("SingleElectron")) {
        if ((v.HLT_Ele32_WPTight_Gsf==1)&&v.Electron.Veto.n==1&&v.Muon.Veto.n==0) pass_aux_trigger = 1;
      } else if (v.sample.Contains("SingleMuon")) {
        if (v.HLT_IsoMu27==1&&v.Muon.Veto.n==1&&v.Electron.Veto.n==0) pass_aux_trigger = 1;
      } else if (v.sample.Contains("SinglePhoton")) {
        bool OR_HLT_Photon =
          v.HLT_Photon200==1 ||
          v.HLT_Photon300_NoHE==1;
        if (OR_HLT_Photon==1&&v.Photon.Select.n==1&&v.nLepVeto==0&&v.Tau.Veto.n==0) pass_aux_trigger = 1;
        njet = v.Jet.JetNoPho.n;
        R2 = v.R2_pho;
        MR = v.MR_pho;
      }
      if (pass_aux_trigger) {
        if (v.FatJet.JetAK8.n>=1 && njet>=3 && MR>=800 && R2>=0.08) {
          if (v.HLT_AK8PFJet450==1 || v.HLT_PFHT1050==1) {
            h_trigger_pass  ->Fill(v.AK4_Ht);
            h_trigger2d_pass->Fill(v.AK4_Ht, v.FatJet.JetAK8(0).pt);
          }
          h_trigger_total   ->Fill(v.AK4_Ht);
          h_trigger2d_total ->Fill(v.AK4_Ht, v.FatJet.JetAK8(0).pt);
        }
      }
    } // end syst_index == 0x
  } else {
    // Skimming only histos
    // Save the number of vertices in Data and Signal
    if (v.isData) {
      // Use baseline selection
      if (v.FatJet.JetAK8.n>=1 && v.Jet.Jet.n>=3 && v.MR>=800 && v.R2>=0.08) {
        if (v.HLT_AK8PFJet450==1 || v.HLT_PFHT1050==1) h_npvLowHigh_data->Fill(v.PV_npvsGood);
      }
    } else if (v.isSignal) {
      if (v.sample.Contains("T2tt")) vh_npvLowHigh_signal[1]->Fill(v.susy_mass[0], v.susy_mass[1], v.PV_npvsGood);
      else                          vh_npvLowHigh_signal[0]->Fill(v.susy_mass[0], v.susy_mass[1], v.PV_npvsGood);
    }
  }
}


//_______________________________________________________
//           Read cross-section from ntuple
double
Weighting::get_xsec_from_ntuple(const std::vector<std::string>& filenames, const bool& runOnSkim)
{
  float evt_XSec=0, prev_XSec=0;
  for (const auto& filename : filenames) {
    TFile *f = TFile::Open(filename.c_str());
    TTree* tree = (TTree*)f->Get(runOnSkim ? "B2GTree" : "B2GTTreeMaker/B2GTree");
    tree->GetBranch("evt_XSec")->SetAddress(&evt_XSec);
    tree->GetEntry(0);
    f->Close();
    if (prev_XSec!=0&&prev_XSec!=evt_XSec) {
      error("Weighting - Files added with different cross-sections. Please, add them separately!");
      return 0;
    }
    prev_XSec = evt_XSec;
  }
  return evt_XSec;
}

//_______________________________________________________
//           Read cross-section from txt file
std::pair<double, double>
Weighting::get_xsec_totweight_from_txt_file(const std::string& txt_file)
{
  double XSec = 0, Totweight = 0;
  std::ifstream xsecFile(txt_file.c_str());
  if ( !xsecFile.good() ) {
    return std::make_pair(0,0);
    std::cout<<"Unable to open cross-section file: "<<txt_file<<std::endl;
    error("Please provide the correct txt file for Cross-sections in settings.h!");
  } else {

    std::string line;
    std::string shortname, primary_dataset;
    double xsec, totweight;
    while ( std::getline(xsecFile, line) ) {
      std::stringstream nth_line;
      nth_line<<line;
      nth_line>>shortname;
      nth_line>>primary_dataset;
      nth_line>>xsec;
      nth_line>>totweight;
      // For skimmed samples, remove certain postfixes
      // Please, synchronize with setup.py script
      //std::string dirname = sample;
      //for (std::string pf : { "_2", "_ext1", "_ext2", "_ext3", "_ext4", "_backup", "_unskimmed" })
      //if (TString(dirname).EndsWith(pf.c_str())) dirname.erase(dirname.size()-pf.size(), pf.size());
      if (v.sample.Contains(shortname)) {
        XSec = xsec;
        Totweight = totweight;
      }

    }
  }
  if (XSec == 0) {
    std::cout<<"No crossection found for "<<v.sample<<" in cross section file: "<<txt_file<<std::endl;
    error("Please fix the cross-section file in settings.h!");
  }

  return std::make_pair(XSec, Totweight);
}

//_______________________________________________________
//          Read total weight from ntuple histos
double
Weighting::get_totweight_from_ntuple(const std::vector<std::string>& filenames, const bool& runOnSkim)
{
  // Merging totweight histos
  for (const auto& filename : filenames) {
    TFile* f = TFile::Open(filename.c_str());
    h_totweight->Add((TH1D*)f->Get(runOnSkim ? "totweight" : "EventCounter/totweight"));
    //std::cout<<f<<" "<<filename<<" "<<histoname.c_str()<<" "<<f->Get(histoname.c_str())<<" "<<h_totweight->GetBinContent(1)<<std::endl;
    f->Close();
  }
  return h_totweight->GetBinContent(1);
}

//_______________________________________________________
//       Calculate weight normalization for signal
void
Weighting::calc_signal_weightnorm(const std::vector<std::string>& filenames, const double& intLumi, const bool& varySystematics, TDirectory* dir, bool verbose=0)
{
  // Find the index of the current signal
  std::string weightname;
  if (v.year== 2018) {
    if (TString(filenames[0]).Contains("T1tttt"))        { v.signal_index = 0; weightname = "data/SMS-T1tttt_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIAutumn18NanoAODv5.root"; }
    else if (TString(filenames[0]).Contains("T5ttcc"))   { v.signal_index = 0; weightname = "data/SMS-T5ttcc_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIAutumn18NanoAODv5.root"; }
    else if (TString(filenames[0]).Contains("T5qqqqVV")) { v.signal_index = 0; weightname = "data/SMS-T5qqqqVV_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIAutumn18NanoAODv5.root"; }
    else if (TString(filenames[0]).Contains("T2tt"))     { v.signal_index = 1; weightname = "data/SMS-T2tt_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIAutumn18NanoAODv5.root"; }
    else if (TString(filenames[0]).Contains("T2bW"))     { v.signal_index = 1; weightname = "data/SMS-T2bW_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIAutumn18NanoAODv5.root"; }
    else if (TString(filenames[0]).Contains("TChi"))     { v.signal_index = 2; weightname = "data/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIAutumn18NanoAODv5.root"; }
  } else if (v.year==2017) {
    if (TString(filenames[0]).Contains("T1tttt"))        { v.signal_index = 0; weightname = "data/SMS-T1tttt_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17NanoAODv4.root"; }
    else if (TString(filenames[0]).Contains("T5ttcc"))   { v.signal_index = 0; weightname = "data/SMS-T5ttcc_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17NanoAODv4.root"; }
    else if (TString(filenames[0]).Contains("T5qqqqVV")) { v.signal_index = 0; weightname = "data/SMS-T5qqqqVV_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17NanoAODv4.root"; }
    else if (TString(filenames[0]).Contains("T2tt"))     { v.signal_index = 1; weightname = "data/SMS-T2tt_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17NanoAODv4.root"; }
    else if (TString(filenames[0]).Contains("T2bW"))     { v.signal_index = 1; weightname = "data/SMS-T2bW_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17NanoAODv4.root"; }
    else if (TString(filenames[0]).Contains("TChiHH"))   { v.signal_index = 3; weightname = "data/SMS-TChiHH_HToBB_HToBB_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17NanoAODv4.root"; } //totweight_TChiHH
    else if (TString(filenames[0]).Contains("TChiWZ"))   { v.signal_index = 2; weightname = "data/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17NanoAODv5.root"; }
    else if (TString(filenames[0]).Contains("TChiWH"))   { v.signal_index = 2; weightname = "data/SMS-TChiWH_RunIIFall17NanoAODv5.root"; }
    else if (TString(filenames[0]).Contains("T5qqqqZH")) { v.signal_index = 4; weightname = "data/SMS-T5qqqqZH_TuneCUETP8M1_13TeV-madgraphMLM-pythia8NanoAODv5.root"; }
    else if (TString(filenames[0]).Contains("T6bbZH"))   { v.signal_index = 5; weightname = "data/SMS-T6bbZH_RunIIFall17NanoAODv5.root"; } //totweight_T6bbZH
  }

  // Merge totweight histos
  std::map<int, double> xsec_mother;
  //TFile* f = TFile::Open(filenames[0].c_str());
  TFile* f = TFile::Open(weightname.c_str());
  // Get total weight
  if (v.signal_index==0 || v.signal_index==4) {
    vh_totweight_signal[v.signal_index]->Add((TH2D*)f->Get("totweight_T1tttt"));
  }
  else if (v.signal_index==3) {
    vh_totweight_signal[v.signal_index]->Add((TH2D*)f->Get("totweight_TChiHH"));
  }
  else if (v.signal_index==5) {
    TH3D *h = (TH3D*)f->Get("totweight_T6bbZH");
    for (int binx=1; binx<=401; ++binx) for (int biny=1; biny<=401; ++biny) for (int binz=1; binz<=401; ++binz)
      if (h->GetBinContent(binx,biny,binz)) totweight3D_signal[binx+401*(biny+401*binz)] = h->GetBinContent(binx,biny,binz);
  } else {
    vh_totweight_signal[v.signal_index]->Add((TH2D*)f->Get("totweight_T2tt")); // has equivalent binning to TChiWZ
  }
  f->Close();

  // Set xsec for each gluino/stop mass bin
  // Read gluino/stop xsec from same file used in TTree step
  std::map<uint32_t, std::string> signal_bins;

  if(v.signal_index == 5){
    if (verbose) std::cout<<"Normalization variables:"<<std::endl;
    // Calculate weight normalization
    // weightnorm = (settings.intLumi*xsec)/totweight;
    for (const auto& totw : totweight3D_signal) {
      double mMother = ((totw.first%402)-1)*5;
      double mLSP2   = (((totw.first/402)%402)-1)*5;
      double mLSP1   = (((totw.first/402)/402)-1)*5;
      double xsec    = GetStopXSec(mMother).first;
      double tot     = totw.second;
      double wnorm   = intLumi * xsec / tot;
      weightnorm3D_signal[totw.first] = wnorm;
      if (verbose)
        std::cout<<"  Bin: M(b~)="<<mMother<<" M(LSP2)="<<mLSP2<<" M(LSP1)="<<mLSP1<<":   xsec="<<xsec<<" totweight="<<tot<<" weightnorm="<<wnorm<<std::endl;
      uint32_t bin = totw.first;
      std::stringstream ss;
      ss<<"_"<<mMother<<"_"<<mLSP1<<"_"<<mLSP2;
      signal_bins[bin] = ss.str();
    }
  }
  else {
    for (int binx=1, nbinx=vh_xsec_signal[v.signal_index]->GetNbinsX(); binx<=nbinx; ++binx) {
      double mMother = vh_xsec_signal[v.signal_index]->GetXaxis()->GetBinCenter(binx);
      xsec_mother[binx] = (v.signal_index==0 || v.signal_index==4) ? GetGluinoXSec(mMother).first : ((v.signal_index==1 || v.signal_index==5) ? GetStopXSec(mMother).first : (v.signal_index==3 ? GetNeutralinoXSec(mMother).first : GetCharginoXSec(mMother).first)); // first: mean xsec (pb), second: error (%)
      for (int biny=1, nbiny=vh_xsec_signal[v.signal_index]->GetNbinsY(); biny<=nbiny; ++biny)
        vh_xsec_signal[v.signal_index]->SetBinContent(binx, biny, xsec_mother[binx]);
    }
    // Calculate weight normalization
    // weightnorm = (settings.intLumi*xsec)/totweight;
    // Divide(h1,h2,c1,c2) --> c1*h1/(c2*h2)
    vh_weightnorm_signal[v.signal_index]->Divide(vh_xsec_signal[v.signal_index], vh_totweight_signal[v.signal_index], intLumi);
    if (verbose) std::cout<<"Normalization variables:"<<std::endl;
    for (int binx=1, nbinx=vh_xsec_signal[v.signal_index]->GetNbinsX(); binx<=nbinx; ++binx) {
      for (int biny=1, nbiny=vh_xsec_signal[v.signal_index]->GetNbinsY(); biny<=nbiny; ++biny) {
        double mMother = vh_xsec_signal[v.signal_index]->GetXaxis()->GetBinCenter(binx);
        double mLSP = vh_xsec_signal[v.signal_index]->GetYaxis()->GetBinCenter(biny);
        double xsec  = vh_xsec_signal[v.signal_index]      ->GetBinContent(binx, biny);
        double totw  = vh_totweight_signal[v.signal_index] ->GetBinContent(binx, biny);
        double wnorm = vh_weightnorm_signal[v.signal_index]->GetBinContent(binx, biny);
        if (totw>0) {
          if (verbose) {
            if (v.signal_index==0) std::cout<<"  Bin: M(g~)=" <<mMother<<" M(LSP)="<<mLSP<<":   xsec="<<xsec<<" totweight="<<totw<<" weightnorm="<<wnorm<<std::endl;
            if (v.signal_index==1) std::cout<<"  Bin: M(s~)="<<mMother<<" M(LSP)="<<mLSP<<":   xsec="<<xsec<<" totweight="<<totw<<" weightnorm="<<wnorm<<std::endl;
            if (v.signal_index==2) std::cout<<"  Bin: M(chi~)="<<mMother<<" M(LSP)="<<mLSP<<":   xsec="<<xsec<<" totweight="<<totw<<" weightnorm="<<wnorm<<std::endl;
            if (v.signal_index==3) std::cout<<"  Bin: M(LSP2)="<<mMother<<" M(LSP)="<<mLSP<<":   xsec="<<xsec<<" totweight="<<totw<<" weightnorm="<<wnorm<<std::endl;
            if (v.signal_index==4) std::cout<<"  Bin: M(g~)="<<mMother<<" M(LSP2)="<<mLSP<<":   xsec="<<xsec<<" totweight="<<totw<<" weightnorm="<<wnorm<<std::endl;
          }
          uint32_t bin = mMother * 10000 + mLSP;
          std::stringstream ss;
          ss<<"_"<<mMother<<"_"<<mLSP;
          signal_bins[bin] = ss.str();
        }
      }
    }
  }
}

double Weighting::get_signal_weightnorm() {
  v.get_signal_mass();
  if (v.signal_index==5) {
    return weightnorm3D_signal[size_t((v.susy_mass[0]/5)+1 + 402*((v.susy_mass[2]/5)+1+402*((v.susy_mass[1]/5)+1)))];
  } else {
    return vh_weightnorm_signal[v.signal_index]->GetBinContent(vh_weightnorm_signal[v.signal_index]->FindBin(v.susy_mass[0], v.susy_mass[1]));
  }
}


//_______________________________________________________
//             Load pile-up reweighting infos
void
Weighting::init_pileup_reweighting(const std::string& pileupDir, const bool& runOnSkim, const std::vector<std::string>& filenames)
{
  // Get data histogram (generated by pileupCalc.py script)
  TFile* f_pileup_data = TFile::Open((pileupDir+"data_pileup.root").c_str());
  h_pileup_data->Add((TH1D*)f_pileup_data->Get("pileup"));
  f_pileup_data->Close();
  // Also get up/down variations
  TFile* f_pileup_data_down = TFile::Open((pileupDir+"data_pileup_down.root").c_str());
  h_pileup_data_down->Add((TH1D*)f_pileup_data_down->Get("pileup"));
  f_pileup_data_down->Close();
  TFile* f_pileup_data_up = TFile::Open((pileupDir+"data_pileup_up.root").c_str());
  h_pileup_data_up->Add((TH1D*)f_pileup_data_up->Get("pileup"));
  f_pileup_data_up->Close();
  // get mc histogram (used to generate mc pile-up)
  TFile* f_pileup_mc = TFile::Open((pileupDir+"mc_pileup.root").c_str());
  h_pileup_mc->Add((TH1D*)f_pileup_mc->Get("pileup"));
  f_pileup_mc->Close();
  // // Get mc histogram saved inside the ntuple (unfiltered pileup distribution)
  // std::cout<<h_pileup_mc->GetEntries()<<std::endl;
  // for (const auto& filename : filenames) {
  //   TFile* f_pileup_mc = TFile::Open(filename.c_str());
  //   h_pileup_mc->Add((TH1D*)f_pileup_mc->Get(runOnSkim ? "pileup_mc" : "EventCounter/pileup"));
  //   f_pileup_mc->Close();
  //   std::cout<<h_pileup_mc->GetEntries()<<std::endl;
  // }
  // Divide normalized data histo by normalized mc histo to get pileup weights for each bin
  h_pileup_weight     ->Divide(h_pileup_data,      h_pileup_mc, 1/h_pileup_data->Integral(),      1/h_pileup_mc->Integral());
  h_pileup_weight_down->Divide(h_pileup_data_down, h_pileup_mc, 1/h_pileup_data_down->Integral(), 1/h_pileup_mc->Integral());
  h_pileup_weight_up  ->Divide(h_pileup_data_up,   h_pileup_mc, 1/h_pileup_data_up->Integral(),   1/h_pileup_mc->Integral());
}

//_______________________________________________________
//              function to get scaled weight
double
Weighting::get_syst_weight(const double& weight_nominal, const double& weight_up, const double& weight_down, const double& nSigma)
{
  double w = weight_nominal;
  if (nSigma == 0) {
    return w;
  } else {
    // Compute the weight according to the systematic variation considered
    // Use difference between nominal and up/down as 1 sigma variation
    double dw_up = weight_up - weight_nominal;
    double dw_down = weight_nominal - weight_down;
    if (nSigma >= 0.) {
      w += nSigma*dw_up;
    } else {
      w += nSigma*dw_down;
    }
    return w;
  }
}

double
Weighting::get_syst_weight(const double& weight_nominal, const double& uncertainty, const double& nSigma)
{
  double w = weight_nominal;
  // Use symmetrical difference for up/down variation
  if (nSigma!=0.) w *= 1.0 + nSigma * uncertainty;
  return w;
}


//_______________________________________________________
//                  Top pt reweighting
double
Weighting::get_toppt_weight(const double& nSigmaToppt, const unsigned int& syst_index, const bool& runOnSkim)
{
  double w_nom = 1;//, n=0;
  if (!v.isData) while (v.GenPart.Top.Loop()) {
    double a = 0.0615, b = -0.0005;
    w_nom *= std::exp(a + b * v.GenPart.Top().pt);
  }
  w_nom = std::sqrt(w_nom);
  //std::cout<<"N top = "<<v.GenPart.Top.n<<" w_nom = "<<w_nom<<std::endl<<std::endl;
  double w_toppt_up = 1;
  double w_toppt = std::sqrt(w_nom);
  double w_toppt_down = w_nom; // Nominal weight is typically below 1 (below top pt>123) --> use as down variation
  double w = get_syst_weight(w_toppt, w_toppt_up, w_toppt_down, nSigmaToppt);
  if (syst_index==0&&!runOnSkim) {
    h_totweight_toppt->Fill(0);
    h_totweight_toppt->Fill(1, w_toppt);
  }
  return w;
}

//_______________________________________________________
//                    ISR reweighting
double
Weighting::get_isr_weight(const double& nSigmaISR, const unsigned int& syst_index, const bool& runOnSkim)
{
  // Implementing the reweighting in this presentation:
  // https://indico.cern.ch/event/592621/contributions/2398559/attachments/1383909/2105089/16-12-05_ana_manuelf_isr.pdf
  // Using the values found on slide 8 (T2tt and T1tttt)
  double w = 1; // NanoAOD don't have ISR information, Maybe signal have them
  // ttbar ISR reweighting not needed, we do top pt reweighting!
  /*
    if (v.isSignal) {
    double d = v.sample.Contains("T2tt") ? 1.121 : 1.143;
    int n = data.evt.NISRJets;
    double w = 0;
    if      (n==0) w = d;
    else if (n==1) w = d * 0.920;
    else if (n==2) w = d * 0.821;
    else if (n==3) w = d * 0.715;
    else if (n==4) w = d * 0.662;
    else if (n==5) w = d * 0.561;
    else           w = d * 0.511;
    double err = (1-w)/2;
    double w_isr_up   = w + err;
    double w_isr      = w;
    double w_isr_down = w - err;
    w = get_syst_weight(w_isr, w_isr_up, w_isr_down, nSigmaISR);
    if (syst_index==0&&!runOnSkim) {
    if (v.sample.Contains("T2tt")) {
    vh_totweight_signal_isr[1]->Fill(data.evt.SUSY_Stop_Mass,   data.evt.SUSY_LSP_Mass, 0);
    vh_totweight_signal_isr[1]->Fill(data.evt.SUSY_Stop_Mass,   data.evt.SUSY_LSP_Mass, 1, w_isr);
    } else {
    vh_totweight_signal_isr[0]->Fill(data.evt.SUSY_Gluino_Mass, data.evt.SUSY_LSP_Mass, 0);
    vh_totweight_signal_isr[0]->Fill(data.evt.SUSY_Gluino_Mass, data.evt.SUSY_LSP_Mass, 1, w_isr);
    }
    }
    }*/
  return w;
}

//_______________________________________________________
//                  Get pile-up weight
double
Weighting::get_pileup_weight(const double weight, const double& nSigmaPU, const unsigned int& syst_index, const bool& runOnSkim)
{
  if (!v.isSignal) {
    // Background
    int pu_bin = v.Pileup_nTrueInt+1; // eg. pileup 0, is filled in bin 1
    double w_pileup = h_pileup_weight->GetBinContent(pu_bin);
    double w_pileup_up = h_pileup_weight_up->GetBinContent(pu_bin);
    double w_pileup_down = h_pileup_weight_down->GetBinContent(pu_bin);
    double w = get_syst_weight(w_pileup, w_pileup_up, w_pileup_down, nSigmaPU);
    if (syst_index==0&&!runOnSkim) {
      h_totweight_pileup->Fill(0);
      h_totweight_pileup->Fill(1, w_pileup);
    }
    if (syst_index == 0) {
      h_nvtx   ->Fill(v.PV_npvsGood, weight);
      h_nvtx_rw->Fill(v.PV_npvsGood, weight*w);
    }
    return w;
  } else {
    // Signal
    // Do not reweight, but split to a low/high pileup region
    if (nSigmaPU==1) return v.PV_npvsGood>=h_npvLowHigh_data->GetBinLowEdge(2);
    else if (nSigmaPU==-1) return v.PV_npvsGood<h_npvLowHigh_data->GetBinLowEdge(2);
    return 1;
  }
}


//____________________________________________________
//                  HT reweighting

double
Weighting::get_ht_weight(const double& nSigmaHT)
{
  // Using method described by Julie Hogan:
  // https://indico.cern.ch/event/508384/contributions/2029874/attachments/1255336/1852975/JetRwtIssues_B2GWkshp_040816.pdf
  // Use linear functions calculated with scripts/CalcHTScaleFactors.C macro
  // linear function(s): p0 + p1 * HT

  // Silver JSON
  /*
    const double p0[2]     = { 1.16434, 1.00188 };
    const double p0_err[2] = { 0.00459931, 0.0266651 };
    const double p1[2]     = { -0.000142391, -7.80628e-05 };
    const double p1_err[2] = { 3.62929e-06, 1.11035e-05 };
  */
  
  // Golden JSON
  const double p0[2]     = { 1.17155, 1.00513 };
  const double p0_err[2] = { 0.00477137, 0.028861 };
  const double p1[2]     = { -0.000143935, -7.81881e-05 };
  const double p1_err[2] = { 3.79477e-06, 1.20209e-05 };

  // Calculate unscaled jet HT
  double ht = 0;
  while (v.FatJet.Loop()) ht += v.FatJet().pt;
  
  double w = 1.0;
  if (ht>=800&&ht<2000)
    w *= get_syst_weight(p0[0], p0_err[0]/p0[0], nSigmaHT) + get_syst_weight(p1[0], p1_err[0]/p1[0], nSigmaHT) * ht;
  else if (ht>=2000)
    w *= get_syst_weight(p0[1], p0_err[1]/p0[1], nSigmaHT) + get_syst_weight(p1[1], p1_err[1]/p1[1], nSigmaHT) * ht;

  return w;
}


//_______________________________________________________
//                  Get alpha_s weight
double
Weighting::get_alphas_weight(const double& nSigmaAlphaS, const int& LHA_PDF_ID)
{
  /*
  std::vector<float> alphas_Weights; // not available in NanoAOD

  // A set of two weights corresponding to
  // Powheg:  alpha_s = 0.118 -+ 0.002
  // aMC@NLO: alpha_s = 0.118 -+ 0.001
  // Recommendation is to use +- 0.0015 --> rescale difference by 0.75 or 1.5
  // Treat weight as usual, gaussian, rescale to desired nSigma
  double w_alphas = 1;
  //double w_alphas_up   = alphas_Weights[1];
  //double w_alphas_down = alphas_Weights[0];
  double w_alphas_up   = 0;
  double w_alphas_down = 0;
  double nSigma_0_0015 = nSigmaAlphaS;
  if (LHA_PDF_ID==260000||LHA_PDF_ID==260400) {
    // Powheg samples have -+ 0.001
    nSigma_0_0015 *= 1.5;
  } else {
    // aMC@NLO samples have -+ 0.002
    nSigma_0_0015 *= 0.75;
  }
  w_alphas = get_syst_weight(w_alphas, w_alphas_up, w_alphas_down, nSigma_0_0015);
  return w_alphas;
  */
  return 1;
}


//_______________________________________________________
//                  Get scale weight
double
Weighting::get_scale_weight(const std::vector<double>& scale_weight_norm, const double& nSigmaScale, const unsigned int& numScale)
{
  std::vector<float> scale_Weights;
  /*
    New LHEScaleWeight in NanoAOD:
    LHE scale variation weights (w_var / w_nominal); 
    v.LHEScaleWeight[0] is rensc=0.5d0 facsc=0.5d0
    v.LHEScaleWeight[1] is rensc=0.5d0 facsc=1d0
    v.LHEScaleWeight[2] is rensc=0.5d0 facsc=2d0
    v.LHEScaleWeight[3] is rensc=  1d0 facsc=0.5d0
    v.LHEScaleWeight[4] is rensc=  1d0 facsc=1d0
    v.LHEScaleWeight[5] is rensc=  1d0 facsc=2d0
    v.LHEScaleWeight[6] is rensc=  2d0 facsc=0.5d0
    v.LHEScaleWeight[7] is rensc=  2d0 facsc=1d0
    v.LHEScaleWeight[8] is rensc=  2d0 facsc=2d0 
  */
  if (nSigmaScale==0) return 1; // No systematics
  if (v.nLHEScaleWeight==0) return 1; // ST samples are known to miss scale weights
  if (scale_weight_norm.empty()) {
    error("Weighting - Scale weight normalizations were not provided for this sample, rerun scripts/get_scaleweight_norm.py on unskimmed ntuple");
  }
  double w_scale = 1;
  double w_scale_up = 1;   // Corresponds to 0.5 (More signal events)
  double w_scale_down = 1; // Corresponds to 2.0 (Less signal events)
  if (numScale==1) {
    // Vary factorization scale
    // fix mu_r = 1.0, vary mu_f = 0,5, 2.0
    w_scale_up   = v.LHEScaleWeight[3] / v.LHEWeight_originalXWGTUP;
    w_scale_down = v.LHEScaleWeight[5] / v.LHEWeight_originalXWGTUP;
  } else if (numScale==2) {
    // Vary renormalization scale
    // fix mu_f = 1.0, vary mu_r = 0,5, 2.0
    w_scale_up   = v.LHEScaleWeight[1] / v.LHEWeight_originalXWGTUP;
    w_scale_down = v.LHEScaleWeight[7] / v.LHEWeight_originalXWGTUP;
  } else if (numScale==3) {
    // Vary both simulatneously
    // mu_r = mu_f = 0,5, 2.0
    w_scale_up   = v.LHEScaleWeight[0] / v.LHEWeight_originalXWGTUP;
    w_scale_down = v.LHEScaleWeight[8] / v.LHEWeight_originalXWGTUP;
  }
  w_scale = get_syst_weight(w_scale, w_scale_up, w_scale_down, nSigmaScale);
  return w_scale;
}

//  double Weighting::calc_njet_weight(eventBuffer data, const double& nSigmaNJetWeight) {
//    // QCD Njet Reweighting
//    // First run this script to obtain the expo fit parameters
//    // root scripts/calc_njet_weight.C
//    // Up variation taken from Q region
//    // Down variation taken from G-1 region
//    double w_up = std::exp(-4.15259e-01 + 9.97141e-02 * nJet);
//    double w_dn = std::exp( 1.02951e+00 - 1.90498e-01 * nJet);
//    double w = get_syst_weight(1.0, w_up, w_dn, nSigmaNJetWeight);
//    return w;
//  }


double Weighting::calc_lostlep_weight(const double& nSigmaLostLep) {
  // Lost Lepton event weight
  // First run this script to obtain the lost lepton uncertainties
  // python scripts/calc_lostlepsyst.py
  // Use the averages combination of all leptons for W/top
  double w = 1;
  if (!v.isData) if (v.Electron.Veto.n+v.Muon.Veto.n+v.Tau.Veto.n==0) {
    // Final state leptons from W mother
    while (v.GenPart.LeptonFromW.Loop()) {
      double unc = 0;
      double abseta = std::abs(v.GenPart.LeptonFromW().eta);
      if      (abseta<0.5) unc = 0.125;
      else if (abseta<1.0) unc = 0.126;
      else if (abseta<1.5) unc = 0.129;
      else if (abseta<2.0) unc = 0.143;
      else if (abseta<2.5) unc = 0.175;
      w *= get_syst_weight(1.0, unc, nSigmaLostLep);
      //std::cout<<"Lost-lepton found: pt/eta/id = "<<v.GenPart.LeptonFromW().pt[i]<<" "
      //         <<v.GenPart.LeptonFromW().eta[i]<<" "<<v.GenPart.LeptonFromW().pdgId
      //         <<" E/M/T="<<v.Electron.Veto.n<<"/"<<v.Muon.Veto.n<<"/"<<v.Tau.Veto.n<<std::endl;
    }
  }
  return w;
}


double Weighting::calc_trigger_efficiency(const double& nSigmaTrigger) {
  /*
  // 1D trigger efficiency method
  double eff, err_up, err_down;
  if (v.FatJet.JetAK8.n>0) {
  geteff_AE(eff_trigger, v.AK4_Ht, eff, err_up, err_down);
  double w = get_syst_weight(eff, eff+err_up, eff-err_down, nSigmaTrigger);
  return w;
  } else return 0;
  */
  // 2D trigger efficiency in bins of HT and Jet1pt
  // Check the presence of a lepton/photon and apply different weights
  TH2D *h      = eff_trigger_lep;
  TH2D *h_up   = eff_trigger_lep_up;
  TH2D *h_down = eff_trigger_lep_down;
  double MET = v.MET_pt;

  if (v.Photon.PreSelect.n>=1) {
    h      = eff_trigger_pho;
    h_up   = eff_trigger_pho_up;
    h_down = eff_trigger_pho_down;
  }

  if (v.FatJet.JetAK8.n>0) {
    double eff = 0, total = 0;
    geteff2D(h, MET, v.AK4_Ht, eff, total); // total was saved to histo error
    if (total>0) {
      double eff_up   = geteff2D(h_up,   MET, v.AK4_Ht);
      double eff_down = geteff2D(h_down, MET, v.AK4_Ht);
      double w = get_syst_weight(eff, eff_up, eff_down, nSigmaTrigger);
      return w;
    } else return 0;
  } else return 0;

  
  /*
  TH3D *h      = eff_3D_trigger_lep;
  TH3D *h_up   = eff_3D_trigger_lep_up;
  TH3D *h_down = eff_3D_trigger_lep_down;
  if (v.FatJet.JetAK8.n>0) {
    double eff = 0, total = 0;
    geteff3D(h, v.FatJet.JetAK8(0).msoftdrop, v.MET_pt, v.AK4_Ht, eff, total); // total was saved to histo error
    // For the time being only weight the measurable phase space
    // Rest is 0 --> Could weight with the TGraphAsymmErrors::Efficiency value (0.5+-0.5)
    if (total>0) {
      double eff_up   = geteff3D(h_up,   v.FatJet.JetAK8(0).msoftdrop, v.MET_pt, v.AK4_Ht);
      double eff_down = geteff3D(h_down, v.FatJet.JetAK8(0).msoftdrop, v.MET_pt, v.AK4_Ht);
      double w = get_syst_weight(eff, eff_up, eff_down, nSigmaTrigger);
      return w;
    } else return 0;
  } else return 0;
  */
}

#endif // End header guard

