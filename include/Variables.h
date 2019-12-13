#ifndef VARIABLES_H
#define VARIABLES_H

// Private headers
#include "eventBuffer_small.h"
#include "Razor.h"

// 3rd party headers
#include "TLorentzVector.h"
#include "TString.h"

// common libraries
#include <iostream>
#include <functional>

#define NOVAL_F 9999.0
#define NOVAL_I 9999

//_______________________________________________________
//                 Object selections

/*
  Latest Electron IDs:
  [1] Cut Based  - https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2?rev=41#Working_points_for_2016_data_for
  [2] MVA        - https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentificationRun2?rev=30#Recommended_MVA_recipes_for_2016
  [3] SUSY (Use) - https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF?rev=172#Electrons

  Latest Isolation WPs:
  [4] SUSY MiniIso Loose/Tight - https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF?rev=172#ID_IP_ISO_AN1
  [5] RazorInclusive - https://github.com/RazorCMS/RazorAnalyzer/blob/master/src/RazorAnalyzer.cc#L1210

  Latest Impact Point Cut:
  [6] SUSY Loose/Tight IP2D (Use) - https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF?rev=172#ID_IP_ISO_AN1
  [7] POG  Tight - https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2?rev=41#Offline_selection_criteria

  For Veto (Regions except Z) Choose:
  - Spring15 Cut based Veto ID without relIso (EA) cut
  - for pt<20  absolute iso03 (EA) < 5
  - Mini-Isolation (EA)/pt < 0.1 (Medium WP [4])
  - pt >= 5
  - |eta| < 2.5
  - |d0| < 0.2, |dz| < 0.5 (Loose IP2D [6])
  OR when using MVA:
  - Spring16 MVA Loose ID
  - for pt>=20 Mini-Isolation (EA)/pt < 0.2 (RazorInclusive cut [5])
  - pt >= 5
  - |eta| < 2.5
  - 3D IP sig < 4

  For Selection (Z) Choose:
  - Spring15 Cut based Medium ID without relIso (EA) cut
  - Mini-Isolation (EA)/pt < 0.1 (Tight WP [4])
  - pt >= 10
  - |eta| < 2.5, also exclude barrel-endcap gap [1.442,1556]
  - |d0| < 0.05, |dz| < 0.1 (Tight IP2D [6])
  OR when using MVA:
  - Spring16 MVA Loose ID
  - Mini-Isolation (EA)/pt < 0.1 (Tight WP [4])
  - pt >= 10
  - |eta| < 2.5, also exclude barrel-endcap gap [1.442,1556]
  - |d0| < 0.05, |dz| < 0.1 (Tight IP2D [6])


  For Tight Selection (TriggerEff Only) Choose:
  - Spring15 Cut based Tight ID (including relIso (EA) cut)
  - pt >= 30
  - |eta| < 2.5, also exclude barrel-endcap gap [1.442,1556]
  - |d0| < 0.05, |dz| < 0.1 (Tight IP2D and IP3D [6])

  For Loose selection (deprecated) Choose:
  - Spring15 Cut based Loose ID without relIso (EA) cut
  - Mini-Isolation (EA)/pt < 0.1 (Tight WP [4])
  - pt >= 10
  - |eta| < 2.5, also exclude barrel-endcap gap [1.442,1556]
  - |d0| < 0.2, |dz| < 0.5 (Loose IP2D [6])

*/

#define ELE_VETO_PT_CUT        5
#define ELE_VETO_ETA_CUT       2.5
#define ELE_VETO_MINIISO_CUT   0.4
#define ELE_VETO_IP_D0_CUT     0.05
#define ELE_VETO_IP_DZ_CUT     0.1
#define ELE_VETO_IP_3D_CUT     4   // For skim only
#define ELE_VETO_ABSISO_CUT    5   // For skim only

#define ELE_SELECT_PT_CUT      10
#define ELE_SELECT_ETA_CUT     2.5
#define ELE_SELECT_MINIISO_CUT 0.1
#define ELE_SELECT_IP_D0_CUT   0.05
#define ELE_SELECT_IP_DZ_CUT   0.1

#define ELE_TIGHT_PT_CUT       30
#define ELE_TIGHT_ETA_CUT      2.5
#define ELE_TIGHT_IP_D0_CUT    0.05
#define ELE_TIGHT_IP_DZ_CUT    0.1
//#define ELE_TIGHT_IP_SIG_CUT   4

/*
  Latest Muon IDs (Loose/Medium):
  [1] POG Loose/Medium - https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonIdRun2?rev=28#Short_Term_Instructions_for_Mori

  Latest Isolation WPs:
  [2] SUSY MiniISo Loose/Tight - https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF?rev=172#ID_IP_ISO
  [3] POG Tight RelIso - https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonIdRun2?rev=28#Muon_Isolation
  [4] RazorInclusive - https://github.com/RazorCMS/RazorAnalyzer/blob/master/src/RazorAnalyzer.cc#L1602

  Latest Impact Point Cut (Loose/Tight):
  [5] SUSY Loose/Tight IP2D - https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF?rev=172#ID_IP_ISO

  For Veto (All except Z) Choose:
  - POG recommended Loose ID (No Iso/IP)
  - for pt>=20 Mini-Isolation (EA)/pt < 0.2 (RazorInclusive cut [4])
  - for pt<20  absolute iso04 (EA) < 10
  - pt >= 5
  - |eta| < 2.4
  - 3D IP sig < 4

  For Loose Choose:
  - POG recommended Loose ID (No Iso/IP)
  - Mini-Isolation (EA)/pt < 0.2 (Tight WP [2])
  - pt >= 10
  - |eta| < 2.4
  - |d0| < 0.2, |dz| < 0.5 (Loose IP2D [5])

  For Selection (Z) Choose:
  - POG recommended Medium ID (No Iso/IP)
  - Mini-Isolation (EA)/pt < 0.2 (tight WP [2])
  - pt >= 5
  - |eta| < 2.4
  - |d0| < 0.05, |dz| < 0.1 (Tight IP2D [5])

  For Tight Selection (TriggerEff Only) Choose:
  - POG recommended Tight ID (No Iso/IP)
  - comb. rel. Isolation (R=0.4) < 0.15 (tight WP [3])
  - pt >= 30
  - |eta| < 2.4
  - |d0| < 0.05, |dz| < 0.1 (Tight IP2D and IP3D [5])

*/

#define MU_VETO_PT_CUT         5
#define MU_VETO_ETA_CUT        2.4
#define MU_VETO_MINIISO_CUT    0.4
#define MU_VETO_IP_D0_CUT      0.2
#define MU_VETO_IP_DZ_CUT      0.5
#define MU_VETO_ABSISO_CUT     10  // For skim only
#define MU_VETO_IP_3D_CUT      4   // For skim only

#define MU_SELECT_PT_CUT       10
#define MU_SELECT_ETA_CUT      2.4
#define MU_SELECT_MINIISO_CUT  0.2
#define MU_SELECT_IP_D0_CUT    0.05
#define MU_SELECT_IP_DZ_CUT    0.1

#define MU_TIGHT_PT_CUT        30
#define MU_TIGHT_ETA_CUT       2.4
#define MU_TIGHT_RELISO_CUT    0.15
#define MU_TIGHT_IP_D0_CUT     0.05
#define MU_TIGHT_IP_DZ_CUT     0.1
#define MU_TIGHT_IP_SIG_CUT    4

/*
  Latest Tau IDs:
  https://twiki.cern.ch/twiki/bin/view/CMS/TauIDRecommendation13TeV?rev=35

  For Veto Choose:
  - POG Loose ID:
  + byLooseCombinedIsolationDeltaBetaCorr3Hits"
  - pt >= 18 (Same as MINIAOD)

  OR use:
  - Isolated charged trk = 0
  + pt>=5 (ele/mu), 10 (hadrons)
  + isolation (dR=0.3) / pt < 0.2 (ele/mu), 0.1 (hadrons)
  + |dz| < 0.1

*/

//#define  TAU_VETO_PT_CUT  18
//#define  TAU_VETO_ETA_CUT 3.0


/*
  Latest Photon IDs:
  https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2?rev=36

  For Selection Choose:
  - Spring16 Cut based Medium photon ID
  - Pass electron veto
  - pt >= 80
  - |eta| < 2.5

*/

#define PHOTON_SELECT_PT_CUT        80
#define PHOTON_SELECT_ETA_CUT       2.5

/*
  Jet ID (Oct31/Jan12 ntuple):
  https://twiki.cern.ch/twiki/bin/view/CMS/JetID13TeVRun2017

  For AK4 Jet Selection Choose:
  - Tight jet ID
  - pt > 30
  - |eta| < 2.4 //Maybe we should change the higher

  For AK8 Jet Selection Choose:
  - Tight jet ID
  - pt > 200 (this cut was lowered to 170 for skimming)
  - |eta| < 2.4 //Maybe we should change the higher

*/

#define JET_AK4_PT_CUT  40 //30 is original
#define JET_AK4_ETA_CUT 2.4
#define JET_AK8_PT_CUT  200
#define JET_AK8_ETA_CUT 2.4


/*
  Latest b-tagging WPs/SFs:
  https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X

  Choose:
  - CombinedSecondaryVertex v2
  - DeepCSV >= 0.1522 (Loose - for Veto)
  - DeepCSV >= 0.4941 (Medium - for Tag)

*/

#define B_SUBJET_DEPP_LOOSE_CUT 0.1522
#define B_DEEP_LOOSE_CUT        0.1522
#define B_DEEP_MEDIUM_CUT       0.4941
#define B_DEEP_TIGHT_CUT        0.8001


/*
  Boosted hadronic W-tagging:

  Latest WPs/SFs:
  https://twiki.cern.ch/twiki/bin/view/CMS/JetWtagging?rev=54#Working_points_and_scale_factors

  W-Tag selection:
  - AK8 CHS jets
  - pt > 200
  - |eta| < 2.4
  - 65 <= Puppi SD Mass (corrected) < 105
  - Medium WP: Puppi tau21 < 0.4

*/

#define HADW_PT_CUT               200
#define HADW_ETA_CUT              2.4
#define HADW_SD_MASS_CUT_LOW      65
#define HADW_SD_MASS_CUT_HIGH     105
#define HADW_TAU21_LOOSE_CUT      0.55 // 0.75 upper cut for low purity
#define HADW_TAU21_TIGHT_CUT      0.45 // there is a Tighter WP: 0.35
#define HADW_TAU21_TIGHT_CUT_2016 0.40 // there is a Tighter WP: 0.35

//2018
//#define HADW_TAG_HP_SF       0.964
//#define HADW_TAG_HP_SF_ERR   0.032
//#define HADW_TAG_LP_SF       1.118
//#define HADW_TAG_LP_SF_ERR   0.143
//2017
#define HADW_TAG_HP_SF       0.97
#define HADW_TAG_HP_SF_ERR   0.06
#define HADW_TAG_LP_SF       1.14
#define HADW_TAG_LP_SF_ERR   0.29

#define HADW_TAG_JMS_SF      0.982
#define HADW_TAG_JMS_SF_ERR  0.004
#define HADW_TAG_JMR_SF      1.09
#define HADW_TAG_JMR_SF_ERR  0.05
#define HADW_TAG_SIGMA_MC    10.1

/*
  Boosted hadronic Z-tagging:

  Latest WPs/SFs:
  https://twiki.cern.ch/

*/

#define HADZ_PT_CUT            200
#define HADZ_ETA_CUT           2.4
#define HADZ_SD_MASS_CUT_LOW   80
#define HADZ_SD_MASS_CUT_HIGH  100

/*
  Boosted hadronic H-tagging:

  Latest WPs/SFs:
  https://twiki.cern.ch/

*/

#define HADH_PT_CUT            300
#define HADH_ETA_CUT           2.4
#define HADH_SD_MASS_CUT_LOW   100
#define HADH_SD_MASS_CUT_HIGH  140

/*

Top Tagging working points (No subjet B tag):

Latest WPs/SFs:
https://twiki.cern.ch/twiki/bin/view/CMS/JetTopTagging?rev=14#13_TeV_working_points_CMSSW_8_0

Choose:
- Tightest selection
- AK8 CHS jets
- 105 < Puppi SD Mass < 210
- Puppi tau32 < 0.46


Top Tagging working point (with subjet b tag):

Latest WPs/SFs:
https://twiki.cern.ch/twiki/bin/view/CMS/JetTopTagging?rev=14#13_TeV_working_points_CMSSW_8_0
https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X

Choose:
- Use Tightest selection
- AK8 CHS jets
- 105 <= Puppi SD Mass < 210
- Puppi tau32 < 0.46
- max subjet BTag DeepCSV > 0.1522
*/

#define HADTOP_PT_CUT            500
#define HADTOP_ETA_CUT           2.4
#define HADTOP_SD_MASS_CUT_LOW   105
#define HADTOP_SD_MASS_CUT_HIGH  210
#define HADTOP_TAU32_CUT        0.46

#define HADTOP_TAG_SF           1.05
#define HADTOP_TAG_SF_ERR_UP    0.07
#define HADTOP_TAG_SF_ERR_DOWN  0.04

#define LEPTOP_PT_CUT            300
#define LEPTOP_ETA_CUT           2.4
#define LEPTOP_SD_MASS_CUT_LOW    50
#define LEPTOP_SD_MASS_CUT_HIGH  200 // Not un upper cut, but a massive jet version

#define TOP_BTAG_DEEP         0.1522 // Common for lep/had tops


//_______________________________________________________
//                    Event Variables

class EventData : public eventBuffer::Event_s {
public:
  EventData(eventBuffer& d) : d_(d) {}
  
private:
  eventBuffer& d_;
  
public:
  void initEventData() {
    eventBuffer::Event_s& base = *this;
    base = std::move(d_);
    resetEventData();
  }

  // Selected object counts
  // ----------- All Leptons ----------
  size_t nLepVetoNoIso;
  size_t nLepVeto;
  size_t nLepSelect;
  size_t nLepTight;
  size_t nLepNoIso;
  size_t nLepNonIso;

  double lepNeutrinoDR;

  // --------------- MET --------------
  TVector3 MET;
  TVector3 MET_1vl;
  TVector3 MET_1l;
  TVector3 MET_2l;
  TVector3 MET_pho;
  
  // ---------- Discriminators --------
  // Signal discriminators
  double AK4_Ht;
  double AK4_HtOnline;
  double AK8_Ht;
  double MT_lepVeto;
  double MT;
  double MT_lepTight;
  double MT_lepNonIso;
  double MT_boost;
  double M_2l;
  double dPhi_2l_met;
  double dPhi_2l_jet;
  double minDeltaPhi; // Min(DeltaPhi(Jet_i, MET)), i=1,2,3,4
  double minDeltaPhi_1vl;
  double minDeltaPhi_1l;
  double minDeltaPhi_2l;
  double minDeltaPhi_pho;
  double dPhiRazor;
  double dPhiRazorNoPho;
  double dPhiRazor_new;
  double dPhiBoostedJetMET;
  double dPhiBoostedJetLep;
  double dPhiBoostedJetLepMET;
  double MR;
  double MTR;
  double R2;
  double MTR_1l;
  double R2_1l;
  double MTR_1vl;
  double R2_1vl;
  double MTR_2l;
  double R2_2l;
  double MR_pho;
  double MTR_pho;
  double R2_pho;
  double MR_new;
  double MTR_new;
  double R2_new;

  void resetEventData() {
    nLepVetoNoIso = 0;
    nLepVeto      = 0;
    nLepSelect    = 0;
    nLepTight     = 0;
    nLepNoIso     = 0;
    nLepNonIso    = 0;
    lepNeutrinoDR = NOVAL_F;
    
    MET           .SetXYZ(0,0,0);
    MET_1vl       .SetXYZ(0,0,0);
    MET_1l        .SetXYZ(0,0,0);
    MET_2l        .SetXYZ(0,0,0);
    MET_pho       .SetXYZ(0,0,0);
    
    AK4_Ht       = 0;
    AK4_HtOnline = 0;
    AK8_Ht       = 0;
    MT_lepVeto    = NOVAL_F;
    MT            = NOVAL_F;
    MT_lepTight   = NOVAL_F;
    MT_lepNonIso  = NOVAL_F;
    MT_boost      = NOVAL_F;
    M_2l          = -NOVAL_F;
    dPhi_2l_met   = NOVAL_F;
    dPhi_2l_jet   = NOVAL_F;
    minDeltaPhi     = NOVAL_F;
    minDeltaPhi_1l  = NOVAL_F;
    minDeltaPhi_1vl = NOVAL_F;
    minDeltaPhi_2l  = NOVAL_F;
    minDeltaPhi_pho = NOVAL_F;
    dPhiRazor            = NOVAL_F;
    dPhiRazorNoPho       = NOVAL_F;
    dPhiRazor_new        = NOVAL_F;
    dPhiBoostedJetMET    = NOVAL_F;
    dPhiBoostedJetLep    = NOVAL_F;
    dPhiBoostedJetLepMET = NOVAL_F;
    MR      = -NOVAL_F;
    MTR     = -NOVAL_F;
    R2      = -NOVAL_F;
    MTR_1l  = -NOVAL_F;
    R2_1l   = -NOVAL_F;
    MTR_1vl = -NOVAL_F;
    R2_1vl  = -NOVAL_F;
    MTR_2l  = -NOVAL_F;
    R2_2l   = -NOVAL_F;
    MR_pho  = -NOVAL_F;
    MTR_pho = -NOVAL_F;
    R2_pho  = -NOVAL_F;
    MR_new  = -NOVAL_F;
    MTR_new = -NOVAL_F;
    R2_new  = -NOVAL_F;
  }

};


class Variables : public EventData {

public:

  Variables(eventBuffer& d, const int& y, const bool& data, const bool& signal, const std::string& dirname) :
    EventData(d),
    year(y),
    isData(data),
    isSignal(signal),
    isBackground(!data&&!signal),
    sample(dirname),
    Electron(d),
    Muon(d),
    Tau(d),
    Photon(d),
    Jet(d),
    SubJet(d),
    FatJet(d),
    GenPart(d)
  { resetEventData(); }
  ~Variables() {}

  const int  year;
  const bool isData;
  const bool isSignal;
  const bool isBackground;
  const TString sample;

  std::vector<double> susy_mass;
  int signal_index = -1;

  //_______________________________________________________
  //                   Analysis Variables

  //_______________________________________________________
  //                    Object containers

  // Selected object container class
  // It also stores variables for a derived collection
  // which inherits its data structure from a base type
  template<typename BaseType, typename DerivedType>
  class ObjectVectorContainer {
    
