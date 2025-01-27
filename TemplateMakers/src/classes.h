#include "EFTMultilepton/TemplateMakers/interface/WCPoint.h"
#include "EFTMultilepton/TemplateMakers/interface/WCFit.h"
#include "EFTMultilepton/TemplateMakers/interface/TH1EFT.h"
#include "EFTMultilepton/TemplateMakers/interface/objectClasses.h"

#ifndef TTH_DUMMY_OBJS
#define TTH_DUMMY_OBJS

namespace {
  struct dictionary {
    ttH::Lepton dummy_lepton;
    std::vector<ttH::Lepton> dummy_lepton_vector;
    std::vector< std::vector<ttH::Lepton> > dummy_lepton_vector_vector;

    ttH::Electron dummy_electron;
    std::vector<ttH::Electron> dummy_electron_vector;
    std::vector< std::vector<ttH::Electron> > dummy_electron_vector_vector;

    ttH::Muon dummy_muon;
    std::vector<ttH::Muon> dummy_muon_vector;
    std::vector< std::vector<ttH::Muon> > dummy_muon_vector_vector;

    ttH::Tau dummy_tau;
    std::vector<ttH::Tau> dummy_tau_vector;
    std::vector< std::vector<ttH::Tau> > dummy_tau_vector_vector;

    ttH::Jet dummy_jet;
    std::vector<ttH::Jet> dummy_jet_vector;
    std::vector< std::vector<ttH::Jet> > dummy_jet_vector_vector;

    ttH::MET dummy_MET;
    std::vector<ttH::MET> dummy_MET_vector;
    std::vector< std::vector<ttH::MET> > dummy_MET_vector_vector;

    ttH::GenParticle dummy_genparticle;
    std::vector<ttH::GenParticle> dummy_genparticle_vector;
    std::vector< std::vector<ttH::GenParticle> > dummy_genparticle_vector_vector;

    //std::map<std::pair<std::pair<std::string,std::string>,std::pair<std::string,std::string>>,double> dummy_errormap;
    //WCPoint dummy_wcpoint;
    //WCFit dummy_wcfit;
    //TH1EFT dummy_TH1EFT;
    ////TH1EFT dummy_TH1EFTconst("dummmy","instance of TH1D constructor",10,0,10);

  };
}

#endif
