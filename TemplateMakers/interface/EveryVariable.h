////////////////////////////////////////////////////////////////
//
//   The purpose of this file is to have all the
//   includes for the kinematic variables
//   in a single location so that every program can easily
//   include them and use them as necessary.
//
////////////////////////////////////////////////////////////////

#include "EFTMultilepton/TemplateMakers/interface/KinematicVariable.h"
#include "EFTMultilepton/TemplateMakers/interface/GenericObjectMember.h"
#include "EFTMultilepton/TemplateMakers/interface/GenericCollectionMember.h"
#include "EFTMultilepton/TemplateMakers/interface/CSVWeights.h"
#include "EFTMultilepton/TemplateMakers/interface/CSVWeightsSF.h"
#include "EFTMultilepton/TemplateMakers/interface/PUWeights.h"
#include "EFTMultilepton/TemplateMakers/interface/TopPtWeights.h"
#include "EFTMultilepton/TemplateMakers/interface/LeptonIDAndIsoScaleFactors.h"
#include "EFTMultilepton/TemplateMakers/interface/LeptonTriggerScaleFactors.h"
#include "EFTMultilepton/TemplateMakers/interface/CleanEventVars.h"
#include "EFTMultilepton/TemplateMakers/interface/CheckTwoLepTrigger.h"
#include "EFTMultilepton/TemplateMakers/interface/CERN/LepMVAs.h"
#include "EFTMultilepton/TemplateMakers/interface/LepGenInfos.h"
#include "EFTMultilepton/TemplateMakers/interface/CERN/TightCharges.h"
#include "EFTMultilepton/TemplateMakers/interface/GenericCollectionSizeVariable.h"
#include "EFTMultilepton/TemplateMakers/interface/MetLD.h"
#include "EFTMultilepton/TemplateMakers/interface/DBCorrectedRelIsoDR04s.h"
#include "EFTMultilepton/TemplateMakers/interface/HiggsDecayType.h"
#include "EFTMultilepton/TemplateMakers/interface/TwoObjectKinematic.h"
#include "EFTMultilepton/TemplateMakers/interface/CERN/FinalBDT.h"
#include "EFTMultilepton/TemplateMakers/interface/CERN/TightChargeAndLepMVAScaleFactors.h"
#include "EFTMultilepton/TemplateMakers/interface/CERN/DataDrivenFR.h"
#include "EFTMultilepton/TemplateMakers/interface/BTagWeights.h"
#include "EFTMultilepton/TemplateMakers/interface/NOVa/BTagShapeWeights.h"
#include "EFTMultilepton/TemplateMakers/interface/PassZmask.h"
#include "EFTMultilepton/TemplateMakers/interface/TwoJetVariables.h"
#include "EFTMultilepton/TemplateMakers/interface/NOVa/FinalBDT_OS_2012.h"
#include "EFTMultilepton/TemplateMakers/interface/GenericVariable.h"
#include "EFTMultilepton/TemplateMakers/interface/CERN/RecoIDIsoSIPSFs.h"
#include "EFTMultilepton/TemplateMakers/interface/CERN/TightChargeAndLepMVAScaleFactorsPerLepton.h"
#include "EFTMultilepton/TemplateMakers/interface/CERN/DataDrivenFRPerLepton.h"
#include "EFTMultilepton/TemplateMakers/interface/ThreeObjectKinematic.h"
#include "EFTMultilepton/TemplateMakers/interface/FourObjectKinematic.h"
#include "EFTMultilepton/TemplateMakers/interface/CERN/FinalBDT_3l.h"
#include "EFTMultilepton/TemplateMakers/interface/MatchTester_ttZ_3l.h"
//#include "EFTMultilepton/TemplateMakers/interface/MatchTester_ttZ_3l_GP.h"
//#include "EFTMultilepton/TemplateMakers/interface/MatchTesterBDT_ttZ_3l.h"
#include "EFTMultilepton/TemplateMakers/interface/MatchTester_ttW_3l.h"
#include "EFTMultilepton/TemplateMakers/interface/MatchTester_ttbar_fake_3l.h"
#include "EFTMultilepton/TemplateMakers/interface/MatchTester_ttW_SS.h"
#include "EFTMultilepton/TemplateMakers/interface/MatchTester_ttbar_fake_SS.h"
#include "EFTMultilepton/TemplateMakers/interface/NOVa/FinalBDT_ttZ_3l.h"
#include "EFTMultilepton/TemplateMakers/interface/NOVa/FinalBDT_ttW_3l.h"
#include "EFTMultilepton/TemplateMakers/interface/NOVa/FinalBDT_ttW_SS.h"
#include "EFTMultilepton/TemplateMakers/interface/MatchTester_ttbar_jj.h"
#include "EFTMultilepton/TemplateMakers/interface/NOVa/LepCuts.h"
#include "EFTMultilepton/TemplateMakers/interface/NOVa/TightChargeAndLepCutScaleFactors.h"
#include "EFTMultilepton/TemplateMakers/interface/NOVa/TightChargeAndLepCutScaleFactorsPerLepton.h"
#include "EFTMultilepton/TemplateMakers/interface/NOVa/DataDrivenFRLepCut.h"
#include "EFTMultilepton/TemplateMakers/interface/NOVa/DataDrivenFRLepCutPerLepton.h"
#include "EFTMultilepton/TemplateMakers/interface/MatchTester_ttbar_ll.h"