  public:
    ObjectVectorContainer<BaseType, DerivedType>
    (std::vector<BaseType>& baseColl, 
     size_t default_buffer_size) : 
      baseColl_(baseColl),
      default_buffer_size_(default_buffer_size) {
      init(); 
    };
    ~ObjectVectorContainer<BaseType, DerivedType>() {};
    
  private:
    // reference to the base collection
    std::vector<BaseType>& baseColl_;
    size_t default_buffer_size_; // default buffer size from eventBuffer class

    // new data storage containers
    std::vector<DerivedType> derivedColl_;
    std::vector<TLorentzVector> v4_;

  public:
    size_t i;
    size_t n;
    
    void init() {
      i = -1;
      n = 0;
    }

    // Reset only the newly declared variables
    // Lazy implementation --> use the move constructor (FAST) for the base class
    // AND the class member initialization for the rest (new feature of C++11)
    // This way, we don't have to implement and use an initializer at all :)
    // variables can be declared and initialized in one line in the class declaration
    // this way of resetting is benefitting from this automatic initialization
    // while not compromising speed much excessive copying
    // Please, keep the base collection intact (do not pass it's reference anywhere)
    // and make sure the object survives so this method can (re-)access it's contents
    void moveData() {
      init();
      derivedColl_.clear();
      v4_.clear();
      for (auto& obj : baseColl_) {
        derivedColl_.emplace_back(std::move(obj));
        TLorentzVector v4;
        v4.SetPtEtaPhiM(obj.pt, obj.eta, obj.phi, obj.mass);
        v4_.emplace_back(v4);
      }
      n=derivedColl_.size();
    }

    bool Loop() {
      if (++i<n) { 
        return 1; 
      } else {
        i=-1;
        return 0;
      }
    }

    DerivedType& operator() () {
      if (i!=(size_t)-1) return derivedColl_[i]; 
      else {
        std::cout<<"ERROR - Variables::ObjectVectorContainer::operator(): "
                 <<"method called outside while(ObjectVectorContainer.Loop())"<<std::endl;
        std::exit(1);
      }
    };
    TLorentzVector&    v4   () { 
      if (i!=(size_t)-1) return v4_[i];
      else {
        std::cout<<"ERROR - Variables::ObjectVectorContainer::v4(): "
                 <<"method called outside while(ObjectVectorContainer.Loop())"<<std::endl;
        std::exit(1);
      }
    };
    DerivedType& operator() (size_t& iRef) { 
      if (iRef<derivedColl_.size()) return derivedColl_[iRef];
      else {
        std::cout<<"ERROR - Variables::ObjectVectorContainer::operator(size_t): Object index out of range "
                 <<"iRef="<<iRef<<" >= "<<derivedColl_.size()<<"(collection size)"<<std::endl;
        std::exit(1);
      }
    };
    TLorentzVector&    v4   (size_t& iRef) { 
      if (iRef<v4_.size()) return v4_[iRef];
      else {
        std::cout<<"ERROR - Variables::ObjectVectorContainer::v4(size_t): Object index out of range "
                 <<"iRef="<<iRef<<" >= "<<v4_.size()<<"(collection size)"<<std::endl;
        std::exit(1);
      }
    };
    DerivedType& operator() (int     iRef) { 
      if (iRef<derivedColl_.size()&&iRef>=0) return derivedColl_[iRef];
      else {
        if (iRef<0) std::cout<<"ERROR - Variables::ObjectVectorContainer::operator(int): Object index negative "
                             <<"iRef="<<iRef<<". Please use a non-negative number!"<<std::endl;

        else std::cout<<"ERROR - Variables::ObjectVectorContainer::operator(int): Object index out of range "
                      <<"iRef="<<iRef<<" >= "<<derivedColl_.size()<<"(collection size)"<<std::endl;
        std::exit(1);
      }
    };
    TLorentzVector&    v4   (int     iRef) { 
      if (iRef<v4_.size()&&iRef>=0) return v4_[iRef];
      else {
        if (iRef<0) std::cout<<"ERROR - Variables::ObjectVectorContainer::v4(int): Object index negative "
                             <<"iRef="<<iRef<<". Please use a non-negative number!"<<std::endl;
        
        else std::cout<<"ERROR - Variables::ObjectVectorContainer::v4(int): Object index out of range "
                      <<"iRef="<<iRef<<" >= "<<v4_.size()<<"(collection size)"<<std::endl;
        std::exit(1);
      }
    };
    size_t       get_default_buffer_size() { return default_buffer_size_; }

  };
  
  // Class to store multiple selected objects
  template<typename BaseType, typename DerivedType> class Object {
  public:
    Object(ObjectVectorContainer<BaseType, DerivedType>* x) : 
      parent_(x) {
      init(x->get_default_buffer_size());
    };
    Object(ObjectVectorContainer<BaseType, DerivedType>* x, std::function<bool()> s) : 
      parent_(x), selector_(s) {
      init(x->get_default_buffer_size());
    };
    ~Object() {};

    std::vector<bool> pass;            // tells if the object passes the selection
    std::vector<size_t> viSel;         // indices of the selected objects
    std::vector<size_t> viRef;         // indices of the reference objects (in the whole collection)

    size_t  i;         // index of the selected object
    size_t  iRef;      // index of the reference object
    size_t  n;         // number of selected objects

    // Initialize buffers
    void init(size_t nreserve) {
      pass .reserve(nreserve);
      viSel.reserve(nreserve);
      viRef.reserve(nreserve);
      reset();
    }
    void reset() {
      pass.assign(parent_->n, 0);
      viSel.assign(parent_->n, (size_t)-1);
      viRef.clear();
      i = -1;
      n = 0;
    }

    bool auto_define() { return define(selector_()); }
    
    bool define(bool passObj) {
      if ((pass[parent_->i] = passObj)) {
        viSel[parent_->i] = n++;
        viRef.emplace_back(parent_->i);
      }
      return passObj;
    }

    bool Loop() {
      if (++i<n) { 
        iRef    = viRef[i];
        return 1;
      } else {
        i=-1;
        return 0;
      }
    }

    const DerivedType& operator() ()             { return parent_->operator()(viRef[i]);    };
    const TLorentzVector&    v4   ()             { return parent_->v4(viRef[i]);            };
    const DerivedType& operator() (size_t& iRef) { return parent_->operator()(viRef[iRef]); };
    const TLorentzVector&    v4   (size_t& iRef) { return parent_->v4(viRef[iRef]);         };
    const DerivedType& operator() (int     iRef) { return parent_->operator()(viRef[iRef]); };
    const TLorentzVector&    v4   (int     iRef) { return parent_->v4(viRef[iRef]);         };

  private:
    ObjectVectorContainer<BaseType, DerivedType>* parent_;
    std::function<bool()> selector_;
  };

  //_______________________________________________________
  //                      Electrons

  struct ElectronData : eventBuffer::Electron_s {
    ElectronData(eventBuffer::Electron_s&& arg) : 
      eventBuffer::Electron_s(std::move(arg)) {}
  
    // Matched AK4 jet
    size_t iMatchedAK4    = -1;
    double jetDR          =  NOVAL_F;
    double jetDPhi        =  NOVAL_F;
    double jetDRmin       =  NOVAL_F;
    double cleanJetPtrel  =  NOVAL_F;
    bool   isPartOfJet    =  0;
    // Associated neutrino
    TLorentzVector nu     {0,0,0,0};
    double nuDR           =  NOVAL_F;
    // Matched AK8 jet
    bool   matchAK8       =  0;
    size_t iMatchedAK8    = -1;
    double ak8JetDR       =  NOVAL_F;
    double ak8JetDPhi     =  NOVAL_F;
    double nuMatchedAK8DR =  NOVAL_F;
    // GenTruth
    bool matchGenEle                = 0;
    bool matchGenEleFromHardProcess = 0;
    bool matchGenEleFromW           = 0;
    bool matchGenEleFromZ           = 0;
    bool matchGenEleFromH           = 0;
    bool matchGenEleFromTop         = 0;
    bool nuMatchGenEleNu            = 0;
    bool nuMatchGenEleNuFromW       = 0;
    bool nuMatchGenEleNuFromTop     = 0;
  };

  class ElectronSelection : 
    public ObjectVectorContainer<eventBuffer::Electron_s, ElectronData> {
    
  public:
    ElectronSelection(eventBuffer& d) :
      ObjectVectorContainer<eventBuffer::Electron_s, ElectronData>
    (d.Electron, d.Electron_pt.size()) { init(); };
    ~ElectronSelection() {};
    typedef Object<eventBuffer::Electron_s, ElectronData> Electron_c;
    
    Electron_c Veto      {this};
    Electron_c VetoNoIso {this};
    Electron_c Select    {this};
    Electron_c Tight     {this};
    Electron_c NoIso     {this};
    Electron_c NonIso    {this};
    
    void initObjects() {
      moveData();
      Veto     .reset();
      VetoNoIso.reset();
      Select   .reset();
      Tight    .reset();
      NoIso    .reset();
      NonIso   .reset();
    }

  } Electron;

  //_______________________________________________________
  //                        Muons

  struct MuonData : eventBuffer::Muon_s {
    MuonData(eventBuffer::Muon_s&& arg) : 
      eventBuffer::Muon_s(std::move(arg)) {}
  
    // Matched AK4 jet
    size_t iMatchedAK4    = -1;
    double jetDR          =  NOVAL_F;
    double jetDPhi        =  NOVAL_F;
    double jetDRmin       =  NOVAL_F;
    double cleanJetPtrel  =  NOVAL_F;
    bool   isPartOfJet    =  0;
    // Associated neutrino
    TLorentzVector nu     {0,0,0,0};
    double nuDR           =  NOVAL_F;
    // Matched AK8 jet
    bool   matchAK8       =  0;
    size_t iMatchedAK8    = -1;
    double ak8JetDR       =  NOVAL_F;
    double ak8JetDPhi     =  NOVAL_F;
    double nuMatchedAK8DR =  NOVAL_F;
    // GenTruth
    bool matchGenMu                = 0;
    bool matchGenMuFromHardProcess = 0;
    bool matchGenMuFromW           = 0;
    bool matchGenMuFromZ           = 0;
    bool matchGenMuFromH           = 0;
    bool matchGenMuFromTop         = 0;
    bool nuMatchGenMuNu            = 0;
    bool nuMatchGenMuNuFromW       = 0;
    bool nuMatchGenMuNuFromTop     = 0;
  };

  class MuonSelection : public ObjectVectorContainer
  <eventBuffer::Muon_s, MuonData> {
    
  public:
    MuonSelection(eventBuffer& d) :
      ObjectVectorContainer<eventBuffer::Muon_s, MuonData>
    (d.Muon, d.Muon_pt.size()) { init(); };
    ~MuonSelection() {};
    typedef Object<eventBuffer::Muon_s, MuonData> Muon_c;
    
    Muon_c Veto      {this};
    Muon_c VetoNoIso {this};
    Muon_c Select    {this};
    Muon_c Tight     {this};
    Muon_c NoIso     {this};
    Muon_c NonIso    {this};

    void initObjects() {
      moveData();
      Veto     .reset();
      VetoNoIso.reset();
      Select   .reset();
      Tight    .reset();
      NoIso    .reset();
      NonIso   .reset();
    }
  } Muon;

  //_______________________________________________________
  //                        Taus

  struct TauData : eventBuffer::Tau_s {
    TauData(eventBuffer::Tau_s&& arg) : 
      eventBuffer::Tau_s(std::move(arg)) {}
    // GenTruth
    bool matchGenTau = 0;
  };

  class TauSelection : public ObjectVectorContainer
  <eventBuffer::Tau_s, TauData> {
    
  public:
    TauSelection(eventBuffer& d) :
      ObjectVectorContainer<eventBuffer::Tau_s, TauData>
    (d.Tau, d.Tau_pt.size()) { init(); };
    ~TauSelection() {};
    typedef Object<eventBuffer::Tau_s, TauData> Tau_c;
    
    Tau_c Veto{this};
    
    void initObjects() {
      moveData();
      Veto     .reset();
    }
  } Tau;

  //_______________________________________________________
  //                       Photons

  struct PhotonData : eventBuffer::Photon_s {
    PhotonData(eventBuffer::Photon_s&& arg) : 
      eventBuffer::Photon_s(std::move(arg)) {}
    // GenTruth
    bool fromFrag             = 0;
    bool matchGenFake         = 1;
    bool matchGenPrompt       = 0;
    bool matchGenPromptDirect = 0;
    bool matchGenPromptFrag   = 0;
  };

  class PhotonSelection : public ObjectVectorContainer
  <eventBuffer::Photon_s, PhotonData> {
    
  public:
    PhotonSelection(eventBuffer& d) :
      ObjectVectorContainer<eventBuffer::Photon_s, PhotonData>
    (d.Photon, d.Photon_pt.size()) { init(); };
    ~PhotonSelection() {};
    typedef Object<eventBuffer::Photon_s, PhotonData> Photon_c;
    
    Photon_c PreSelect {this};
    Photon_c Select    {this};
    Photon_c Fake      {this};

    void initObjects() {
      moveData();
      PreSelect.reset();
      Select   .reset();
      Fake     .reset();
    }
  } Photon;


  //_______________________________________________________
  //                        Jets 

  struct JetData : eventBuffer::Jet_s {
    JetData(eventBuffer::Jet_s&& arg) : 
      eventBuffer::Jet_s(std::move(arg)) {}
  
    double eleDR       =  NOVAL_F;
    double elePtRatio  =  0;
    double muDR        =  NOVAL_F;
    double muPtRatio   =  0;
    double phoDR       =  NOVAL_F;
    double phoPtRatio  =  0;
    // GenTruth
  };

  class JetSelection : public ObjectVectorContainer
  <eventBuffer::Jet_s, JetData> {
    
  public:
    JetSelection(eventBuffer& d) :
      ObjectVectorContainer<eventBuffer::Jet_s, JetData>
    (d.Jet, d.Jet_pt.size()) { init(); };
    ~JetSelection() {};
    typedef Object<eventBuffer::Jet_s, JetData> Jet_c;
    
    Jet_c Jet            {this};
    Jet_c JetLepNoIso    {this};
    Jet_c JetLepNonIso   {this};
    Jet_c JetNoLep       {this};
    Jet_c JetNoPho       {this};
    Jet_c LooseBTag      {this};
    Jet_c MediumBTag     {this};
    Jet_c TightBTag      {this};
    Jet_c MediumBTagNoPho{this};
    Jet_c LooseIsoBTag   {this};
    Jet_c MediumIsoBTag  {this};

    void initObjects() {
      moveData();
      Jet          .reset();
      JetLepNoIso  .reset();
      JetLepNonIso .reset();
      JetNoLep     .reset();
      JetNoPho     .reset();
      LooseBTag    .reset();
      MediumBTag   .reset();
      TightBTag    .reset();
      LooseIsoBTag .reset();
      MediumIsoBTag.reset();
    }
  } Jet;

  //_______________________________________________________
  //                  FatJets and SubJets

  struct SubJetData : eventBuffer::SubJet_s {
    SubJetData(eventBuffer::SubJet_s&& arg) : 
      eventBuffer::SubJet_s(std::move(arg)) {}
    // GenTruth
  };

  class SubJetSelection : public ObjectVectorContainer
  <eventBuffer::SubJet_s, SubJetData> {
    
  public:
    SubJetSelection(eventBuffer& d) :
      ObjectVectorContainer<eventBuffer::SubJet_s, SubJetData>
    (d.SubJet, d.SubJet_pt.size()) { init(); };
    ~SubJetSelection() {};
    typedef Object<eventBuffer::SubJet_s, SubJetData> SubJet_c;
    
    void initObjects() {
      moveData();
    }
  } SubJet;

  struct FatJetData : eventBuffer::FatJet_s {
    FatJetData(eventBuffer::FatJet_s&& arg) : 
      eventBuffer::FatJet_s(std::move(arg)) {}
  
    bool   passSubJetBTag =  0;
    size_t nSubJet        =  0;
    size_t nSubJetBTag    =  0;
    double maxSubJetDeepB = -NOVAL_F;
    double tau21          =  NOVAL_F;
    double tau31          =  NOVAL_F;
    double tau32          =  NOVAL_F;
    double eleDR          =  NOVAL_F;
    double elePtRatio     =  0;
    double muDR           =  NOVAL_F;
    double muPtRatio      =  0;
    double phoDR          =  NOVAL_F;
    double phoPtRatio     =  0;
    bool   matchLepNoIso  =  0;
    bool   matchLepNonIso =  0;
    double lepNonIsoNuDR  =  NOVAL_F;
    double LSF            = -NOVAL_F;
    double LSF_NoIso      = -NOVAL_F;
    double matchedNoIsoLepJetDRmin      = -NOVAL_F;
    double matchedNoIsoLepCleanJetPtrel = -NOVAL_F;
    // GenTruth
    bool matchGenHadW     =  0;
    bool matchGenHadZ     =  0;
    bool matchGenHadH     =  0;
    bool matchGenHadTop   =  0;
    bool matchGenLepTop   =  0;
    bool matchGenLepton   =  0;
    int  matchedGenLeptonMotherID = -NOVAL_F;
  };

