#ifndef _DataDrivenFRLepCut_h
#define _DataDrivenFRLepCut_h

#include "ttHMultileptonAnalysis/TemplateMakers/interface/KinematicVariable.h"
#include "ttHMultileptonAnalysis/TemplateMakers/interface/BranchInfo.h"
#include <typeinfo>

template <class collectionType>
class DataDrivenFRLepCut: public KinematicVariable<double> {

public:

  //Store branch values so they are accessible to other classes
  vector<BranchInfo<double>> myVars;

  collectionType **selCollection;
  unsigned int number_of_leptons;
  int working_point;
  string file_name_NP;
  string file_name_QF;
  string branchNameNP;
  string branchNameQF;
  string label;
  collectionType **selCollection_2;
  unsigned int number_of_leptons_2;
  int working_point_2;

  //TFile * weight_file_NP;
  TFile * weight_file_QF;
  //TH2 * FR_NP_loose_mu; //FR for < 2 b-jets
  //TH2 * FR_NP_loose2_mu; //FR for >= 2 b-jets
  //TH2 * FR_NP_loose_el; //FR for < 2 b-jets
  //TH2 * FR_NP_loose2_el; //FR for >= 2 b-jets
  //TH2 * FR_NP_tight_mu; //FR for < 2 b-jets
  //TH2 * FR_NP_tight2_mu; //FR for >= 2 b-jets
  //TH2 * FR_NP_tight_el; //FR for < 2 b-jets
  //TH2 * FR_NP_tight2_el; //FR for >= 2 b-jets
  TH2 * FR_QF_el; //Charge flip FR
  double FR_QF[6];
  int QF_charge[6];

  DataDrivenFRLepCut(collectionType **_selCollection, int _number_of_leptons,
               int _working_point, string _file_name_NP, string _file_name_QF, string _label,
               collectionType **_selCollection_2, int _number_of_leptons_2, int _working_point_2);
  ~DataDrivenFRLepCut();
  void evaluate();

  int computeLepCut2(double _pt, int _isElectron, int _isMuon,
                     double _jetBTagCSV, double _chargedHadronIsoDR04,
                     double _IP, double _SIP, double _jetPtRatio);
  
};

template <class collectionType>
DataDrivenFRLepCut<collectionType>::DataDrivenFRLepCut(collectionType **_selCollection, int _number_of_leptons,
                                           int _working_point, string _file_name_NP, string _file_name_QF, string _label = "",
                                           collectionType **_selCollection_2 = 0, int _number_of_leptons_2 = 0, int _working_point_2 = -99):
  selCollection(_selCollection), number_of_leptons(_number_of_leptons),
  working_point(_working_point), file_name_NP(_file_name_NP), file_name_QF(_file_name_QF),label(_label),
  selCollection_2(_selCollection_2), number_of_leptons_2(_number_of_leptons_2), working_point_2(_working_point_2) {

  this->resetVal = 1.0;
  branchNameNP = Form("DataDrivenFRLepCut_NP%s", label.c_str());
  branchNameQF = Form("DataDrivenFRLepCut_QF%s", label.c_str());
  branches[branchNameNP] = BranchInfo<double>(branchNameNP);
  branches[branchNameQF] = BranchInfo<double>(branchNameQF);
  branches[branchNameNP].branchVal = 1.0;
  branches[branchNameQF].branchVal = 1.0;

  string directory = (string(getenv("CMSSW_BASE"))+"/src/ttHMultileptonAnalysis/TemplateMakers/data/CERN/fakerate/").c_str();
//   TString weight_file_name_NP = Form("%s%s.root", directory.c_str(), file_name_NP.c_str());
//   weight_file_NP = TFile::Open(weight_file_name_NP);

//   FR_NP_loose_mu = (TH2*)weight_file_NP->Get("FR_loose_mu")->Clone();
//   FR_NP_loose2_mu = (TH2*)weight_file_NP->Get("FR_loose2_mu")->Clone();
//   FR_NP_loose_el = (TH2*)weight_file_NP->Get("FR_loose_el")->Clone();
//   FR_NP_loose2_el = (TH2*)weight_file_NP->Get("FR_loose2_el")->Clone();
//   FR_NP_tight_mu = (TH2*)weight_file_NP->Get("FR_tight_mu")->Clone();
//   FR_NP_tight2_mu = (TH2*)weight_file_NP->Get("FR_tight2_mu")->Clone();
//   FR_NP_tight_el = (TH2*)weight_file_NP->Get("FR_tight_el")->Clone();
//   FR_NP_tight2_el = (TH2*)weight_file_NP->Get("FR_tight2_el")->Clone();

  TString weight_file_name_QF = Form("%s%s.root", directory.c_str(), file_name_QF.c_str());
  weight_file_QF = TFile::Open(weight_file_name_QF);
  FR_QF_el = (TH2*)weight_file_QF->Get("QF_el_data")->Clone();


}