  class FatJetSelection : public ObjectVectorContainer
  <eventBuffer::FatJet_s, FatJetData> {
    
  public:
    FatJetSelection(eventBuffer& d) :
      ObjectVectorContainer<eventBuffer::FatJet_s, FatJetData>
    (d.FatJet, d.FatJet_pt.size()) { init(); };
    ~FatJetSelection() {};
    typedef Object<eventBuffer::FatJet_s, FatJetData> FatJet_c;
    
    FatJet_c JetAK8               {this};
    FatJet_c JetAK8Mass           {this};
    // All old and new WPs
    FatJet_c W1                   {this};
    FatJet_c W2                   {this};
    FatJet_c W3                   {this};
    FatJet_c Top1                 {this};
    FatJet_c Top2                 {this};
    FatJet_c Top3                 {this};
    FatJet_c Top4                 {this};
    FatJet_c Top5                 {this};
    FatJet_c WDeepMD1             {this};
    FatJet_c WDeepMD2             {this};
    FatJet_c WDeepMD3             {this};
    FatJet_c WDeep1               {this};
    FatJet_c WDeep2               {this};
    FatJet_c WDeep3               {this};
    FatJet_c ZDeepMD1             {this};
    FatJet_c ZDeepMD2             {this};
    FatJet_c ZDeepMD3             {this};
    FatJet_c ZDeep1               {this};
    FatJet_c ZDeep2               {this};
    FatJet_c ZDeep3               {this};
    FatJet_c VDeep1               {this};
    FatJet_c VDeep2               {this};
    FatJet_c VDeep3               {this};
    FatJet_c HDeepMD1             {this};
    FatJet_c HDeepMD2             {this};
    FatJet_c HDeepMD3             {this};
    FatJet_c HDeep1               {this};
    FatJet_c HDeep2               {this};
    FatJet_c HDeep3               {this};
    FatJet_c TopDeepMD1           {this};
    FatJet_c TopDeepMD2           {this};
    FatJet_c TopDeepMD3           {this};
    FatJet_c TopDeepMD4           {this};
    FatJet_c TopDeep1             {this};
    FatJet_c TopDeep2             {this};
    FatJet_c TopDeep3             {this};
    FatJet_c TopDeep4             {this};
    // New definitions
    FatJet_c HadW                 {this};
    FatJet_c HadZ                 {this};
    FatJet_c HadV                 {this};
    FatJet_c HadH                 {this};
    FatJet_c HadTop               {this};
    FatJet_c LepJetCand           {this};
    FatJet_c HadW_MD              {this};
    FatJet_c HadZ_MD              {this};
    FatJet_c HadTop_MD            {this};
    FatJet_c LepJet               {this};
    FatJet_c LepJetNoIso          {this};
    FatJet_c LepJetNoPt           {this};
    FatJet_c LepTop               {this};
    FatJet_c LepJetHighMass       {this};
    FatJet_c LepTopHighMass       {this};
    FatJet_c LepTopNoIso          {this};
    FatJet_c LepTopNoPt           {this};
    FatJet_c LepTopNoMass         {this};
    FatJet_c LepTopNoSubJetB      {this};

    void initObjects() {
      moveData();
      JetAK8              .reset();
      JetAK8Mass          .reset();
      W1                  .reset();
      W2                  .reset();
      W3                  .reset();
      Top1                .reset();
      Top2                .reset();
      Top3                .reset();
      Top4                .reset();
      Top5                .reset();
      WDeepMD1            .reset();
      WDeepMD2            .reset();
      WDeepMD3            .reset();
      WDeep1              .reset();
      WDeep2              .reset();
      WDeep3              .reset();
      ZDeepMD1            .reset();
      ZDeepMD2            .reset();
      ZDeepMD3            .reset();
      ZDeep1              .reset();
      ZDeep2              .reset();
      ZDeep3              .reset();
      VDeep1              .reset();
      VDeep2              .reset();
      VDeep3              .reset();
      HDeepMD1            .reset();
      HDeepMD2            .reset();
      HDeepMD3            .reset();
      HDeep1              .reset();
      HDeep2              .reset();
      HDeep3              .reset();
      TopDeepMD1          .reset();
      TopDeepMD2          .reset();
      TopDeepMD3          .reset();
      TopDeepMD4          .reset();
      TopDeep1            .reset();
      TopDeep2            .reset();
      TopDeep3            .reset();
      TopDeep4            .reset();
      HadW                .reset();
      HadZ                .reset();
      HadV                .reset();
      HadH                .reset();
      HadTop              .reset();
      LepJetCand          .reset();
      HadW_MD             .reset();
      HadZ_MD             .reset();
      HadTop_MD           .reset();
      LepJet              .reset();
      LepJetNoIso         .reset();
      LepJetNoPt          .reset();
      LepTop              .reset();
      LepJetHighMass      .reset();
      LepTopHighMass      .reset();
      LepTopNoIso         .reset();
      LepTopNoPt          .reset();
      LepTopNoMass        .reset();
      LepTopNoSubJetB     .reset();
    }
  } FatJet;
  
  //_______________________________________________________
  //                     Gen Particles

  struct GenPartData : eventBuffer::GenPart_s {
    GenPartData(eventBuffer::GenPart_s&& arg) : 
      eventBuffer::GenPart_s(std::move(arg)) {}
  
    bool LastCopyCand   = 0;
    bool NoSameDaughter = 1;
    bool FinalState     = 0;
    // For Object efficiency - N.B: Not for Mistag!
    bool passLepVeto       = 0;
    bool passLepNoIso      = 0;
    bool passLepNonIso     = 0;
    bool passHadWTag       = 0;
    bool passHadZTag       = 0;
    bool passHadHTag       = 0;
    bool passHadTopTag     = 0;
    bool passLepTopTag     = 0;
    // Pointers to matched object indices
    bool   matchAK8                  =  0;
    size_t iMatchedAK8               = -1;
    size_t iMother                   = -1; // eg. (W->nu e)
    size_t iGrandMother              = -1; // eg. (t->b (W->nu mu))
    size_t iGreatGrandMother         = -1; // eg. (t->b (W->nu (tau->nu e)))
    size_t iGenLepDaughter           = -1;
    size_t iGenLepGrandDaughter      = -1;
    size_t iGenLepGreatGrandDaughter = -1;
    //size_t iGenHadWMatchedAK8; --> replace with above
    //size_t iGenLepWMatchedGenLep;
    //size_t iGenLepTopMatchedGenLep;
    // Ancestor ID
    int motherId           = -NOVAL_I;
    int grandMotherId      = -NOVAL_I;
    int greatGrandMotherId = -NOVAL_I;
  };

  class GenPartSelection : public ObjectVectorContainer
  <eventBuffer::GenPart_s, GenPartData> {
    
  public:
    GenPartSelection(eventBuffer& d) :
      ObjectVectorContainer<eventBuffer::GenPart_s, GenPartData>
    (d.GenPart, d.GenPart_pt.size()) { init(); };
    ~GenPartSelection() {};
    typedef Object<eventBuffer::GenPart_s, GenPartData> GenPart_c;
    
    GenPart_c Lepton                {this};
    GenPart_c Ele                   {this};
    GenPart_c Mu                    {this};
    GenPart_c Tau                   {this};
    GenPart_c LeptonFromHardProcess {this};
    GenPart_c EleFromHardProcess    {this};
    GenPart_c MuFromHardProcess     {this};
    GenPart_c TauFromHardProcess    {this};
    GenPart_c LeptonFromW           {this};
    GenPart_c EleFromW              {this};
    GenPart_c MuFromW               {this};
    GenPart_c TauFromW              {this};
    GenPart_c LeptonFromZ           {this};
    GenPart_c EleFromZ              {this};
    GenPart_c MuFromZ               {this};
    GenPart_c TauFromZ              {this};
    GenPart_c LeptonFromH           {this};
    GenPart_c EleFromH              {this};
    GenPart_c MuFromH               {this};
    GenPart_c TauFromH              {this};
    GenPart_c LeptonFromTop         {this};
    GenPart_c EleFromTop            {this};
    GenPart_c MuFromTop             {this};
    GenPart_c TauFromTop            {this};
    GenPart_c Nu                    {this};
    GenPart_c EleNu                 {this};
    GenPart_c MuNu                  {this};
    GenPart_c TauNu                 {this};
    GenPart_c NuFromW               {this};
    GenPart_c EleNuFromW            {this};
    GenPart_c MuNuFromW             {this};
    GenPart_c TauNuFromW            {this};
    GenPart_c NuFromTop             {this};
    GenPart_c EleNuFromTop          {this};
    GenPart_c MuNuFromTop           {this};
    GenPart_c TauNuFromTop          {this};
    GenPart_c PromptPhoton          {this};
    GenPart_c PromptDirectPhoton    {this};
    GenPart_c PromptFragPhoton      {this};
    GenPart_c b                     {this};
    GenPart_c W                     {this};
    GenPart_c LepW                  {this};
    GenPart_c HadW                  {this};
    GenPart_c Z                     {this};
    GenPart_c LepZ                  {this};
    GenPart_c HadZ                  {this};
    GenPart_c H                     {this};
    GenPart_c LepH                  {this};
    GenPart_c HadH                  {this};
    GenPart_c Top                   {this};
    GenPart_c LepTop                {this};
    GenPart_c HadTop                {this};
    
    void initObjects() {
      moveData();
      Lepton               .reset();
      Ele                  .reset();
      Mu                   .reset();
      Tau                  .reset();
      LeptonFromHardProcess.reset();
      EleFromHardProcess   .reset();
      MuFromHardProcess    .reset();
      TauFromHardProcess   .reset();
      LeptonFromW          .reset();
      EleFromW             .reset();
      MuFromW              .reset();
      TauFromW             .reset();
      LeptonFromZ          .reset();
      EleFromZ             .reset();
      MuFromZ              .reset();
      TauFromZ             .reset();
      LeptonFromH          .reset();
      EleFromH             .reset();
      MuFromH              .reset();
      TauFromH             .reset();
      LeptonFromTop        .reset();
      EleFromTop           .reset();
      MuFromTop            .reset();
      TauFromTop           .reset();
      Nu                   .reset();
      EleNu                .reset();
      MuNu                 .reset();
      TauNu                .reset();
      NuFromW              .reset();
      EleNuFromW           .reset();
      MuNuFromW            .reset();
      TauNuFromW           .reset();
      NuFromTop            .reset();
      EleNuFromTop         .reset();
      MuNuFromTop          .reset();
      TauNuFromTop         .reset();
      PromptPhoton         .reset();
      PromptDirectPhoton   .reset();
      PromptFragPhoton     .reset();
      b                    .reset();
      W                    .reset();
      LepW                 .reset();
      HadW                 .reset();
      Z                    .reset();
      LepZ                 .reset();
      HadZ                 .reset();
      H                    .reset();
      LepH                 .reset();
      HadH                 .reset();
      Top                  .reset();
      LepTop               .reset();
      HadTop               .reset();
    }
  } GenPart;

  int recalc_jets     = 0;
  int recalc_megajets = 0;
  int recalc_met      = 0;

  // New methods instead of calculate_common_variables
  void get_signal_mass() {
    susy_mass.assign(3, -NOVAL_F);
    if (isSignal) while (GenPart.Loop()) {
      if (signal_index==0 || signal_index==4) { if(std::abs(GenPart().pdgId) == 1000021) susy_mass[0] = GenPart().mass; } // gluino
      else if (signal_index==1)               { if(std::abs(GenPart().pdgId) == 1000006) susy_mass[0] = GenPart().mass; } // stop
      else if (signal_index==2)               { if(std::abs(GenPart().pdgId) == 1000024) susy_mass[0] = GenPart().mass; } // chargino
      else if (signal_index==3)               { if(std::abs(GenPart().pdgId) == 1000023) susy_mass[0] = GenPart().mass; } // neutralino2 (chi^0_2)
      else if (signal_index==5)               { if(std::abs(GenPart().pdgId) == 1000005) susy_mass[0] = GenPart().mass; } // sb
      if (signal_index==4)                    { if(std::abs(GenPart().pdgId) == 1000023) susy_mass[1] = GenPart().mass; } // neutralino2
      else                                    { if(std::abs(GenPart().pdgId) == 1000022) susy_mass[1] = GenPart().mass; } // LSP neutralino (chi^0_1)
      if (signal_index==5)                    { if(std::abs(GenPart().pdgId) == 1000023) susy_mass[2] = GenPart().mass; } // neutralino2
    }
    susy_mass[0] = int(std::round(susy_mass[0]/5)*5);
    susy_mass[1] = int(std::round(susy_mass[1]/25)*25);
    susy_mass[2] = int(std::round(susy_mass[2]/25)*25);
  }

  void define_lepton_and_photon_variables() {
    // Lepton selections do not depend on any systematics
    // --> event weights are assigned instead (for SF, lost lepton etc)
    define_leptons_and_photons_();
  }

  void define_jet_variables() {
    // Vairables need to be recalculated each time the jet energy is changed
    // eg. Jet selection, W/top tags etc. that depends on jet pt
    define_jets_();
  }
  
  void define_genparticle_variables() {
    define_genparticles_();
  }
  
  void define_event_variables(const unsigned int& syst_index) {
    define_event_(syst_index);
  }

  //_______________________________________________________
  //              Rescale jet 4-momenta

  /*
  // Variables to save the original values before applying any systematics on them
  std::vector<float> AK4_E, AK4_Pt;
  std::vector<float> AK8_E, AK8_Pt, AK8_softDropMass;//, AK8_trimmedMass, AK8_prunedMass, AK8_filteredMass;
  std::vector<float> AK8_softDropMass0;
  std::vector<float> AK8_softDropMass1;
  std::vector<float> AK8_softDropMass2;
  std::vector<float> AK8_softDropMassCorr; // Correction for W tagging
  std::vector<float> softDropMassCorr;     // POG Correction + uncertainties for W tagging
  std::vector<float> softDropMass;         // JES Correction + uncertainties for top tagging
  
  std::vector<float> AK4_JERSmearFactor,     AK8_JERSmearFactor;
  std::vector<float> AK4_JERSmearFactorUp,   AK8_JERSmearFactorUp;
  std::vector<float> AK4_JERSmearFactorDown, AK8_JERSmearFactorDown;
  std::vector<float> AK8_JMR_random;
  
  TVector3 met;
  TVector3 dmet_JESUp,  dmet_JESDown;
  TVector3 dmet_JERUp,  dmet_JERDown;
  TVector3 dmet_RestUp, dmet_RestDown;
  */

  void rescale_smear_jet_met(const bool& applySmearing, const unsigned int& syst_index,
                             const double& nSigmaJES, const double& nSigmaJER, const double& nSigmaRestMET,
                             const bool& rescaleAK8, const double& nSigmaRescaleAK8)
  {
    /*
    // Apply Jet Energy Scale (JES) and Jet Energy Resolution (JER) corrections
    // For AK8 jets which are used for W tagging (only):
    // - Additionally or instead apply jet mass scale (JMS) and jet mass resolutin (JMR) corrections
  
    // Initialization (needed for later variations
    if (syst_index==0) {
      // Save the original values for later (before applying any systematics)
      AK4_E            = data.jetsAK4.E;
      AK4_Pt           = data.jetsAK4.Pt;
      AK8_E            = data.jetsAK8.E;
      AK8_Pt           = data.jetsAK8.Pt;
      //AK8_softDropMass = data.jetsAK8.softDropMassPuppi; 
      AK8_softDropMass = data.jetsAK8.corrSDMassPuppi; // Make sure to change also sd_mass_top if needed!!!
      AK8_softDropMass0 = data.jetsAK8.uncorrSDMassPuppi; // No correction (needed for W)
      AK8_softDropMass1 = data.jetsAK8.corrSDMassPuppi;   // L1L2L3 on subjet
      AK8_softDropMass2 = data.jetsAK8.softDropMassPuppi; // L2L3 on fatjet
      //AK8_trimmedMass  = data.jetsAK8.trimmedMass;
      //AK8_prunedMass   = data.jetsAK8.prunedMass;
      //AK8_filteredMass = data.jetsAK8.filteredMass;
  
      // Correction for Puppi SoftDrop Mass
      // (Needed for W tagging)
      // https://twiki.cern.ch/twiki/bin/view/CMS/JetWtagging?rev=43#Working_points_and_scale_factors
      if (puppisd_corrGEN_==0) {
        AK8_softDropMassCorr = data.jetsAK8.uncorrSDMassPuppi;
      } else {
        AK8_softDropMassCorr.clear();
        for (size_t i=0; i<data.jetsAK8.size; ++i) {
  	double puppi_pt  = data.jetsAK8.PtPuppi[i];
  	double puppi_eta = data.jetsAK8.EtaPuppi[i];
  	double puppi_sd_mass_w = data.jetsAK8.uncorrSDMassPuppi[i];
  	double corr = puppisd_corrGEN_->Eval(puppi_pt);
  	if(std::abs(puppi_eta)<=1.3) corr *= puppisd_corrRECO_cen_->Eval(puppi_pt);
  	else corr *= puppisd_corrRECO_for_->Eval(puppi_pt);
  	
  	AK8_softDropMassCorr.push_back(puppi_sd_mass_w * corr);
        }
      } // end sys_index==0
  
      // Calculate the JER/JMR smear factors
      if (applySmearing) {
        AK4_JERSmearFactor    .clear();
        AK4_JERSmearFactorUp  .clear();
        AK4_JERSmearFactorDown.clear();
        for (size_t i=0; i<data.jetsAK4.size; ++i) {
          double JERSmear     = data.jetsAK4.SmearedPt[i]/data.jetsAK4.Pt[i];
          double JERSmearUp   = 1 + (JERSmear-1) * (data.jetsAK4.JERSFUp[i]  -1) / (data.jetsAK4.JERSF[i]-1);
          double JERSmearDown = 1 + (JERSmear-1) * (data.jetsAK4.JERSFDown[i]-1) / (data.jetsAK4.JERSF[i]-1);
          AK4_JERSmearFactor    .push_back(JERSmear);
          AK4_JERSmearFactorUp  .push_back(JERSmearUp);
          AK4_JERSmearFactorDown.push_back(JERSmearDown);
        }
        AK8_JERSmearFactor    .clear();
        AK8_JERSmearFactorUp  .clear();
        AK8_JERSmearFactorDown.clear();
        AK8_JMR_random.clear();
        for (size_t i=0; i<data.jetsAK8.size; ++i) {
          double JERSmear     = data.jetsAK8.SmearedPt[i]/data.jetsAK8.Pt[i];
          double JERSmearUp   = 1 + (JERSmear-1) * (data.jetsAK8.JERSFUp[i]  -1) / (data.jetsAK8.JERSF[i]-1);
          double JERSmearDown = 1 + (JERSmear-1) * (data.jetsAK8.JERSFDown[i]-1) / (data.jetsAK8.JERSF[i]-1);
          AK8_JERSmearFactor    .push_back(JERSmear);
          AK8_JERSmearFactorUp  .push_back(JERSmearUp);
          AK8_JERSmearFactorDown.push_back(JERSmearDown);
  	// Apply random gaussian smearing to worsen mass resolution (It cannot improve with this method)
  	// Recipe is the same as the stochastic smearing explained here:
  	// https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#Smearing_procedures
  	// Generate the random number once, and vary the systematics only on the SF
  	double sigma_rel = W_TAG_SIGMA_MC / (AK8_softDropMassCorr[i] * W_TAG_JMS_SF);
  	double random = rnd_.Gaus(0,sigma_rel);
  	AK8_JMR_random.push_back(random);
        }
      }
  
      //_________________________________________________
      //                      MET
  
      // Save the original MET
      met.SetPtEtaPhi(data.met.Pt[0], 0, data.met.Phi[0]);
  
      // MET Uncertainties
      //  Met uncertainty vector indices:
      //  enum METUncertainty {
      //  JetResUp =0, JetResDown =1,
      //  JetEnUp =2, JetEnDown =3,
      //  MuonEnUp =4, MuonEnDown =5,
      //  ElectronEnUp =6, ElectronEnDown =7,
      //  TauEnUp =8, TauEnDown =9,
      //  UnclusteredEnUp =10, UnclusteredEnDown =11,
      //  PhotonEnUp =12, PhotonEnDown =13,
      //  }
      float maxdpt_up = 0, maxdpt_down = 0;
      float dphi_up   = 0, dphi_down   = 0;
      float ptsum_up  = 0, ptsum_down  = 0;
      // Consider JES/JER modulation separately
      // Add the rest of the systematic pt modulations in quadrature
      // Use the phi direction of the largest remaining systematic
      for (size_t i=0; i<data.syst_met.size; ++i) {
        TVector3 met_syst;
        met_syst.SetPtEtaPhi(data.syst_met.Pt[i], 0, data.syst_met.Phi[i]);
        TVector3 dmet;
        dmet = met_syst - met;
        if (i==0) {
  	dmet_JERUp = dmet;
        } else if (i==1) {
  	dmet_JERDown = dmet;
        } else if (i==2) {
  	dmet_JESUp = dmet;
        } else if (i==3) {
  	dmet_JESDown = dmet;
        } else if (i%2==0) {
  	// Rest Up
  	if (dmet.Pt()>maxdpt_up) {
  	  maxdpt_up = dmet.Pt();
  	  dphi_up   = dmet.Phi();
  	  ptsum_up  = std::sqrt(ptsum_up*ptsum_up + dmet.Perp2());
  	}
        } else {
  	// Rest Down
  	if (dmet.Pt()>maxdpt_down) {
  	  maxdpt_down = dmet.Pt();
  	  dphi_down   = dmet.Phi();
  	  ptsum_down  = std::sqrt(ptsum_down*ptsum_down + dmet.Perp2());
  	}
        }
      }
      dmet_RestUp.  SetPtEtaPhi(ptsum_up,   0, dphi_up);
      dmet_RestDown.SetPtEtaPhi(ptsum_down, 0, dphi_down);
    }
  
  
    // Apply systematic variations
    // Even if Sigmas=0, we still smear jets!
    // AK4 jets
    while(data.jetsAK4.Loop()) {
      size_t i = data.jetsAK4.it;
      double scaleJES = get_syst_weight(1.0, data.jetsAK4.jecUncertainty[i], nSigmaJES);
      data.jetsAK4.Pt[i] = AK4_Pt[i] * scaleJES;
      data.jetsAK4.E[i]  = AK4_E[i]  * scaleJES;
      if (applySmearing) {
        double scaleJER = get_syst_weight(AK4_JERSmearFactor[i], AK4_JERSmearFactorUp[i], AK4_JERSmearFactorDown[i], nSigmaJER);
        data.jetsAK4.Pt[i] *= scaleJER;
        data.jetsAK4.E[i]  *= scaleJER;
      }
    }
    // AK8 jets
    softDropMassCorr.clear();
    softDropMass    .clear();
    while(data.jetsAK8.Loop()) {
      size_t i = data.jetsAK8.it;
      double scaleJES = get_syst_weight(1.0, data.jetsAK8.jecUncertainty[i], nSigmaJES);
      data.jetsAK8.Pt[i] = AK8_Pt[i] * scaleJES;
      data.jetsAK8.E[i]  = AK8_E[i]  * scaleJES;
      double scaleJER = 1;
      if (applySmearing) {
        scaleJER = get_syst_weight(AK8_JERSmearFactor[i], AK8_JERSmearFactorUp[i], AK8_JERSmearFactorDown[i], nSigmaJER);
        data.jetsAK8.Pt[i] *= scaleJER;
        data.jetsAK8.E[i]  *= scaleJER;
      }
  
      double rescale_v4 = 1.0;
      if (rescaleAK8) {
        // Apply AK8 jet rescaling to match data distribution for Z/G/DY MCs
        // Get the scale/offset from: scripts/calc_pt_scaling.C
        // Rescale the whole 4-momentum, because the Z-mass peak (in AK8 jet) also looks shifted
        //double scale = 0.867 + 0.039*nSigmaRescaleAK8, offset = 33.2 + 36.4 *nSigmaRescaleAK8;
        //double scale = 0.840 + 0.037*nSigmaRescaleAK8, offset = 50.5 + 35.0 *nSigmaRescaleAK8;
        double scale = 0.848 + 0.047*nSigmaRescaleAK8, offset = 39.6 + 38.7 *nSigmaRescaleAK8;
        // Use original pt without any scale/smear to determine the right scaling
        double orig_pt = AK8_Pt[i];
        if (applySmearing) orig_pt *= AK8_JERSmearFactor[i];
        rescale_v4 = scale + offset/orig_pt;
        data.jetsAK8.Pt[i] *= rescale_v4;
        data.jetsAK8.E[i]  *= rescale_v4;
      }
      //AK8_Ht += data.jetsAK8.Pt[i];
  
      // For W jet mass apply combination of both JES+JMS and JMR (JER not needed)
      // JES uncertainty is added on top of the JMS one (in quadrature)
      double comb_unc = std::sqrt(W_TAG_JMS_SF_ERR*W_TAG_JMS_SF_ERR + data.jetsAK8.jecUncertainty[i]*data.jetsAK8.jecUncertainty[i]);
      double scaleJMS = get_syst_weight(W_TAG_JMS_SF, comb_unc, nSigmaJES);
      //double scaled_corrected_mass = AK8_softDropMassCorr[i] * scaleJMS * rescale_v4;
      double scaled_corrected_mass = AK8_softDropMassCorr[i] * scaleJMS;
      // Apply random gaussian smearing to worsen mass resolution (It cannot improve with this method)
      if (applySmearing) {
        double scale_factor = get_syst_weight(W_TAG_JMR_SF, W_TAG_JMR_SF_ERR, nSigmaJER);
        if (scale_factor > 1) {
  	double scaleJMR = 1 + AK8_JMR_random[i] * std::sqrt( scale_factor*scale_factor - 1 );
  	scaled_corrected_mass *= scaleJMR;
        }
      }
      softDropMassCorr.push_back(scaled_corrected_mass);
  
      // For Top jet mass, apply only JES + JER for now
      // (Since there's no other recommendation)
      //double scaled_top_mass = AK8_softDropMass[i] * scaleJES * rescale_v4;
      double scaled_top_mass = AK8_softDropMass[i] * scaleJES;
      if (applySmearing) scaled_top_mass *= scaleJER;
      softDropMass.push_back(scaled_top_mass);
  
    }
  
    TVector3 dmet(0,0,0);
    // MET Uncertainties
    if (!isSignal) {
      // Background
      if (nSigmaJES   >=0) dmet += std::abs(nSigmaJES) * dmet_JESUp;
      else                 dmet += std::abs(nSigmaJES) * dmet_JESDown;
      if (applySmearing) {
        if (nSigmaJER   >=0) dmet += std::abs(nSigmaJER) * dmet_JERUp;
        else                 dmet += std::abs(nSigmaJER) * dmet_JERDown;
      }
      if (nSigmaRestMET>=0) dmet += std::abs(nSigmaRestMET) * dmet_RestUp;
      else                  dmet += std::abs(nSigmaRestMET) * dmet_RestDown;
    } else {
      // Signal - Use only a one sided variation
      // Take the gen MET as the Up variation
      // and use the PF MET as the nominal/down variation -> i.e do nothing for Sigma < 0
      // --> In the end use the average of the acceptances as the nominal value
      // But do not use the average PF/gen met as the nominal value
      if (nSigmaRestMET>0) {
        TVector3 genmet;
        genmet.SetPtEtaPhi(data.met.genPt, 0, data.met.genPhi);
        dmet = genmet - met;
        dmet = std::abs(nSigmaRestMET) * dmet;
      }
    }
    TVector3 shifted_met = met + dmet;
    data.met.Pt[0]  = shifted_met.Pt();
    data.met.Phi[0] = shifted_met.Phi();
    */
  }

  
private:
  
  // Definitions of the internal methods defining variables and object selections

  void define_leptons_and_photons_(int debug = 0) {
    if (debug) std::cout<<"Variables::define_leptons_and_photons_: start (new event)"<<std::endl;

    // Initial loop on AK4 jets 
    // only needed for ele/muon 2D cut (which relies on the associated jet)
    Jet.initObjects();
    while (Jet.Loop()) {
      Jet.Jet.define( (Jet().jetId == 2 || Jet().jetId == 6) &&
                      Jet().pt            >= JET_AK4_PT_CUT &&
                      std::abs(Jet().eta)  < JET_AK4_ETA_CUT);
    }
    if (debug) std::cout<<"Variables::define_leptons_and_photons_: end AK4 jet definition"<<std::endl;
  
    // No need to loop on AK8 jets, but only to initialize the collection
    FatJet.initObjects();

    if (debug) std::cout<<"Variables::define_leptons_and_photons_: init AK8 jet collection"<<std::endl;

    // Electrons - full definitions
    Electron.initObjects();
    while (Electron.Loop()) {
      float pt      = Electron().pt;
      float abseta  = std::abs(Electron().eta);
      float absd0   = std::abs(Electron().dxy);
      float absdz   = std::abs(Electron().dz);
      float miniIso = Electron().miniPFRelIso_all;
      //float ipsig   = std::abs(Electron().sip3d);

      // Calculate the B2G 2D cut variables
      // [Lepton]_jetPtRelv2 is available in v5 NanoAOD, but it is bugged :(
      // Try to do what (should be) done in NanoAOD (revert JEC, subtract ele, reapply JEC) based on the original NanoAOD recipe:
      // https://github.com/cms-nanoAOD/cmssw/blob/master-cmsswmaster/PhysicsTools/NanoAOD/plugins/LeptonJetVarProducer.cc#L152-L162
      // 1) if no jetIdx, or no jet id --> match nearest jet passing the ID for DR, leave Ptrel = NOVAL_F
      // 2) if    jetIdx -> calculate DR
      // 3)              -> subtract lep pt from jet, check if passs pt cut (if not Ptrel=NOVAL_F), calculate Ptrel
      // Calculate dRmin for the associated/nearest jet
      int jetIdx = Electron().jetIdx;
      Electron().isPartOfJet = true;
      if (jetIdx==-1) Electron().isPartOfJet = false;
      else if (!Jet.Jet.pass[jetIdx]) Electron().isPartOfJet = false;
      if (debug>1) std::cout<<"Start calculating 2D cut for - iEle="<<Electron.i<<" isPartOfJet="<<Electron().isPartOfJet<<" matched jetIdx="<<jetIdx<<std::endl;
      if (Electron().isPartOfJet) {
        Electron().jetDRmin = Electron.v4().DeltaR(Jet.v4(jetIdx));
      } else while (Jet.Jet.Loop()) {
        double dR = Electron.v4().DeltaR(Jet.Jet.v4());
        if (dR<Electron().jetDRmin) {
          Electron().jetDRmin = dR;
          if (debug>1) std::cout<<" - matched to jetdIdx="<<Jet.Jet.iRef<<" dR="<<dR<<std::endl; 
        }
      }
      if (debug>1) std::cout<<"jetIdx="<<jetIdx<<std::endl;
      // Calculate pTrel for the associated/nearest jet
      // First subtract the ele from the jet
      // The JEC reverted ele energy subtraction should work in the mean time
      if (jetIdx!=-1) {
        TLorentzVector cleanjet_v4 = Jet.v4(jetIdx);
        if (debug>1) std::cout<<"-       jet pt="<<cleanjet_v4.Pt()<<std::endl;
        cleanjet_v4 -= Electron.v4()*(1/(1-Jet(jetIdx).rawFactor));
        // Then, for version 2 check also that the cleaned jet passes the minimum pt threshold cut that is considered in the analysis
        // If not, revert back to the previous closest jet
        if (debug>1) std::cout<<"- clean jet pt="<<cleanjet_v4.Pt()<<std::endl;
        if (cleanjet_v4.Pt() >= JET_AK4_PT_CUT) {
          Electron().cleanJetPtrel = Electron.v4().Perp(cleanjet_v4.Vect());
          if (debug>1) std::cout<<"  + pass pt cut, Ptrel="<<Electron().cleanJetPtrel<<std::endl;
        }
      }
      if (debug>1) std::cout<<"-------------------------------------------------------------"<<std::endl;
      if (debug>1) std::cout<<"ele jet DR="<<Electron().jetDRmin<<" Ptrel="<<Electron().cleanJetPtrel<<std::endl<<std::endl;

      // Reconstruct the neutrino 4 momentum using W mass constraint
      // https://twiki.cern.ch/twiki/bin/view/Main/TopPairProduction
      // https://github.com/BoostedScalefactors/WTopScalefactorProducer/blob/master/Skimmer/python/variables.py
      // Do the calculation for all ellectron and muon candidates
      double MET_px = MET_pt*std::cos(MET_phi);
      double MET_py = MET_pt*std::sin(MET_phi);
      double a = 80.379*80.379 - Electron.v4().M()*Electron.v4().M() + 2*Electron.v4().Px()*MET_px + 2*Electron.v4().Py()*MET_py;
      double A =  4 * (Electron.v4().E()*Electron.v4().E() - Electron.v4().Pz()*Electron.v4().Pz());
      double B = -4 * a * Electron.v4().Pz();
      double C =  4 * (Electron.v4().E()*Electron.v4().E()) * (MET_px*MET_px + MET_py*MET_py) - a*a;
      double D = B*B - 4*A*C;
      // If there are real solutions, use the one with lowest Pz                                            
      double MET_pz = D>=0 ? std::min((-B+std::sqrt(D))/(2*A), (-B-std::sqrt(D))/(2*A)) : -B/(2*A);
      Electron().nu.SetXYZM(MET_px, MET_py, MET_pz, 0);
      Electron().nuDR = Electron().nu.DeltaR(Electron.v4());

      // Assoicated AK8 jet
      double mindR_AK8 = NOVAL_F;
      size_t iMatchAK8 = -1;
      while(FatJet.Loop()) {
        double dR = Electron.v4().DeltaR(FatJet.v4());
        if (dR<0.8 && dR<mindR_AK8) {
          mindR_AK8 = dR;
          Electron().matchAK8 = true;
          iMatchAK8 = FatJet.i;
        }
      }
      if (Electron().matchAK8) {
        Electron().iMatchedAK8 = iMatchAK8;
        Electron().nuMatchedAK8DR = Electron().nu.DeltaR(FatJet.v4(iMatchAK8));
      }

      // Selected objects
      // Veto
      if (Electron.VetoNoIso.define(Electron().mvaFall17V2noIso_WPL &&
                                    pt      >= ELE_VETO_PT_CUT &&
                                    abseta  <  ELE_VETO_ETA_CUT && //!(abseta>=1.442 && abseta< 1.556) &&
                                    absd0   <  ELE_VETO_IP_D0_CUT &&
                                    absdz   <  ELE_VETO_IP_DZ_CUT))
        Electron.Veto       .define((miniIso <  ELE_VETO_MINIISO_CUT));
      
      // Select
      Electron.Select.define( Electron().mvaFall17V2noIso_WP90 &&
                              pt        >= ELE_SELECT_PT_CUT &&
                              abseta    <  ELE_SELECT_ETA_CUT && !(abseta>=1.442 && abseta< 1.556) &&
                              miniIso   <  ELE_SELECT_MINIISO_CUT &&
                              absd0     <  ELE_SELECT_IP_D0_CUT &&
                              absdz     <  ELE_SELECT_IP_DZ_CUT);
      
      if ( pt        >= ELE_TIGHT_PT_CUT &&
           abseta    <  ELE_TIGHT_ETA_CUT && !(abseta>=1.442 && abseta< 1.556) &&
           //ipsig     <  ELE_TIGHT_IP_SIG_CUT &&
           absd0     <  ELE_TIGHT_IP_D0_CUT &&
           absdz     <  ELE_TIGHT_IP_DZ_CUT) {
        // Tight - ID with isolation - Medium MVA ID
        Electron.Tight.define(Electron().mvaFall17V2Iso_WP90);
        // Non-isolated lepton - Loose MVA ID
        if (Electron.NoIso.define(Electron().mvaFall17V2noIso_WPL)) {
          // + Loose 2D isolation
          Electron.NonIso.define(!(Electron().jetDRmin<0.4 && Electron().cleanJetPtrel<15));
        }
      }      
    }
    if (debug) std::cout<<"Variables::define_leptons_and_photons_: end electron definitions"<<std::endl;


    // Muons - full definitions
    Muon.initObjects();
    while (Muon.Loop()) {
      float pt      = Muon().pt;
      float abseta  = std::abs(Muon().eta);
      float absd0   = std::abs(Muon().dxy);
      float absdz   = std::abs(Muon().dz);
      float miniIso = Muon().miniPFRelIso_all;
      float ipsig   = std::abs(Muon().sip3d);
      float relIso = Muon().pfRelIso04_all;

      // Calculate the B2G 2D cut variables
      // [Lepton]_jetPtRelv2 is available in v5 NanoAOD, but it is bugged :(
      // Try to do what (should be) done in NanoAOD (revert JEC, subtract ele, reapply JEC) based on the original NanoAOD recipe:
      // https://github.com/cms-nanoAOD/cmssw/blob/master-cmsswmaster/PhysicsTools/NanoAOD/plugins/LeptonJetVarProducer.cc#L152-L162
      // 1) if no jetIdx, or no jet id --> match nearest jet passing the ID for DR, leave Ptrel = NOVAL_F
      // 2) if    jetIdx -> calculate DR
      // 3)              -> subtract lep pt from jet, check if passs pt cut (if not Ptrel=NOVAL_F), calculate Ptrel
      // Calculate dRmin for the associated/nearest jet
      int jetIdx = Muon().jetIdx;
      Muon().isPartOfJet = true;
      if (jetIdx==-1) Muon().isPartOfJet = false;
      else if (!Jet.Jet.pass[jetIdx]) Muon().isPartOfJet = false;
      if (debug>1) std::cout<<"Start calculating 2D cut for - iMu="<<Muon.i<<" isPartOfJet="<<Muon().isPartOfJet<<" matched jetIdx="<<jetIdx<<std::endl;
      if (Muon().isPartOfJet) {
        Muon().jetDRmin = Muon.v4().DeltaR(Jet.v4(jetIdx));
      } else while (Jet.Jet.Loop()) {
        double dR = Muon.v4().DeltaR(Jet.Jet.v4());
        if (dR<Muon().jetDRmin) {
          Muon().jetDRmin = dR;
          if (debug>1) std::cout<<" - matched to jetdIdx="<<Jet.Jet.iRef<<" dR="<<dR<<std::endl; 
        }
      }
      if (debug>1) std::cout<<"jetIdx="<<jetIdx<<std::endl;
      // Calculate pTrel for the associated/nearest jet
      // First subtract the muon from the jet
      // Similar to [Lepton]_jetPtRelv2, a useful variable Jet_muonSubtrFactor is in NanoAOD v5
      // Problem is that this variable is also bugged :(
      // The JEC reverted muon energy subtraction should work in the mean time (similar to electrons)
      if (jetIdx!=-1) {
        TLorentzVector cleanjet_v4 = Jet.v4(jetIdx);
        if (debug>1) std::cout<<"-       jet pt="<<cleanjet_v4.Pt()<<std::endl;
        //cleanjet_v4 *= Jet(jetIdx).muonSubtrFactor; // Variable bugged
        cleanjet_v4 -= Muon.v4()*(1/(1-Jet(jetIdx).rawFactor));
        // Then, for version 2 check also that the cleaned jet passes the minimum pt threshold cut that is considered in the analysis
        // If not, revert back to the previous closest jet
        if (debug>1) std::cout<<"- clean jet pt="<<cleanjet_v4.Pt()<<std::endl;
        if (cleanjet_v4.Pt() >= JET_AK4_PT_CUT) {
          Muon().cleanJetPtrel = Muon.v4().Perp(cleanjet_v4.Vect());
          if (debug>1) std::cout<<"  + pass pt cut, Ptrel="<<Muon().cleanJetPtrel<<std::endl;
        }
      }
      if (debug>1) std::cout<<"-------------------------------------------------------------"<<std::endl;
      if (debug>1) std::cout<<"mu jet DRmin="<<Muon().jetDRmin<<" Ptrel="<<Muon().cleanJetPtrel<<std::endl<<std::endl;

      // Reconstruct the neutrino 4 momentum using W mass constraint
      // https://twiki.cern.ch/twiki/bin/view/Main/TopPairProduction
      // https://github.com/BoostedScalefactors/WTopScalefactorProducer/blob/master/Skimmer/python/variables.py
      // Do the calculation for all ellectron and muon candidates
      double MET_px = MET_pt*std::cos(MET_phi);
      double MET_py = MET_pt*std::sin(MET_phi);
      double a = 80.379*80.379 - Muon.v4().M()*Muon.v4().M() + 2*Muon.v4().Px()*MET_px + 2*Muon.v4().Py()*MET_py;
      double A =  4 * (Muon.v4().E()*Muon.v4().E() - Muon.v4().Pz()*Muon.v4().Pz());
      double B = -4 * a * Muon.v4().Pz();
      double C =  4 * (Muon.v4().E()*Muon.v4().E()) * (MET_px*MET_px + MET_py*MET_py) - a*a;
      double D = B*B - 4*A*C;
      // If there are real solutions, use the one with lowest Pz                                            
      double MET_pz = D>=0 ? std::min((-B+std::sqrt(D))/(2*A), (-B-std::sqrt(D))/(2*A)) : -B/(2*A);
      Muon().nu.SetXYZM(MET_px, MET_py, MET_pz, 0);
      Muon().nuDR = Muon().nu.DeltaR(Muon.v4());
    
      // Assoicated AK8 jet
      double mindR_AK8 = NOVAL_F;
      size_t iMatchAK8 = -1;
      while(FatJet.Loop()) {
        double dR = Muon.v4().DeltaR(FatJet.v4());
        if (dR<0.8 && dR<mindR_AK8) {
          mindR_AK8 = dR;
          Muon().matchAK8 = true;
          iMatchAK8 = FatJet.i;
        }
      }
      if (Muon().matchAK8) {
        Muon().iMatchedAK8 = iMatchAK8;
        Muon().nuMatchedAK8DR = Muon().nu.DeltaR(FatJet.v4(iMatchAK8));
      }
    
      // Selected objects
      // Veto
      if (Muon.VetoNoIso.define(Muon().softId &&
                                pt      >= MU_VETO_PT_CUT &&
                                abseta  <  MU_VETO_ETA_CUT &&
                                absd0   <  MU_VETO_IP_D0_CUT &&
                                absdz   <  MU_VETO_IP_DZ_CUT))
        Muon.Veto       .define(miniIso <  MU_VETO_MINIISO_CUT);
      
      // Select
      Muon.Select.define( Muon().mediumPromptId &&
                          pt        >= MU_SELECT_PT_CUT &&
                          abseta    <  MU_SELECT_ETA_CUT &&
                          miniIso   <  MU_SELECT_MINIISO_CUT &&
                          absd0     <  MU_SELECT_IP_D0_CUT &&
                          absdz     <  MU_SELECT_IP_DZ_CUT);
       
      if (pt        >= MU_TIGHT_PT_CUT &&
          abseta    <  MU_TIGHT_ETA_CUT &&
          ipsig     <  MU_TIGHT_IP_SIG_CUT &&
          absd0     <  MU_TIGHT_IP_D0_CUT &&
          absdz     <  MU_TIGHT_IP_DZ_CUT) {
        // Tight - ID without isolation + relIso
        Muon.Tight.define(Muon().tightId &&
                          relIso  <  MU_TIGHT_RELISO_CUT);
        // Non-isolated lepton - Loose MVA ID
        if (Muon.NoIso.define(Muon().mvaId>0)) {
          // + Loose 2D isolation
          Muon.NonIso.define(!(Muon().jetDRmin<0.4 && Muon().cleanJetPtrel<15));
        }
      }
    }
    if (debug) std::cout<<"Variables::define_leptons_and_photons_: end muon definitions"<<std::endl;

    // Taus - full definitions
    Tau.initObjects();
    while (Tau.Loop())
      Tau.Veto.define((Tau().idMVAnewDM2017v2&1)); // --> VVLoose WP
    // No pt/eta cut
    if (debug) std::cout<<"Variables::define_leptons_and_photons_: end tau definitions"<<std::endl;
    
    // Photons - full definitions
    Photon  .initObjects();
    while (Photon.Loop()) {
      float pt = Photon().pt;
      float abseta = std::abs(Photon().eta);
      bool ele_veto  = Photon().electronVeto;
      bool pixel_veto = Photon().pixelSeed;
      //bool id_select = (Photon().Photon_cutBasedBitmap&2)==2;
  
      // Implementing cuts for Fall17 V2 ID
      // https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2?rev=49#Working_points_for_94X_and_later
      // https://github.com/cms-sw/cmssw/blob/master/RecoEgamma/PhotonIdentification/python/Identification/cutBasedPhotonID_Fall17_94X_V2_cff.py#L53-L81
      // Revert Sigma_ietaieta cut in order to be able to measure QCD background
      // Fit shape of the charge distribution
      // Do not apply both cuts for preselection
      // select fakes with reverted sieie cut
      
      bool id_preselect = false, id_select = false;
      // Cut based ID, without
      if (Photon().isScEtaEB){
        // Barrel cuts (EB)
        id_preselect =
          Photon().hoe                                      < 0.02197 &&
          Photon().pfRelIso03_all - Photon().pfRelIso03_chg < 1.189+0.01512*pt+2.259e-05*pt*pt &&
          Photon().pfRelIso03_all                           < 2.08+0.004017*pt;
        id_select = id_preselect &&
          Photon().sieie                                    < 0.01015 &&
          Photon().pfRelIso03_chg                           < 1.141;
      } else {
        // Encap cuts (EE)
        id_preselect =
          Photon().hoe                                      < 0.0326 &&
          Photon().pfRelIso03_all - Photon().pfRelIso03_chg < 2.718+0.0117*pt+2.3e-05*pt*pt &&
          Photon().pfRelIso03_all                           < 3.867+0.0037*pt;
        id_select = id_preselect &&
          Photon().sieie                                    < 0.0272 &&
          Photon().pfRelIso03_chg                           < 1.051;
      }
      // Medium ID without Sigma_ietaieta cut
      if (Photon.PreSelect.define(id_preselect &&
                                  ele_veto &&
                                  !pixel_veto &&
                                  pt        >= PHOTON_SELECT_PT_CUT &&
                                  abseta    <  PHOTON_SELECT_ETA_CUT )) {
        // Fake photons (those that fail the SigmaIeteIeta cut)
        Photon.Fake.define(Photon().sieie >= (Photon().isScEtaEB ? 0.01015 : 0.0272));
      }
  
      // Photons passing full ID
      Photon.Select.define(id_select &&
                           ele_veto &&
                           !pixel_veto &&
                           pt        >= PHOTON_SELECT_PT_CUT &&
                           abseta    <  PHOTON_SELECT_ETA_CUT );
    }
    if (debug) std::cout<<"Variables::define_leptons_and_photons_: end photon definitions"<<std::endl;
  }
 