template <class collectionType>
void DataDrivenFRLepCut<collectionType>::evaluate() {
  if (this->evaluatedThisEvent) return;
  evaluatedThisEvent = true;

  //--------
  FR_QF[0] = 1.0; FR_QF[1] = 1.0; FR_QF[2] = 1.0; FR_QF[3] = 1.0; FR_QF[4] = 1.0; FR_QF[5] = 1.0;
  QF_charge[0] = 0; QF_charge[1] = 0; QF_charge[2] = 0; QF_charge[3] = 0; QF_charge[4] = 0; QF_charge[5] = 0;
  //int num_fail = 0;
  int num_passed_leptons = 0;
  int num_electrons = 0;
  float weight = -1.0;

  for (unsigned int iObj = 0; iObj < (*selCollection)->size(); iObj++) {
    if (iObj < number_of_leptons) {

      double lep_pt = ptr((*selCollection)->at(iObj))->pt;
      double lep_eta = abs(ptr((*selCollection)->at(iObj))->eta);
      int lepCut = -2;
      lepCut = computeLepCut2(ptr((*selCollection)->at(iObj))->pt,
                              ptr((*selCollection)->at(iObj))->isElectron,
                              ptr((*selCollection)->at(iObj))->isMuon,
                              ptr((*selCollection)->at(iObj))->jetBTagCSV,
                              ptr((*selCollection)->at(iObj))->chargedHadronIsoDR04,
                              ptr((*selCollection)->at(iObj))->IP,
                              ptr((*selCollection)->at(iObj))->SIP,
                              ptr((*selCollection)->at(iObj))->jetPtRatio);
      //TH2 * FR_NP_histo = 0;

      if (lepCut < working_point) {
        double FR = 0.0;
        if (ptr((*selCollection)->at(iObj))->isElectron) {
          if (working_point == 2) {
            if (lep_pt > 40) FR = 0.15; //0.12;
            else if (lep_pt > 20) FR = 0.15; //0.16;
            else FR = 0.15; } //0.16; }
          else if (working_point == 1) FR = 0.69;
          else std::cout << "Error: invalid lepCut FR working point = " << working_point << std::endl;
        }
        else if (ptr((*selCollection)->at(iObj))->isMuon) {
          if (working_point == 2) {
            if (lep_pt > 30) FR = 0.36; //0.40;
            else if (lep_pt > 20) FR = 0.21; //0.34;
            else if (lep_pt > 15) FR = 0.20; //0.26;
            else FR = 0.17; } //0.18; }
          else if (working_point == 1) FR = 0.51;
          else std::cout << "Error: invalid lepCut FR working point = " << working_point << std::endl;
        }
        else std::cout << "Error: lepton is neither muon nor electron" << std::endl;

        weight *= -FR/(1.0-FR);
        if ((weight == 1 && (FR <= 0 || FR >= 1)) || weight == 0) {
          std::cout << "Error: DataDrivenFRLepCut weight = " << weight << std::endl;
          std::cout << "WP = " << working_point << ", lepCut == " << lepCut << ", FR = " << FR << std::endl; }
      }
      //else { //Compute QF for all electrons, not just tight electrons
      QF_charge[num_passed_leptons] = (ptr((*selCollection)->at(iObj)))->tkCharge;
      num_passed_leptons += 1;
      if (ptr((*selCollection)->at(iObj))->isElectron == 1) {
        int pt_bin  = std::max(1, std::min(FR_QF_el->GetNbinsX(), FR_QF_el->GetXaxis()->FindBin(lep_pt)));
        int eta_bin  = std::max(1, std::min(FR_QF_el->GetNbinsY(), FR_QF_el->GetYaxis()->FindBin(lep_eta)));
        FR_QF[num_electrons] = FR_QF_el->GetBinContent(pt_bin, eta_bin);
        QF_charge[num_passed_leptons] = ((BNelectron*)ptr((*selCollection)->at(iObj)))->tkCharge;
        num_electrons += 1;
        if (FR_QF_el->GetBinContent(pt_bin, eta_bin) == 1 || FR_QF_el->GetBinContent(pt_bin, eta_bin) == 0) {
          std::cout << "Error: QF SF = " << FR_QF_el->GetBinContent(pt_bin, eta_bin) <<
            ", pt = " << lep_pt << ", eta = " << lep_eta << ", pt_bin = " << pt_bin << ", eta_bin = " << eta_bin << std::endl;
        }
      }
        //}
    } //end if ( iObj < number_of_leptons )
  } //end loop over iObj

  if (number_of_leptons_2 != 0 && working_point_2 != -99) {
    for (unsigned int iObj = 0; iObj < (*selCollection_2)->size(); iObj++) {
      if (iObj < number_of_leptons_2) {

        double lep_pt = ptr((*selCollection_2)->at(iObj))->pt;
        double lep_eta = abs(ptr((*selCollection_2)->at(iObj))->eta);
        int lepCut = -2;
        lepCut = computeLepCut2(ptr((*selCollection_2)->at(iObj))->pt,
                                                    ptr((*selCollection_2)->at(iObj))->isElectron,
                                                    ptr((*selCollection_2)->at(iObj))->isMuon,
                                                    ptr((*selCollection_2)->at(iObj))->jetBTagCSV,
                                                    ptr((*selCollection_2)->at(iObj))->chargedHadronIsoDR04,
                                                    ptr((*selCollection_2)->at(iObj))->IP,
                                                    ptr((*selCollection_2)->at(iObj))->SIP,
                                                    ptr((*selCollection_2)->at(iObj))->jetPtRatio);
        //TH2 * FR_NP_histo = 0;

        if (lepCut < working_point_2) {

          double FR = 0.0;
          if (ptr((*selCollection_2)->at(iObj))->isElectron) {
            if (working_point_2 == 2) {
              if (lep_pt > 40) FR = 0.15; //0.12;
              else if (lep_pt > 20) FR = 0.15; //0.16;
              else FR = 0.15; } //0.16; }
            else if (working_point_2 == 1) FR = 0.69;
            else std::cout << "Error: invalid lepCut FR working point = " << working_point_2 << std::endl;
          }
          else if (ptr((*selCollection_2)->at(iObj))->isMuon) {
            if (working_point_2 == 2) {
              if (lep_pt > 30) FR = 0.36; //0.40;
              else if (lep_pt > 20) FR = 0.21; //0.34;
              else if (lep_pt > 15) FR = 0.20; //0.26;
              else FR = 0.17; } //0.18; }
            else if (working_point_2 == 1) FR = 0.51;
            else std::cout << "Error: invalid lepCut FR working point = " << working_point_2 << std::endl;
          }
          else std::cout << "Error: lepton is neither muon nor electron" << std::endl;
          
          weight *= -FR/(1.0-FR);
          if (weight == 1 || weight == 0) std::cout << "Error: DataDrivenFRLepCut weight for WP2 = " << weight << std::endl;
        }
      } //end if ( iObj < number_of_leptons_2 )
    } //end loop over iObj
  } //end if (number_of_leptons_2 != 0 && working_point_2 != -99.0)
  
  if (weight == -1.0) weight = 1.0;
  branches[branchNameNP].branchVal = weight;

  if ((number_of_leptons == 2) && (num_passed_leptons == 2) && (QF_charge[0] * QF_charge[1] == -1)) {
    //FR for opposite-sign events with electrons
    if (num_electrons == 0) branches[branchNameQF].branchVal = 1.0;
    if (num_electrons == 1) branches[branchNameQF].branchVal = FR_QF[0];
    if (num_electrons == 2) branches[branchNameQF].branchVal = FR_QF[0] + FR_QF[1];
  }

  //Fill a QF branch with weight 0, so we can run over ss and 3l events together
  if (number_of_leptons == 3) branches[branchNameQF].branchVal = 0.0;

  //Clean out values from last event
  myVars.clear();
  for ( typename map<TString, BranchInfo<double>>::iterator iBranch = branches.begin();
        iBranch != branches.end(); iBranch++ ) {
    myVars.push_back(iBranch->second);
  }
}