  void define_jets_(int debug = 0) {
    Jet.initObjects();
    // AK4 jets - Definitions except those depending on AK8 jets
    while (Jet.Loop()) {
      if (debug>1) std::cout<<"Variables::define_jets_: AK4 "<<Jet.i<<" start"<<std::endl;
      // Jet ID
      if (Jet.Jet.define( (Jet().jetId == 2 || Jet().jetId == 6) &&
                          Jet().pt            >= JET_AK4_PT_CUT &&
                          //Jet().chHEF > 0.05 && Jet().neHEF < 0.8 && Jet().neEmEF < 0.7 &&
                          std::abs(Jet().eta)  < JET_AK4_ETA_CUT)) {
        
        if (debug>1) std::cout<<"Variables::define_jets_: AK4 "<<Jet.i<<" id ok"<<std::endl;
        
        // b tagging
        Jet.LooseBTag .define(Jet().btagDeepB >= B_DEEP_LOOSE_CUT);
        Jet.MediumBTag.define(Jet().btagDeepB >= B_DEEP_MEDIUM_CUT);
        Jet.TightBTag .define(Jet().btagDeepB >= B_DEEP_TIGHT_CUT);
        if (debug>1) std::cout<<"Variables::define_jets_: AK4 "<<Jet.i<<" b-tagging ok"<<std::endl;
        
        // Lepton-jet overlap
        while (Electron.Loop()) {
          double dR = Electron.v4().DeltaR(Jet.v4());
          if (dR<Electron().jetDR) {
            Electron().iMatchedAK4 = Jet.i;
            Electron().jetDR   = dR;
            Electron().jetDPhi = std::abs(Electron.v4().DeltaPhi(Jet.v4()));
          }
          if (Electron.Veto.pass[Electron.i]&&dR<Jet().eleDR) {
            Jet().eleDR = dR;
            Jet().elePtRatio = Electron().pt/Jet().pt;
          }
        }
        while (Muon.Loop()) {
          double dR = Muon.v4().DeltaR(Jet.v4());
          if (dR<Muon().jetDR) {
            Muon().iMatchedAK4 = Jet.i;
            Muon().jetDR   = dR;
            Muon().jetDPhi = std::abs(Muon.v4().DeltaPhi(Jet.v4()));
          }
          if (Muon.Veto.pass[Muon.i]&&dR<Jet().muDR) {
            Jet().muDR = dR;
            Jet().muPtRatio = Muon().pt/Jet().pt;
          }
        }
        if (debug>1) std::cout<<"Variables::define_jets_: AK4 "<<Jet.i<<" lepton-overlap ok"<<std::endl;
        // Photon-jet overlap
        while (Photon.Select.Loop()) {
          double dR = Photon.Select.v4().DeltaR(Jet.v4());
          if (dR<Jet().phoDR) {
            Jet().phoDR = dR;
            Jet().phoPtRatio = Photon.Select().pt/Jet().pt;
          }
        }
        // Exclude jets that have overlapping photon
        if (Photon.Select.n==1) {
          if (Jet.JetNoPho.define(Photon.Select(0).jetIdx!=Jet.i))
            Jet.MediumBTagNoPho.define(Jet().btagDeepB >= B_DEEP_MEDIUM_CUT);
        } else if (Photon.PreSelect.n==1) {
          if (Jet.JetNoPho.define(Photon.PreSelect(0).jetIdx!=Jet.i))
            Jet.MediumBTagNoPho.define(Jet().btagDeepB >= B_DEEP_MEDIUM_CUT);
        }
        if (debug>1) std::cout<<"Variables::define_jets_: AK4 "<<Jet.i<<" photon-overlap ok"<<std::endl;
      }
      
    } // End AK4 Jet Loop
    if (debug) std::cout<<"Variables::define_leptons_and_photons_: AK4 ordinary jets end"<<std::endl;


    /*
    // Match non isolated tight lepton to nearest AK4 jet to be able to tell which Razor megajet it belongs
    for (size_t i=0; i<noiso_leptons.size(); ++i) {
      bool match = false;
      if (iJetLepNoIso[i]==-1) match = true;
      else if (!Jet.Jet.pass[iJetLepNoIso[i]]) match = true;
      if (match) {
        double mindR = NOVAL_F;
        for(size_t j=0; j<selected_jets.size(); ++j) {
          double dR = noiso_leptons[i].DeltaR(selected_jets[j]);
          if (dR<mindR) {
            mindR = dR;
            iJetLepNoIso[i] = iJet[j];
          }
        }
      }
    }
    for (size_t i=0; i<noniso_leptons.size(); ++i) {
      bool match = false;
      if (iJetLepNonIso[i]==-1) match = true;
      else if (!Jet.Jet.pass[iJetLepNonIso[i]]) match = true;
      if (match) {
        double mindR = NOVAL_F;
        for(size_t j=0; j<selected_jets.size(); ++j) {
          double dR = noniso_leptons[i].DeltaR(selected_jets[j]);
          if (dR<mindR) {
            mindR = dR;
            iJetLepNonIso[i] = iJet[j];
          }
        }
      }
    }
    */

    // AK8 jets and subjets - full definitions
    SubJet.initObjects();
    FatJet.initObjects();
    while (FatJet.Loop()) {
      if (debug) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" start"<<std::endl;
      // N-subjettiness
      if (FatJet().tau1>0) FatJet().tau21 = FatJet().tau2/FatJet().tau1;
      if (FatJet().tau1>0) FatJet().tau31 = FatJet().tau3/FatJet().tau1;
      if (FatJet().tau2>0) FatJet().tau32 = FatJet().tau3/FatJet().tau2;
      // Maximum SubJet btag discriminator
      for (const int& iSubJet : {FatJet().subJetIdx1, FatJet().subJetIdx2}) if (iSubJet != -1) {
        FatJet().nSubJet++;
        if (SubJet(iSubJet).btagDeepB > FatJet().maxSubJetDeepB) FatJet().maxSubJetDeepB = SubJet(iSubJet).btagDeepB;
      }
      if ((FatJet().passSubJetBTag = (FatJet().maxSubJetDeepB > TOP_BTAG_DEEP))) FatJet().nSubJetBTag++;
      if (debug) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" additional var ok"<<std::endl;

      // Jet ID
      if (FatJet.JetAK8.define((FatJet().jetId == 2 || FatJet().jetId == 6) &&
                               FatJet().pt             >= JET_AK8_PT_CUT &&
                               std::abs(FatJet().eta)  <  JET_AK8_ETA_CUT)) {
        
        if (debug>1) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" id ok"<<std::endl;

        // Lepton-jet overlap
        while (Electron.Loop()) {
          double dR = Electron.v4().DeltaR(FatJet.v4());
          if (dR<Electron().ak8JetDR) {
            Electron().iMatchedAK8 = FatJet.i;
            Electron().ak8JetDR   = dR;
            Electron().ak8JetDPhi = std::abs(Electron.v4().DeltaPhi(FatJet.v4()));
          }
          if (Electron.Veto.pass[Electron.i]&&dR<FatJet().eleDR) {
            FatJet().eleDR = dR;
            FatJet().elePtRatio = Electron().pt/FatJet().pt;
          }
        }
        while (Muon.Loop()) {
          double dR = Muon.v4().DeltaR(FatJet.v4());
          if (dR<Muon().ak8JetDR) {
            Muon().iMatchedAK8 = FatJet.i;
            Muon().ak8JetDR   = dR;
            Muon().ak8JetDPhi = std::abs(Muon.v4().DeltaPhi(FatJet.v4()));
          }
          if (Muon.Veto.pass[Muon.i]&&dR<FatJet().muDR) {
            FatJet().muDR = dR;
            FatJet().muPtRatio = Muon().pt/FatJet().pt;
          }
        }
        if (debug>1) std::cout<<"Variables::define_jets_: AK4 "<<FatJet.i<<" lepton-overlap ok"<<std::endl;
        // Photon-jet overlap
        while (Photon.Select.Loop()) {
          double dR = Photon.Select.v4().DeltaR(FatJet.v4());
          if (dR<FatJet().phoDR) {
            FatJet().phoDR = dR;
            FatJet().phoPtRatio = Photon.Select().pt/FatJet().pt;
          }
        }
        if (debug>1) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" lepton-photon overlap ok"<<std::endl;

        // Calculate LSF for 2D-isolated leptons
        double lepSubJetDR = NOVAL_F;
        while (Electron.NonIso.Loop()) {
          double DR = Electron.NonIso.v4().DeltaR(FatJet.v4());
          if (DR<0.8 && DR<lepSubJetDR) {
            FatJet().matchLepNonIso = true;
            lepSubJetDR = DR;
            FatJet().LSF = std::min(double(Electron.NonIso().pt / FatJet().pt), 0.999999);
            FatJet().lepNonIsoNuDR = Electron.NonIso.v4().DeltaR(Electron.NonIso().nu);
          }
          for (const int& iSubJet : { FatJet().subJetIdx1, FatJet().subJetIdx2 }) if (iSubJet!=-1) {
            DR = Electron.NonIso.v4().DeltaR(SubJet.v4(iSubJet));
            if (DR<0.8 && DR<lepSubJetDR) {
              FatJet().matchLepNonIso = true;
              lepSubJetDR = DR;
              FatJet().LSF = std::min(double(Electron.NonIso().pt / SubJet(iSubJet).pt), 0.999999);
              FatJet().lepNonIsoNuDR = Electron.NonIso.v4().DeltaR(Electron.NonIso().nu);
            }
          }
        }
        while (Muon.NonIso.Loop()) {
          double DR = Muon.NonIso.v4().DeltaR(FatJet.v4());
          if (DR<0.8 && DR<lepSubJetDR) {
            FatJet().matchLepNonIso = true;
            lepSubJetDR = DR;
            FatJet().LSF = std::min(double(Muon.NonIso().pt / FatJet().pt), 0.999999);
            FatJet().lepNonIsoNuDR = Muon.NonIso.v4().DeltaR(Muon.NonIso().nu);
          }
          for (const int& iSubJet : { FatJet().subJetIdx1, FatJet().subJetIdx2 }) if (iSubJet!=-1) {
            DR = Muon.NonIso.v4().DeltaR(SubJet.v4(iSubJet));
            if (DR<0.8 && DR<lepSubJetDR) {
              FatJet().matchLepNonIso = true;
              lepSubJetDR = DR;
              FatJet().LSF = std::min(double(Muon.NonIso().pt / SubJet(iSubJet).pt), 0.999999);
              FatJet().lepNonIsoNuDR = Muon.NonIso.v4().DeltaR(Muon.NonIso().nu);
            }
          }
        }
        // Same with lepton without 2D isolation
        lepSubJetDR = NOVAL_F;
        while (Electron.NoIso.Loop()) {
          double DR = Electron.NoIso.v4().DeltaR(FatJet.v4());
          if (DR<0.8 && DR<lepSubJetDR) {
            FatJet().matchLepNoIso = true;
            lepSubJetDR = DR;
            FatJet().LSF = std::min(double(Electron.NoIso().pt / FatJet().pt), 0.999999);
            FatJet().matchedNoIsoLepJetDRmin = Electron.NoIso().jetDRmin;
            FatJet().matchedNoIsoLepCleanJetPtrel = Electron.NoIso().cleanJetPtrel;
          }
          for (const int& iSubJet : { FatJet().subJetIdx1, FatJet().subJetIdx2 }) if (iSubJet!=-1) {
            DR = Electron.NoIso.v4().DeltaR(SubJet.v4(iSubJet));
            if (DR<0.8 && DR<lepSubJetDR) {
              FatJet().matchLepNoIso = true;
              lepSubJetDR = DR;
              FatJet().LSF = std::min(double(Electron.NoIso().pt / SubJet(iSubJet).pt), 0.999999);
              FatJet().matchedNoIsoLepJetDRmin = Electron.NoIso().jetDRmin;
              FatJet().matchedNoIsoLepCleanJetPtrel = Electron.NoIso().cleanJetPtrel;
            }
          }
        }
        while (Muon.NoIso.Loop()) {
          double DR = Muon.NoIso.v4().DeltaR(FatJet.v4());
          if (DR<0.8 && DR<lepSubJetDR) {
            FatJet().matchLepNoIso = true;
            lepSubJetDR = DR;
            FatJet().LSF = std::min(double(Muon.NoIso().pt / FatJet().pt), 0.999999);
            FatJet().matchedNoIsoLepJetDRmin = Muon.NoIso().jetDRmin;
            FatJet().matchedNoIsoLepCleanJetPtrel = Muon.NoIso().cleanJetPtrel;
          }
          for (const int& iSubJet : { FatJet().subJetIdx1, FatJet().subJetIdx2 }) if (iSubJet!=-1) {
            DR = Muon.NoIso.v4().DeltaR(SubJet.v4(iSubJet));
            if (DR<0.8 && DR<lepSubJetDR) {
              FatJet().matchLepNoIso = true;
              lepSubJetDR = DR;
              FatJet().LSF = std::min(double(Muon.NoIso().pt / SubJet(iSubJet).pt), 0.999999);
              FatJet().matchedNoIsoLepJetDRmin = Muon.NoIso().jetDRmin;
              FatJet().matchedNoIsoLepCleanJetPtrel = Muon.NoIso().cleanJetPtrel;
            }
          }
        }  
        if (debug>1) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" LSF/2D Iso ok"<<std::endl;

        // New selection for Control regions
        FatJet.JetAK8Mass.define(FatJet().msoftdrop >= 50);
        
        // Tagging Variables
        double tau_21     = FatJet().tau21;
        double tau_32     = FatJet().tau32;
        double pt         = FatJet().pt;
        double abseta     = FatJet().eta;
        double deepMD_w   = FatJet().deepTagMD_WvsQCD;
        double deepMD_z   = FatJet().deepTagMD_ZvsQCD;
        double deepMD_h   = FatJet().deepTagMD_HbbvsQCD;
        double deepMD_top = FatJet().deepTagMD_TvsQCD;
        double deep_w     = FatJet().deepTag_WvsQCD;
        double deep_z     = FatJet().deepTag_ZvsQCD;
        //double deep_h     = FatJet().deepTag_H; // Only in v5 NanoAOD
        double deep_top   = FatJet().deepTag_TvsQCD;
        double sd_mass    = FatJet().msoftdrop;

        if (debug>1) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" top tag ok"<<std::endl;
        // New and old tagger WPs
        // WPs from slide 4 in (2017 values)
        // https://indico.cern.ch/event/840827/contributions/3527925/attachments/1895214/3126510/DeepAK8_Top_W_SFs_2017_JMAR_PK.pdf
        // Hadronic W tagging
        if (pt        >= HADW_PT_CUT &&
            abseta    <  HADW_ETA_CUT &&
            sd_mass   >= HADW_SD_MASS_CUT_LOW &&
            sd_mass   <  HADW_SD_MASS_CUT_HIGH) {
          FatJet.W1.define(tau_21 < 0.55);
          FatJet.W2.define(tau_21 < 0.45);
          FatJet.W3.define(tau_21 < 0.35);
          FatJet.WDeepMD1.define(deepMD_w > 0.313);
          FatJet.WDeepMD2.define(deepMD_w > 0.802);
          FatJet.WDeepMD3.define(deepMD_w > 0.884);
        }
        if (pt        >= HADW_PT_CUT &&
            abseta    <  HADW_ETA_CUT) {
          FatJet.WDeep1.define(deep_w > 0.779);
          FatJet.WDeep2.define(deep_w > 0.981);
          FatJet.WDeep3.define(deep_w > 0.991);
        }
        if (debug>1) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" new W tag ok"<<std::endl;
        // Hadronic Z Tagging
        // Use some ad-hoc working points now (unknown mistag rate)
        // TODO: Check again with signal
        if (pt        >= HADZ_PT_CUT &&
            abseta    <  HADZ_ETA_CUT) {
          if ( sd_mass   >= HADZ_SD_MASS_CUT_LOW &&
               sd_mass   <  HADZ_SD_MASS_CUT_HIGH ) {
            FatJet.ZDeepMD1.define(deepMD_z > 0.30);
            FatJet.ZDeepMD2.define(deepMD_z > 0.80);
            FatJet.ZDeepMD3.define(deepMD_z > 0.90);
          }
          FatJet.ZDeep1.define(deep_z > 0.8  );
          FatJet.ZDeep2.define(deep_z > 0.95 );
          FatJet.ZDeep3.define(deep_z > 0.99 );
        }
        if (debug>1) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" new Z tag ok"<<std::endl;
        // Hadronic Higgs Tagging
        // Use some ad-hoc working points now (unknown mistag rate)
        // TODO: Check latest WPs (if available) and check performance with signal
        if (pt        >= HADH_PT_CUT &&
            abseta    <  HADH_ETA_CUT) {
          if ( sd_mass   >= HADH_SD_MASS_CUT_LOW &&
               sd_mass   <  HADH_SD_MASS_CUT_HIGH ) {
            FatJet.HDeepMD1.define(deepMD_h > 0.30);
            FatJet.HDeepMD2.define(deepMD_h > 0.80);
            FatJet.HDeepMD3.define(deepMD_h > 0.90);
          }
          //FatJet.HDeep1.define(deep_h > 0.80);
          //FatJet.HDeep2.define(deep_h > 0.95);
          //FatJet.HDeep3.define(deep_h > 0.99);
        }
        if (debug>1) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" new H tag ok"<<std::endl;
        // Hadronic top tagging
        if (pt        >= HADTOP_PT_CUT &&
            abseta    <  HADTOP_ETA_CUT &&
            sd_mass   >= HADTOP_SD_MASS_CUT_LOW &&
            sd_mass   <  HADTOP_SD_MASS_CUT_HIGH &&
            FatJet().passSubJetBTag) {
          FatJet.Top1.define(tau_32 < 0.80);
          FatJet.Top2.define(tau_32 < 0.65);
          FatJet.Top3.define(tau_32 < 0.54);
          FatJet.Top4.define(tau_32 < 0.46);
          FatJet.Top5.define(tau_32 < 0.40);
        }
        // Pass mass tag without upper mass cut
        // TODO: Check signal discrimination of softdrop mass N-1 (no max cut)
        if ( pt          >= HADTOP_PT_CUT &&
             abseta      <  HADTOP_ETA_CUT &&
             sd_mass     >= HADTOP_SD_MASS_CUT_LOW ) {
          FatJet.TopDeepMD1.define(deepMD_top > 0.054);
          FatJet.TopDeepMD2.define(deepMD_top > 0.391);
          FatJet.TopDeepMD3.define(deepMD_top > 0.578);
          FatJet.TopDeepMD4.define(deepMD_top > 0.843);
        }
        if (pt        >= HADTOP_PT_CUT &&
            abseta    <  HADTOP_ETA_CUT) {
          FatJet.TopDeep1.define(deep_top > 0.093);
          FatJet.TopDeep2.define(deep_top > 0.745);
          FatJet.TopDeep3.define(deep_top > 0.895);
          FatJet.TopDeep4.define(deep_top > 0.986);
        }
        if (debug>1) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" new had top tag ok"<<std::endl;
        // Leptonic top tagging
        if (FatJet.LepJetCand.define( FatJet().matchLepNonIso &&
                                      pt          >= LEPTOP_PT_CUT &&
                                      abseta       < LEPTOP_ETA_CUT )) {
          if ( sd_mass     >= LEPTOP_SD_MASS_CUT_LOW ) {
            FatJet.LepTop.define( FatJet().passSubJetBTag);
            FatJet.LepJet.define(!FatJet().passSubJetBTag);
          }
          // High mass version
          if ( sd_mass     >= LEPTOP_SD_MASS_CUT_HIGH ) {
            FatJet.LepTopHighMass.define( FatJet().passSubJetBTag);
            FatJet.LepJetHighMass.define(!FatJet().passSubJetBTag);
          }
        }
        if (debug>1) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" new lep top tag ok"<<std::endl;
        // N-1 selections
        FatJet.LepJetNoIso.define
          ( FatJet().matchLepNoIso &&
            pt          >= LEPTOP_PT_CUT &&
            abseta       < LEPTOP_ETA_CUT &&
            sd_mass     >= LEPTOP_SD_MASS_CUT_LOW &&
            !FatJet().passSubJetBTag );
        FatJet.LepJetNoPt.define
          ( FatJet().matchLepNonIso &&
            abseta       < LEPTOP_ETA_CUT &&
            sd_mass     >= LEPTOP_SD_MASS_CUT_LOW &&
            !FatJet().passSubJetBTag );
        FatJet.LepTopNoIso.define
          ( FatJet().matchLepNoIso &&
            pt          >= LEPTOP_PT_CUT &&
            abseta       < LEPTOP_ETA_CUT &&
            sd_mass     >= LEPTOP_SD_MASS_CUT_LOW &&
            FatJet().passSubJetBTag );
        FatJet.LepTopNoPt.define 
          ( FatJet().matchLepNonIso &&
            abseta       < LEPTOP_ETA_CUT &&
            sd_mass     >= LEPTOP_SD_MASS_CUT_LOW &&
            FatJet().passSubJetBTag );
        FatJet.LepTopNoMass.define 
          ( FatJet().matchLepNonIso &&
            pt          >= LEPTOP_PT_CUT &&
            abseta       < LEPTOP_ETA_CUT &&
            FatJet().passSubJetBTag );
        FatJet.LepTopNoSubJetB.define 
          ( FatJet().matchLepNonIso &&
            pt          >= LEPTOP_PT_CUT &&
            abseta       < LEPTOP_ETA_CUT &&
            sd_mass     >= LEPTOP_SD_MASS_CUT_LOW );
        if (debug>1) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" new lep top tag N-1 ok"<<std::endl;
        // Exclusive hadronic jet tag definitons
        if (!(FatJet.LepJet.pass[FatJet.i]||FatJet.LepTop.pass[FatJet.i])) {
          // For top signals
          if (!FatJet.HadTop.define(FatJet.TopDeep1.pass[FatJet.i])) {
            FatJet.HadW.define(FatJet.WDeep2.pass[FatJet.i]);
            FatJet.HadZ.define(FatJet.ZDeep2.pass[FatJet.i]);
            FatJet.HadV.define(FatJet.WDeep2.pass[FatJet.i]||FatJet.ZDeep2.pass[FatJet.i]);
            FatJet.HadH.define(FatJet.HDeep2.pass[FatJet.i]);
          }
        }
        if (debug>1) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" new taggers ok"<<std::endl;
  
      } // End Jet Selection
  
      if (debug) std::cout<<"Variables::define_jets_: AK8 "<<FatJet.i<<" end"<<std::endl;
    } // End AK8 Jet Loop
    if (debug) std::cout<<"Variables::define_jets_: AK8 jets end"<<std::endl;

    // AK4 jets - Variables/objects depending on AK8 jets
    // Do not call initObjects() (it was already done earlier)!!
    while (Jet.Loop()) {
      // Isolated btags (not inside V tagged AK8)
      double minDR = NOVAL_F;
      while (FatJet.Loop()) {
        if (FatJet.HadV.pass[FatJet.i]||FatJet.HadH.pass[FatJet.i]) {
          double DR = Jet.v4().DeltaR(FatJet.v4());
          if (DR<minDR) minDR = DR;
        }
      }
      if (minDR>=0.8) {
        Jet.LooseIsoBTag. define(Jet.LooseBTag. pass[Jet.i]);
        Jet.MediumIsoBTag.define(Jet.MediumBTag.pass[Jet.i]);
      }
    } // End special AK4 Jet Loop
    if (debug) std::cout<<"Variables::define_jets_: AK4 special (AK8 dependent) jets end"<<std::endl;
  }
  
  void define_genparticles_(int debug = 0) {
    GenPart .initObjects();

    // Loop on generator particles
    // 1st Loop is a recursive search for (grand/)mothers
    // set flags based on (grand/)daughters (eg. leptonic decays for W/top)
    // TODO: debug Data, which says the collection size is not 0
    while (GenPart.Loop()) {
      if (debug>1) std::cout<<"Variables::define_genparticles_: gen 1st loop "<<GenPart.i<<" start"<<std::endl;
      int motherId = -NOVAL_I, grandMotherId = -NOVAL_I, greatGrandMotherId = -NOVAL_I;
      int iMother = GenPart().genPartIdxMother, iGrandMother = -1, iGreatGrandMother = -1;
      // Set the mother to the first ancestor that has different pdg id
      if (iMother!=-1) {
        while (GenPart(iMother).pdgId == GenPart().pdgId) {
          GenPart(iMother).NoSameDaughter = 0;
          iMother = GenPart(iMother).genPartIdxMother;
          if (iMother==-1) break;
        }
        if (iMother!=-1) {
          motherId = GenPart(iMother).pdgId;
          // The mother is a candidate for final non-decayed state
          // But have to check that it does not have a daughter with the same pdgId
          // found in the previous iteration
          GenPart(iMother).LastCopyCand = 1;
          iGrandMother = GenPart(iMother).genPartIdxMother;
          if (iGrandMother!=-1) {
            // Remove intermediate states from history (where id matches that of the mother)
            while (GenPart(iGrandMother).pdgId == GenPart(iMother).pdgId) {
              iGrandMother = GenPart(iGrandMother).genPartIdxMother;
              if (iGrandMother==-1) break;
            }
            if (iGrandMother!=-1) {
              grandMotherId = GenPart(iGrandMother).pdgId;
              // This seems a bit too much, but there are cases like: top->W->tau->ele/mu
              iGreatGrandMother = GenPart(iGrandMother).genPartIdxMother;
              if (iGreatGrandMother!=-1) {
                while (GenPart(iGreatGrandMother).pdgId == GenPart(iGrandMother).pdgId) {
                  iGreatGrandMother = GenPart(iGreatGrandMother).genPartIdxMother;
                  if (iGreatGrandMother==-1) break;
                }
                if (iGreatGrandMother!=-1) greatGrandMotherId = GenPart(iGreatGrandMother).pdgId;
              }
            }
          }
        }
      }
      // Save info
      GenPart().iMother = iMother;
      GenPart().iGrandMother = iGrandMother;
      GenPart().iGreatGrandMother = iGreatGrandMother;
      GenPart().motherId = motherId;
      GenPart().grandMotherId = grandMotherId;
      GenPart().greatGrandMotherId = greatGrandMotherId;
      if (debug>1) std::cout<<"Variables::define_genparticles_: gen 1st loop "<<GenPart.i<<" (grand/)mother ok"<<std::endl;
      
      // Check if particle is the last copy
      // gen status flags stored bitwise, bits are: 
      // 0 : isPrompt, 1 : isDecayedLeptonHadron, 2 : isTauDecayProduct, 3 : isPromptTauDecayProduct, 
      // 4 : isDirectTauDecayProduct, 5 : isDirectPromptTauDecayProduct, 6 : isDirectHadronDecayProduct, 
      // 7 : isHardProcess, 8 : fromHardProcess, 9 : isHardProcessTauDecayProduct, 
      // 10 : isDirectHardProcessTauDecayProduct, 11 : fromHardProcessBeforeFSR, 
      // 12 : isFirstCopy, 13 : isLastCopy, 14 : isLastCopyBeforeFSR
      // Good presentation from Josh Bendavid (slide 13-17)
      // https://indico.cern.ch/event/402279/contributions/960421/attachments/805964/1104514/mcaod-Jun17-2015.pdf#search=isLastCopy%20GenPart
      // isLastCopy: last copy of a given particle.  More likely, but notguaranteed, to have "final" physical kinematics
      // --> Correctly give status=1 for e/mu, and status=2 for tau
      //     But is unreliable for Ws, there we need to recursively
      //     eg.: lep -> W (final) -> W (interm) -> top
      //     isLastCopy fails O(%) for the final state W, especially for electrons (as high as 3%)
      bool isLastCopy = (GenPart().statusFlags>>13)&1;
      if (isLastCopy) {
        // Generator final state leptons
        if (GenPart.Lepton.define( std::abs(GenPart().pdgId)==11 ||
                                     std::abs(GenPart().pdgId)==13 ||
                                     std::abs(GenPart().pdgId)==15 )) {
          // Leptons from the hard process
          GenPart.LeptonFromHardProcess.define((GenPart().statusFlags>>8)&1);
          // Leptons from W
          if (GenPart.LeptonFromW.define(std::abs(motherId)==24)) {
            GenPart.LepW.pass[iMother] = 1;
            GenPart(iMother).iGenLepDaughter = GenPart.i;
            // also from leptonic top
            if (GenPart.LeptonFromTop.define(std::abs(grandMotherId)==6)) {
              GenPart.LepTop.pass[iGrandMother] = 1;
              GenPart(iGrandMother).iGenLepGrandDaughter = GenPart.i;
            }
            // also from Higgs
            if (GenPart.LeptonFromH.define(std::abs(grandMotherId)==25)) {
              GenPart.LepH.pass[iGrandMother] = 1;
              GenPart(iGrandMother).iGenLepGrandDaughter = GenPart.i;
            }
          }
          // Leptons from Z
          else if (GenPart.LeptonFromZ.define(std::abs(motherId)==23)) {
            GenPart.LepZ.pass[iMother] = 1;
            GenPart(iMother).iGenLepDaughter = GenPart.i;
            // also from Higgs
            if (GenPart.LeptonFromH.define(std::abs(grandMotherId)==25)) {
              GenPart.LepH.pass[iGrandMother] = 1;
              GenPart(iGrandMother).iGenLepGrandDaughter = GenPart.i;
            }
          }
          // Leptons from H
          else if (GenPart.LeptonFromH.define(std::abs(motherId)==25)) {
            GenPart.LepH.pass[iMother] = 1;
            GenPart(iMother).iGenLepDaughter = GenPart.i;
          }
          // Lepton from taus, here we go ... :)
          else if ( ( std::abs(GenPart().pdgId)==11 ||
                      std::abs(GenPart().pdgId)==13 ) &&
                    std::abs(motherId)==15) {
            // also lepton from W
            if (GenPart.LeptonFromW.define(std::abs(grandMotherId)==24)) {
              GenPart.LepW.pass[iGrandMother] = 1;
              GenPart(iGrandMother).iGenLepGrandDaughter = GenPart.i;
              // also lepton from top
              if (GenPart.LeptonFromTop.define(std::abs(greatGrandMotherId)==6)) {
                GenPart.LepTop.pass[iGreatGrandMother] = 1;
                GenPart(iGreatGrandMother).iGenLepGreatGrandDaughter = GenPart.i;
              }
            }
          }
        }
        // Generator neutrinos
        else if (GenPart.Nu.define 
              ( std::abs(GenPart().pdgId)==12 ||
                std::abs(GenPart().pdgId)==14 ||
                std::abs(GenPart().pdgId)==16 )) {
          // Nus from W
          if (GenPart.NuFromW.define(std::abs(motherId)==24)) {
            // also from leptonic top
            GenPart.NuFromTop.define(std::abs(grandMotherId)==6);
          }
          // Nu from taus
          else if ( ( std::abs(GenPart().pdgId)==12 ||
                      std::abs(GenPart().pdgId)==14 ) &&
                    std::abs(motherId)==15) {
            // also lepton from W
            if (GenPart.NuFromW.define(std::abs(grandMotherId)==24)) {
              // also lepton from top
              GenPart.NuFromTop.define(std::abs(greatGrandMotherId)==6);
            }
          }
        }
        // Checking distance of q/g to photons
        // This is needed for flagging fragmentation photons
        if( GenPart().status==23 && ( std::abs(GenPart().pdgId)==1 ||
                                      std::abs(GenPart().pdgId)==2 ||
                                      std::abs(GenPart().pdgId)==3 ||
                                      std::abs(GenPart().pdgId)==4 ||
                                      std::abs(GenPart().pdgId)==5 ||
                                      std::abs(GenPart().pdgId)==6 ||
                                      std::abs(GenPart().pdgId)==21) ) {
          while (Photon.Loop()) if (Photon().genPartIdx!=-1) {
            double dR_genqg = GenPart.v4(Photon().genPartIdx).DeltaR(GenPart.v4());
            if (dR_genqg<0.4) Photon().fromFrag = true;
          }
        }
        if (debug>1) std::cout<<"Variables::define_genparticles_: end direct/frag photon matching"<<std::endl;

      }
      if (debug>1) std::cout<<"Variables::define_genparticles_: gen 1st loop "<<GenPart.i<<" end"<<std::endl;
    } // End GenPart 1st loop
    if (debug) std::cout<<"Variables::define_genparticles_: end gen 1st loop"<<std::endl;
    
    // 2nd Loop, to calculate gen matched objects
    while (GenPart.Loop()) {
      if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" start"<<std::endl;
      double gen_mass = GenPart().mass;
      // mass is not stored in NanoAOD for particle masses below 10 GeV, need to look it up in PDG
      if      (std::abs(GenPart().pdgId)==1)  gen_mass = 0.005;
      else if (std::abs(GenPart().pdgId)==2)  gen_mass = 0.002;
      else if (std::abs(GenPart().pdgId)==3)  gen_mass = 0.095;
      else if (std::abs(GenPart().pdgId)==4)  gen_mass = 1.275;
      else if (std::abs(GenPart().pdgId)==5)  gen_mass = 4.180;
      else if (std::abs(GenPart().pdgId)==11) gen_mass = 0.0005;
      else if (std::abs(GenPart().pdgId)==13) gen_mass = 0.106;
      else if (std::abs(GenPart().pdgId)==15) gen_mass = 1.777;
      GenPart.v4().SetPtEtaPhiM(GenPart().pt, GenPart().eta, GenPart().phi, gen_mass);
      if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" masses ok"<<std::endl;

      // Match to AK8
      double mindR = NOVAL_F;
      while (FatJet.JetAK8.Loop()) {
        double dR = GenPart.v4().DeltaR(FatJet.JetAK8.v4());
        if (dR<0.8 && dR<mindR) {
          mindR = dR;
          GenPart().matchAK8 = true;
          GenPart().iMatchedAK8 = FatJet.JetAK8.iRef;
        }
      }
  
      // Generator leptons
      if (GenPart.Lepton.pass[GenPart.i]) {
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen lep"<<std::endl;
        // Propagate lepton info to AK8
        while (FatJet.Loop()) if (GenPart.v4().DeltaR(FatJet.v4())<0.8) {
          FatJet().matchGenLepton = true;
          if      (GenPart.LeptonFromTop.pass[GenPart.i]) FatJet().matchedGenLeptonMotherID = 6;
          else if (GenPart.LeptonFromW.  pass[GenPart.i]) FatJet().matchedGenLeptonMotherID = 24;
          else if (GenPart.LeptonFromZ  .pass[GenPart.i]) FatJet().matchedGenLeptonMotherID = 23;
          else if (GenPart.LeptonFromH  .pass[GenPart.i]) FatJet().matchedGenLeptonMotherID = 25;
          else                                            FatJet().matchedGenLeptonMotherID = GenPart().motherId;
        }

        // RECO object matching
        if (GenPart.Ele.define(std::abs(GenPart().pdgId)==11)) {
          while (Electron.Loop()) if (Electron().genPartIdx==GenPart.i) {
            Electron().matchGenEle = true;
            if (Electron.Veto  .pass[Electron.i]) GenPart().passLepVeto   = true;
            if (Electron.NoIso .pass[Electron.i]) GenPart().passLepNoIso  = true;
            if (Electron.NonIso.pass[Electron.i]) GenPart().passLepNonIso = true;
          }
        } else if (GenPart.Mu.define(std::abs(GenPart().pdgId)==13)) {
          while (Muon.Loop()) if (Muon().genPartIdx==GenPart.i) {
            Muon().matchGenMu = true;
            if (Muon.Veto  .pass[Muon.i]) GenPart().passLepVeto   = true;
            if (Muon.NoIso .pass[Muon.i]) GenPart().passLepNoIso  = true;
            if (Muon.NonIso.pass[Muon.i]) GenPart().passLepNonIso = true;
          }
        } else if (GenPart.Tau.define(std::abs(GenPart().pdgId)==15)) {
          // Unlike other leptons, the taus decay to leptons or hadrons
          // It is only fair to match to the ID of the corresponding decay product
          // for ele/mu, the matching will be done above for the tau daughters
          // The reco taus are for the hadronic decay, so require Tau_genPartFlav==5
          while (Tau.Loop()) if (Tau().genPartIdx==GenPart.i) {
            Tau().matchGenTau = true;
            if (Tau.Veto.pass[Tau.i]&&Tau().genPartFlav==5) GenPart().passLepVeto = true;
          }
        }
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" lep-reco object matching ok"<<std::endl;
  
        // leptons from the hard process
        if (GenPart.LeptonFromHardProcess.pass[GenPart.i]) {
          if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen lep from hp"<<std::endl;
          if (GenPart.EleFromHardProcess.define(std::abs(GenPart().pdgId)==11))
            while (Electron.Loop()) if (Electron().genPartIdx==GenPart.i) Electron().matchGenEleFromHardProcess = 1;
          if (GenPart.MuFromHardProcess.define(std::abs(GenPart().pdgId)==13))
            while (Muon.Loop()) if (Muon().genPartIdx==GenPart.i) Muon().matchGenMuFromHardProcess = 1;
          GenPart.TauFromHardProcess.define(std::abs(GenPart().pdgId)==15);
        }
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen lep from hard process ok"<<std::endl;
  
        // leptons from W decay
        if (GenPart.LeptonFromW.pass[GenPart.i]) {
          if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen lep from W"<<std::endl;
          if (GenPart.EleFromW.define(std::abs(GenPart().pdgId)==11))
            while (Electron.Loop()) if (Electron().genPartIdx==GenPart.i) Electron().matchGenEleFromW = 1;
          if (GenPart.MuFromW.define(std::abs(GenPart().pdgId)==13))
            while (Muon.Loop()) if (Muon().genPartIdx==GenPart.i) Muon().matchGenMuFromW = 1;
          GenPart.TauFromW.define(std::abs(GenPart().pdgId)==15);
        }
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen lep from W ok"<<std::endl;
  
        // leptons from Z decay
        if (GenPart.LeptonFromZ.pass[GenPart.i]) {
          if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen lep from Z"<<std::endl;
          if (GenPart.EleFromZ.define(std::abs(GenPart().pdgId)==11))
            while (Electron.Loop()) if (Electron().genPartIdx==GenPart.i) Electron().matchGenEleFromZ = 1;
          if (GenPart.MuFromZ.define(std::abs(GenPart().pdgId)==13))
            while (Muon.Loop()) if (Muon().genPartIdx==GenPart.i) Muon().matchGenMuFromZ = 1;
          GenPart.TauFromZ.define(std::abs(GenPart().pdgId)==15);
        }
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen lep from Z ok"<<std::endl;
  
        // leptons from Higgs decay
        if (GenPart.LeptonFromH.pass[GenPart.i]) {
          if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen lep from H"<<std::endl;
          if (GenPart.EleFromH.define(std::abs(GenPart().pdgId)==11))
            while (Electron.Loop()) if (Electron().genPartIdx==GenPart.i) Electron().matchGenEleFromH = 1;
          if (GenPart.MuFromH.define(std::abs(GenPart().pdgId)==13))
            while (Muon.Loop()) if (Muon().genPartIdx==GenPart.i) Muon().matchGenMuFromH = 1;
          GenPart.TauFromH.define(std::abs(GenPart().pdgId)==15);
        }
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen lep from Higgs ok"<<std::endl;
  
        // leptons from top decay
        if (GenPart.LeptonFromTop.pass[GenPart.i]) {
          if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen lep from top"<<std::endl;
          if (GenPart.EleFromTop.define(std::abs(GenPart().pdgId)==11))
            while (Electron.Loop()) if (Electron().genPartIdx==GenPart.i) Electron().matchGenEleFromTop = 1;
          if (GenPart.MuFromTop.define(std::abs(GenPart().pdgId)==13))
            while (Muon.Loop()) if (Muon().genPartIdx==GenPart.i) Muon().matchGenMuFromTop = 1;
          GenPart.TauFromTop.define(std::abs(GenPart().pdgId)==15);
        }
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen lep from top ok"<<std::endl;
      }
      if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen leptons ok"<<std::endl;
  
      // Generator neutrinos
      if (GenPart.Nu.pass[GenPart.i]) {
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen nu"<<std::endl;
        if (GenPart.EleNu.define(std::abs(GenPart().pdgId)==12)) {
          GenPart.EleNuFromW  .define(GenPart.NuFromW  .pass[GenPart.i]);
          GenPart.EleNuFromTop.define(GenPart.NuFromTop.pass[GenPart.i]);
          double dR_min = NOVAL_F;
          size_t iMatch = -1;
          while (Electron.Loop()) {
            double dR = Electron().nu.DeltaR(GenPart.v4());
            if (dR<0.4 && dR<dR_min) { iMatch = Electron.i; dR_min = dR; }
          }
          if (dR_min<0.4) {
            Electron(iMatch).nuMatchGenEleNu = 1;
            if (GenPart.NuFromW  .pass[GenPart.i]) Electron(iMatch).nuMatchGenEleNuFromW   = 1;
            if (GenPart.NuFromTop.pass[GenPart.i]) Electron(iMatch).nuMatchGenEleNuFromTop = 1;
          }
        }
        if (GenPart.MuNu.define(std::abs(GenPart().pdgId)==14)) {
          GenPart.MuNuFromW  .define(GenPart.NuFromW  .pass[GenPart.i]);
          GenPart.MuNuFromTop.define(GenPart.NuFromTop.pass[GenPart.i]);
          double dR_min = NOVAL_F;
          size_t iMatch = -1;
          while (Muon.Loop()) {
            double dR = Muon().nu.DeltaR(GenPart.v4());
            if (dR<0.4 && dR<dR_min) { iMatch = Muon.i; dR_min = dR; }
          }
          if (dR_min<0.4) {
            Muon(iMatch).nuMatchGenMuNu = 1;
            if (GenPart.NuFromW  .pass[GenPart.i]) Muon(iMatch).nuMatchGenMuNuFromW   = 1;
            if (GenPart.NuFromTop.pass[GenPart.i]) Muon(iMatch).nuMatchGenMuNuFromTop = 1;
          }
        }
        if (GenPart.TauNu.define(std::abs(GenPart().pdgId)==16)) {
          GenPart.TauNuFromW  .define(GenPart.NuFromW  .pass[GenPart.i]);
          GenPart.TauNuFromTop.define(GenPart.NuFromTop.pass[GenPart.i]);
        }
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen nu ok"<<std::endl;
      }
      if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gen neutrinos ok"<<std::endl;
  
      // Prompt photons (from gq mother)
      if(GenPart.PromptPhoton.define(std::abs(GenPart().pdgId) == 22 && 
                                     ( std::abs(GenPart().motherId)==1 ||
                                       std::abs(GenPart().motherId)==2 ||
                                       std::abs(GenPart().motherId)==3 ||
                                       std::abs(GenPart().motherId)==4 ||
                                       std::abs(GenPart().motherId)==5 ||
                                       std::abs(GenPart().motherId)==6 ||
                                       std::abs(GenPart().motherId)==21 ))) {
        // RECO matching
        while (Photon.Loop()) if (Photon().genPartIdx == GenPart.i) {
          Photon().matchGenFake         = 0;
          Photon().matchGenPrompt       = 1;
          GenPart.PromptDirectPhoton.define((Photon().matchGenPromptDirect = !Photon().fromFrag));
          GenPart.PromptFragPhoton  .define((Photon().matchGenPromptFrag   =  Photon().fromFrag));
        }
      }
      if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" prompt photons ok"<<std::endl;
      
      // Final non-decayed state particles
      if ((GenPart().FinalState = GenPart().status==1 || GenPart().status==2 || (GenPart().LastCopyCand && GenPart().NoSameDaughter))) {
        // gen bs
        GenPart.b.define(std::abs(GenPart().pdgId)==5);
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" final state bs ok"<<std::endl;
        
        // gen Ws
        // Consider hadronically decaying Ws and match them to AK8 jet
        if (GenPart.W.define(std::abs(GenPart().pdgId)==24)) {
          if (GenPart.HadW.define(!GenPart.LepW.define(GenPart.LepW.pass[GenPart.i]))) {
            if (GenPart().matchAK8) {
              FatJet(GenPart().iMatchedAK8).matchGenHadW = true;
              GenPart().passHadWTag = FatJet.HadW.pass[GenPart().iMatchedAK8];
            }
          }
        }
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" final state Ws ok"<<std::endl;
        
        // gen Zs
        // Consider hadronically decaying Zs and match them to AK8 jet
        if (GenPart.Z.define(std::abs(GenPart().pdgId)==23)) {
          if (GenPart.HadZ.define(!GenPart.LepZ.define(GenPart.LepZ.pass[GenPart.i]))) {
            
            if (GenPart().matchAK8) {
              FatJet(GenPart().iMatchedAK8).matchGenHadZ = true;
              GenPart().passHadZTag = FatJet.HadZ.pass[GenPart().iMatchedAK8];
            }
          }
        }
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" final state Zs ok"<<std::endl;
        
        // gen Higgs
        // Consider hadronically decaying Higgs and match it to AK8 jet
        if (GenPart.H.define(std::abs(GenPart().pdgId)==25)) {
          if (GenPart.HadH.define(!GenPart.LepH.define(GenPart.LepH.pass[GenPart.i]))) {
            if (GenPart().matchAK8) {
              FatJet(GenPart().iMatchedAK8).matchGenHadH = true;
              GenPart().passHadHTag = FatJet.HadH.pass[GenPart().iMatchedAK8];
            }
          }
        }
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" final state Higgs' k"<<std::endl;
        
        // gen tops
        if (GenPart.Top.define(std::abs(GenPart().pdgId)==6)) {
          if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gentop start"<<std::endl;
          if (GenPart.LepTop.define(GenPart.LepTop.pass[GenPart.i])) {
            if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gentop lep start"<<std::endl;
            if (GenPart().matchAK8) {
              FatJet(GenPart().iMatchedAK8).matchGenLepTop = true;
              GenPart().passLepTopTag = FatJet.LepTop.pass[GenPart().iMatchedAK8];
            }
            if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gentop lep ok"<<std::endl;
          } else {
            if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gentop had start"<<std::endl;
            GenPart.LepTop.define(1);
            if (GenPart().matchAK8) {
              FatJet(GenPart().iMatchedAK8).matchGenHadTop = true;
              GenPart().passHadTopTag = FatJet.HadTop.pass[GenPart().iMatchedAK8];
            }
            if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" gentop had ok"<<std::endl;
          }
        }
        if (debug>1) std::cout<<"Variables::define_genparticles_: gen 2nd loop "<<GenPart.i<<" final state tops ok"<<std::endl;
      } // End FinalState

    } // End GenPart 2nd loop
    if (debug) std::cout<<"Variables::define_genparticles_: end gen 2nd loop"<<std::endl;

  }