template <class collectionType>
DataDrivenFRLepCut<collectionType>::~DataDrivenFRLepCut() {

  //Delete histograms BEFORE closing file
  //delete FR_NP_loose_mu;
  //delete FR_NP_loose2_mu;
  //delete FR_NP_loose_el;
  //delete FR_NP_loose2_el;
  //delete FR_NP_tight_mu;
  //delete FR_NP_tight2_mu;
  //delete FR_NP_tight_el;
  //delete FR_NP_tight2_el;
  delete FR_QF_el;
  //weight_file_NP->Close();
  weight_file_QF->Close();

}

template <class collectionType>
int DataDrivenFRLepCut<collectionType>::computeLepCut2(double pt, int isElectron, int isMuon,
                                       double jetBTagCSV, double chargedHadronIsoDR04,
                                       double IP, double SIP, double jetPtRatio) {
  
  int lepCut = -2;
  if (isElectron) {
    if ( jetBTagCSV > 0.679 ) {
      lepCut = -1; }
    else if ( chargedHadronIsoDR04 < 0 || // < -1 does nothing; < 0 eliminates some
              chargedHadronIsoDR04 / pt > 0.15 ) {
      lepCut = 0; }
    else if ( abs(IP) > 0.015 ||
                            chargedHadronIsoDR04 / pt > 0.05 ||
              jetPtRatio < 0.6 ) {
      lepCut = 1; }
    else lepCut = 2;
  }
  else if (isMuon) {
    if ( jetBTagCSV > 0.679 ) {
      lepCut = -1; }
    else if ( abs(SIP) > 4 ||
              chargedHadronIsoDR04 / pt > 0.20 ) {
      lepCut = 0; }
    else if ( chargedHadronIsoDR04 / pt > 0.10 ||
              jetPtRatio < 0.6 ) {
      lepCut = 1; }
    else lepCut = 2;
  }
  return lepCut;

}

#endif