  std::vector<TLorentzVector> megajets;
  std::vector<TLorentzVector> megajets_nophoton;
  std::vector<TLorentzVector> saved_megajets;
  std::vector<TLorentzVector> saved_megajets_nophoton;
  std::vector<TLorentzVector> newmegajets;
  std::vector<TLorentzVector> newmegajets_nophoton;
  std::vector<TLorentzVector> saved_newmegajets;
  std::vector<TLorentzVector> saved_newmegajets_nophoton;
  
  void define_event_(const unsigned int& syst_index, int debug = 0) {
    initEventData();
    
    // Lepton selections combined  
    nLepVetoNoIso = Electron.VetoNoIso.n + Muon.VetoNoIso.n;
    nLepVeto      = Electron.Veto.n      + Muon.Veto.n;
    nLepSelect    = Electron.Select.n    + Muon.Select.n;
    nLepTight     = Electron.Tight.n     + Muon.Tight.n;
    nLepNoIso     = Electron.NoIso.n     + Muon.NoIso.n;
    nLepNonIso    = Electron.NonIso.n    + Muon.NonIso.n;

    std::vector<TLorentzVector> noniso_leptons;
    while (Electron.NonIso.Loop()) noniso_leptons.push_back(Electron.NonIso.v4());
    while (Muon.NonIso.Loop())     noniso_leptons.push_back(Muon.NonIso.v4());

    if (FatJet.LepTop.n>0) lepNeutrinoDR = FatJet.LepTop(0).lepNonIsoNuDR;

    // MET variables
    double MET_px = MET_pt*std::cos(MET_phi);
    double MET_py = MET_pt*std::sin(MET_phi);
    MET.SetXYZ(MET_px, MET_py, 0);
    
    // -----------------------------------------------
    //              Lepton/Phton + MET

    // MT (l, MET), MET + 1l
    TVector3 MET_1vl(MET);
    if (Electron.Veto.n==1&&Muon.Veto.n==0) {
      MET_1vl += TVector3(Electron.Veto.v4(0).Px(),Electron.Veto.v4(0).Py(),0);
      MT_lepVeto = sqrt( 2*Electron.Veto(0).pt*MET_pt * (1 - std::cos(MET_phi-Electron.Veto(0).phi)) );
    } else if (Electron.Veto.n==0&&Muon.Veto.n==1) {
      MET_1vl += TVector3(Muon.Veto.v4(0).Px(),Muon.Veto.v4(0).Py(),0);
      MT_lepVeto = sqrt( 2*Muon.    Veto(0).pt*MET_pt * (1 - std::cos(MET_phi-Muon.    Veto(0).phi)) );
    }
    TVector3 MET_1l (MET);
    if (Electron.Select.n==1&&Muon.Select.n==0) {
      MET_1l += TVector3(Electron.Select.v4(0).Px(),Electron.Select.v4(0).Py(),0);
      MT = sqrt( 2*Electron.Select(0).pt*MET_pt * (1 - std::cos(MET_phi-Electron.Select(0).phi)) );
    } else if (Electron.Select.n==0&&Muon.Select.n==1) {
      MET_1l += TVector3(Muon.Select.v4(0).Px(),Muon.Select.v4(0).Py(),0);
      MT = sqrt( 2*Muon.    Select(0).pt*MET_pt * (1 - std::cos(MET_phi-Muon.    Select(0).phi)) );
    }
    if (Electron.Tight.n==1&&Muon.Tight.n==0) {
      MT_lepTight = sqrt( 2*Electron.Tight(0).pt*MET_pt * (1 - std::cos(MET_phi-Electron.Tight(0).phi)) );
    } else if (Electron.Tight.n==0&&Muon.Tight.n==1) {
      MT_lepTight = sqrt( 2*Muon.    Tight(0).pt*MET_pt * (1 - std::cos(MET_phi-Muon.    Tight(0).phi)) );
    }
    if (Electron.NonIso.n==1&&Muon.NonIso.n==0) {
      MT_lepNonIso = sqrt( 2*Electron.NonIso(0).pt*MET_pt * (1 - std::cos(MET_phi-Electron.NonIso(0).phi)) );
    } else if (Electron.NonIso.n==0&&Muon.NonIso.n==1) {
      MT_lepNonIso = sqrt( 2*Muon.    NonIso(0).pt*MET_pt * (1 - std::cos(MET_phi-Muon.    NonIso(0).phi)) );
    }
    // M(2l), dPhi(2l, MET), MET + 2l
    TVector3 MET_2l (MET);
    TLorentzVector lep_pair;
    if (Electron.Select.n==2&&Muon.Veto.n==0) {
      MET_2l += TVector3(Electron.Select.v4(0).Px(), Electron.Select.v4(0).Py(), 0);
      MET_2l += TVector3(Electron.Select.v4(1).Px(), Electron.Select.v4(1).Py(), 0);
      lep_pair = Electron.Select.v4(0)+Electron.Select.v4(1);
      M_2l = lep_pair.M();
      dPhi_2l_met = std::abs(TVector2::Phi_mpi_pi(lep_pair.Phi() - MET_phi));
    } else if (Electron.Veto.n==0&&Muon.Select.n==2) {
      MET_2l += TVector3(Muon.Select.v4(0).Px(), Muon.Select.v4(0).Py(), 0);
      MET_2l += TVector3(Muon.Select.v4(1).Px(), Muon.Select.v4(1).Py(), 0);
      lep_pair = Muon.Select.v4(0)+Muon.Select.v4(1);
      M_2l = lep_pair.M();
      dPhi_2l_met = std::abs(TVector2::Phi_mpi_pi(lep_pair.Phi() - MET_phi));
    }
  
    // Add the (pre-)selected/fake photon to MET
    TVector3 MET_pho    (MET);
    TVector3 MET_fakepho(MET);
    if      (Photon.Select.n==1)    MET_pho     += TVector3(Photon.Select   .v4(0).Px(), Photon.Select   .v4(0).Py(), 0);
    else if (Photon.PreSelect.n==1) MET_pho     += TVector3(Photon.PreSelect.v4(0).Px(), Photon.PreSelect.v4(0).Py(), 0);
    if      (Photon.Fake.n==1)      MET_fakepho += TVector3(Photon.Fake     .v4(0).Px(), Photon.Fake     .v4(0).Py(), 0);
    if (debug) std::cout<<"Variables::define_jets_: end met + lep/pho variables"<<std::endl;

    // -----------------------------------------------
    //                    Jet/MET

    // Online HT
    while (Jet.Loop())    if (Jet().   pt> 40 && std::abs(Jet().   eta)<3.0) AK4_HtOnline += Jet().pt;
    while (FatJet.Loop()) if (FatJet().pt>150 && std::abs(FatJet().eta)<2.5) AK8_Ht       += FatJet().pt;
    while (Jet.Jet.Loop()) {
      if (debug>1) std::cout<<"Variables::define_event_variables_: Jet "<<Jet.Jet.i<<" min dphi start"<<std::endl;
      // AK4 HT
      AK4_Ht += Jet.Jet().pt;

      // minDeltaPhi
      if (Jet.Jet.i<4) {
        double dphi = std::abs(TVector2::Phi_mpi_pi(MET_phi - Jet.Jet().phi));
        if (dphi<minDeltaPhi) minDeltaPhi = dphi;
        // with added (veto) lepton
        double dphi_met1vl = std::abs(TVector2::Phi_mpi_pi(MET_1vl.Phi() - Jet.Jet().phi));
        if (dphi_met1vl<minDeltaPhi_1vl) minDeltaPhi_1vl = dphi_met1vl;
        double dphi_met1l = std::abs(TVector2::Phi_mpi_pi(MET_1l.Phi() - Jet.Jet().phi));
        if (dphi_met1l<minDeltaPhi_1l) minDeltaPhi_1l = dphi_met1l;
        // with added lepton pair
        double dphi_met2l = std::abs(TVector2::Phi_mpi_pi(MET_2l.Phi() - Jet.Jet().phi));
        if (dphi_met2l<minDeltaPhi_2l) minDeltaPhi_2l = dphi_met2l;
        // with added photon
        double dphi_metpho = std::abs(TVector2::Phi_mpi_pi(MET_pho.Phi() - Jet.Jet().phi));
        if (dphi_metpho<minDeltaPhi_pho) minDeltaPhi_pho = dphi_metpho;
        // jet lep-pair angle
        if (M_2l!=-NOVAL_F) {
          double dphi_2l = std::abs(TVector2::Phi_mpi_pi(lep_pair.Phi() - Jet.Jet().phi));
          if (dphi_2l<dPhi_2l_jet) dPhi_2l_jet = dphi_2l;
        }
      }
      if (debug>1) std::cout<<"Variables::define_event_variables_: Jet "<<Jet.Jet.i<<" min dphi ok"<<std::endl;
    }

    std::vector<size_t> iJet_Boost;
    double maxpt = 0;
    while (FatJet.Loop()) {
      if ( FatJet.HadW.pass[FatJet.i] ||
           FatJet.HadZ.pass[FatJet.i] ||
           FatJet.HadH.pass[FatJet.i] ||
           FatJet.HadTop.pass[FatJet.i] ||
           FatJet.LepJet.pass[FatJet.i] ||
           FatJet.LepTop.pass[FatJet.i]) {
        if (debug>1) std::cout<<"Variables::define_event_variables_: FatJet "<<FatJet.i<<" boost start"<<std::endl;
        // lep/MET angle wrt. leading boost jet
        if (FatJet().pt>maxpt) {
          if (debug>1) std::cout<<"Variables::define_event_variables_: FatJet "<<FatJet.i<<" maxpt start"<<std::endl;
          maxpt = FatJet().pt;
          dPhiBoostedJetMET = std::abs(TVector2::Phi_mpi_pi(MET_phi - FatJet().phi));
          TVector3 MET_nl(MET);
          if (noniso_leptons.size()==1) {
            MET_nl += TVector3(noniso_leptons[0].Px(), noniso_leptons[0].Py(), 0);
            dPhiBoostedJetLep    = std::abs(TVector2::Phi_mpi_pi(FatJet().phi) - noniso_leptons[0].Phi());
            dPhiBoostedJetLepMET = std::abs(TVector2::Phi_mpi_pi(FatJet().phi) - MET_nl.Phi());
            if (debug>1) std::cout<<"Variables::define_event_variables_: FatJet "<<FatJet.i<<" 1lep end"<<std::endl;
          } else if (noniso_leptons.size()==2) {
            if (noniso_leptons[0].Pt()>noniso_leptons[1].Pt()) {
              MET_nl += TVector3(noniso_leptons[0].Px(), noniso_leptons[0].Py(), 0);
              dPhiBoostedJetLep    = std::abs(TVector2::Phi_mpi_pi(noniso_leptons[0].Phi() - FatJet().phi));
            } else {
              MET_nl += TVector3(noniso_leptons[1].Px(), noniso_leptons[1].Py(), 0);
              dPhiBoostedJetLep    = std::abs(TVector2::Phi_mpi_pi(noniso_leptons[1].Phi() - FatJet().phi));
            }
            dPhiBoostedJetLepMET = std::abs(TVector2::Phi_mpi_pi(MET_nl.Phi() - FatJet().phi));
          }
          // MT calculated for leading boosted object and MET
          MT_boost = sqrt( 2*FatJet().pt*MET_pt * (1 - std::cos(MET_phi-FatJet().phi)) );
          if (debug>1) std::cout<<"Variables::define_event_variables_: FatJet "<<FatJet.i<<" maxpt end"<<std::endl;
        }
        if (debug>1) std::cout<<"Variables::define_event_variables_: FatJet "<<FatJet.i<<" dphi end"<<std::endl;
        // Match to nearest AK4 jet to be able to tell which megajet the boosted object belongs to
        double mindR = NOVAL_F;
        size_t iJet = -1;
        while (Jet.Jet.Loop()) {
          double dR = FatJet.v4().DeltaR(Jet.Jet.v4());
          if (dR<mindR) { mindR = dR; iJet = Jet.Jet.i; }
        }
        iJet_Boost.push_back(iJet);
        if (debug>1) std::cout<<"Variables::define_event_variables_: FatJet "<<FatJet.i<<" boost end"<<std::endl;
      }
    }

    // (Re-)calculation of Razor variables
    // Has to be (re-)done after Jet or MET systematics are applied
    // Get selected AK4 jets (input for megajets)
    // Calculating megajets is a CPU intensive task
    // So we only do it when really necessary (pt of jets changed)
    // And save them if no systematics are applied on jets
    if (syst_index==0) {
      saved_megajets.clear();
      saved_megajets_nophoton.clear();
      //    saved_newmegajets.clear();
    }
    if (syst_index==0 || recalc_megajets==1) {
      if (debug) std::cout<<"Variables::define_event_variables_: calc megajets start"<<std::endl;
      // Get input jets
      std::vector<TLorentzVector> selected_jets, nophoton_jets;
      while (Jet.Jet.Loop()) selected_jets.emplace_back(Jet.Jet.v4());
      if (Photon.Select.n==1||Photon.PreSelect.n==1)
        while (Jet.JetNoPho.Loop()) nophoton_jets.emplace_back(Jet.JetNoPho.v4());
      // Add any isolated/non-isolated lepton, which is (not part of any jet)
      while (Electron.Loop()) if (!Electron().isPartOfJet&&(Electron.Select.pass[Electron.i]||Electron.NonIso.pass[Electron.i])) 
        selected_jets.push_back(Electron.v4());
      while (Muon.Loop()) if (!Muon().isPartOfJet&&(Muon.Select.pass[Muon.i]||Muon.NonIso.pass[Muon.i])) 
        selected_jets.push_back(Muon.v4());
      if (debug) std::cout<<"Variables::define_event_variables_: calc megajets - collect jets ok"<<std::endl;
      // Calculate megajets (standard method)
      if (selected_jets.size()>=2) megajets = Razor::CombineJets(selected_jets);
      else megajets.clear();
      if (Photon.Select.n==1||Photon.PreSelect.n==1) {
        if (nophoton_jets.size()>=2) megajets_nophoton = Razor::CombineJets(nophoton_jets);
        else megajets_nophoton.clear();
      } else megajets_nophoton = megajets;
      // Save result for when we revert back to no-systematics
      if (syst_index==0) {
        saved_megajets = megajets;
        saved_megajets_nophoton = megajets_nophoton;
      }
      if (debug) std::cout<<"Variables::define_event_variables_: calc megajets - find megajets ok"<<std::endl;
      // Calculate megajets (new method)
      // Force boosted objects to be located on separate megajets
      //    if (selected_jets.size()>=2) newmegajets = Razor::CombineJetsNew(selected_jets, iJet_Boost);
      //    else newmegajets.clear();
      //    // Save result
      //    if (syst_index==0) saved_newmegajets = newmegajets;
      if (debug) std::cout<<"Variables::define_event_variables_: calc megajets - find megajets (new) ok"<<std::endl;
    } else {
      // Load previously calculated result
      megajets = saved_megajets;
      megajets_nophoton = saved_megajets_nophoton;
      //    newmegajets = saved_newmegajets;
    }
  
    // Recalculate Razor (also with 1/2lep or pho added to MET)
    if (megajets.size()==2) {
      // Normal Razor
      MR  = Razor::CalcMR(megajets[0], megajets[1]);
      MTR = Razor::CalcMTR(megajets[0], megajets[1], MET);
      R2  = (MTR/MR)*(MTR/MR);
      dPhiRazor = std::abs(TVector2::Phi_mpi_pi(megajets[0].Phi() - megajets[1].Phi()));
      // MET variatons (MET + 1/2 lepton)
      // 1 (selected) lepton added
      if (nLepSelect==1) {
        MTR_1l = Razor::CalcMTR(megajets[0], megajets[1], MET_1l);
        R2_1l  = (MTR_1l/MR)*(MTR_1l/MR);
      }
      // 1 veto lepton added (default)
      if (nLepVeto==1) {
        MTR_1vl = Razor::CalcMTR(megajets[0], megajets[1], MET_1vl);
        R2_1vl  = (MTR_1vl/MR)*(MTR_1vl/MR);
      }
      // 2 leptons added
      if (M_2l!=-NOVAL_F) {
        MTR_2l = Razor::CalcMTR(megajets[0], megajets[1], MET_2l);
        R2_2l  = (MTR_2l/MR)*(MTR_2l/MR);
      }
    }
    // Remove photon from both jet collections and add to MET
    MR_pho  = MR;
    MTR_pho = MTR;
    R2_pho  = R2;
    if (megajets_nophoton.size()==2) {
      MR_pho  = Razor::CalcMR(megajets_nophoton[0], megajets_nophoton[1]);
      MTR_pho = Razor::CalcMTR(megajets_nophoton[0], megajets_nophoton[1], MET_pho);
      R2_pho  = (MTR_pho/MR)*(MTR_pho/MR);
      dPhiRazorNoPho = std::abs(TVector2::Phi_mpi_pi(megajets_nophoton[0].Phi() - megajets_nophoton[1].Phi()));
    }
    // Razor calculated from 2 boosted objects
    //if (FatJet.HadW.n==2) {
    //  MR  = Razor::CalcMR (FatJet.HadW.v4(0), FatJet.HadW.v4(1));
    //  MTR = Razor::CalcMTR(FatJet.HadW.v4(0), FatJet.HadW.v4(1), MET);
    //  R2  = (MTR/MR)*(MTR/MR);
    //  dPhiRazor = std::abs(TVector2::Phi_mpi_pi(FatJet.HadW(0).phi - FatJet.HadW(1).phi));
    //}
    // Forcing boosted objects to be in separate megajets
    //    if (newmegajets.size()==2) {
    //      MR_new  = Razor::CalcMR(newmegajets[0], newmegajets[1]);
    //      MTR_new = Razor::CalcMTR(newmegajets[0], newmegajets[1], MET);
    //      R2_new  = (MTR_new/MR_new)*(MTR_new/MR_new);
    //      dPhiRazor_new = std::abs(TVector2::Phi_mpi_pi(newmegajets[0].Phi() - newmegajets[1].Phi()));
    //    }
    if (debug) std::cout<<"Variables::define_event_variables_: calc razor ok"<<std::endl;

    if (debug) std::cout<<"Variables::define_event_variables_: end"<<std::endl;
  } // End define_event_variables_


}; // End Variables class definition

#endif // End header guard