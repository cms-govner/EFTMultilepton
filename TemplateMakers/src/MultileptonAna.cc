// created by Geoff Smith

#include "ttHMultileptonAnalysis/TemplateMakers/interface/MultileptonAna.h"

void MultileptonAna::parse_params()
{
	setupoptionsparams = 	entire_pset.getParameter<edm::ParameterSet> ("setupoptions");
	triggerparams = 	entire_pset.getParameter<edm::ParameterSet> ("triggers");
	muonparams = 		entire_pset.getParameter<edm::ParameterSet> ("muons");
      	electronparams = 	entire_pset.getParameter<edm::ParameterSet> ("electrons");
      	leptonparams = 		entire_pset.getParameter<edm::ParameterSet> ("leptons");
      	jetparams = 		entire_pset.getParameter<edm::ParameterSet> ("jets");
      	subjetparams = 		entire_pset.getParameter<edm::ParameterSet> ("fatjets");
      	btagparams = 		entire_pset.getParameter<edm::ParameterSet> ("btags");
      	metparams = 		entire_pset.getParameter<edm::ParameterSet> ("met");
      	variableparams = 	entire_pset.getParameter<edm::ParameterSet> ("variables");
      	systparams = 		entire_pset.getParameter<edm::ParameterSet> ("systematics");
      	selectionparams = 	entire_pset.getParameter<edm::ParameterSet> ("eventselection");
	
}

void MultileptonAna::SetupOptions (const edm::Event& event)
{
	string rhostr = setupoptionsparams.getParameter<string> ("rhoHandle");
	edm::Handle<double> rhoHandle;
  	event.getByLabel(rhostr,rhoHandle);  //"fixedGridRhoAll");
  	rho = *rhoHandle; 

}
vstring MultileptonAna::HLTInfo ()
{
	vstring HLT_triggers = triggerparams.getParameter<vstring> ("trigger_vstring");
	if (debug) cout << "triggers to include:" << endl;
	if (debug) for (unsigned int i=0; i<HLT_triggers.size(); i++) { cout << HLT_triggers[i] << endl; }
	hltTag = triggerparams.getParameter<string> ("hltlabel");
	
	return HLT_triggers;
}


void MultileptonAna::setupMva(){
  mvaID_ = new EGammaMvaEleEstimatorFWLite();
  bool useBinnedVersion_ = true;
  string method_ = "BDT";
  EGammaMvaEleEstimatorFWLite::MVAType type_ = EGammaMvaEleEstimatorFWLite::kNonTrig;
  std::vector<std::string> mvaWeightFiles_;
  mvaWeightFiles_.push_back("EgammaAnalysis/ElectronTools/data/Electrons_BDTG_NonTrigV0_Cat1.weights.xml");
  mvaWeightFiles_.push_back("EgammaAnalysis/ElectronTools/data/Electrons_BDTG_NonTrigV0_Cat2.weights.xml");
  mvaWeightFiles_.push_back("EgammaAnalysis/ElectronTools/data/Electrons_BDTG_NonTrigV0_Cat3.weights.xml");
  mvaWeightFiles_.push_back("EgammaAnalysis/ElectronTools/data/Electrons_BDTG_NonTrigV0_Cat4.weights.xml");
  mvaWeightFiles_.push_back("EgammaAnalysis/ElectronTools/data/Electrons_BDTG_NonTrigV0_Cat5.weights.xml");
  mvaWeightFiles_.push_back("EgammaAnalysis/ElectronTools/data/Electrons_BDTG_NonTrigV0_Cat6.weights.xml");
  mvaID_->initialize(method_, type_, useBinnedVersion_, mvaWeightFiles_);

  mu_reader_high_b = new TMVA::Reader( "!Color:!Silent" );
  mu_reader_high_e = new TMVA::Reader( "!Color:!Silent" );
  mu_reader_low_b = new TMVA::Reader( "!Color:!Silent" );
  mu_reader_low_e = new TMVA::Reader( "!Color:!Silent" );
  ele_reader_high_cb = new TMVA::Reader( "!Color:!Silent" );
  ele_reader_high_fb = new TMVA::Reader( "!Color:!Silent" );
  ele_reader_high_ec = new TMVA::Reader( "!Color:!Silent" );
  ele_reader_low_cb = new TMVA::Reader( "!Color:!Silent" );
  ele_reader_low_fb = new TMVA::Reader( "!Color:!Silent" );
  ele_reader_low_ec = new TMVA::Reader( "!Color:!Silent" );

  ele_reader_high_cb->AddVariable( "neuRelIso", &varneuRelIso );
  ele_reader_high_cb->AddVariable( "chRelIso", &varchRelIso );
  ele_reader_high_cb->AddVariable( "jetDR_in", &varjetDR_in );
  ele_reader_high_cb->AddVariable( "jetPtRatio_in", &varjetPtRatio_in );
  ele_reader_high_cb->AddVariable( "jetBTagCSV_in", &varjetBTagCSV_in );
  ele_reader_high_cb->AddVariable( "sip3d", &varsip3d );
  ele_reader_high_cb->AddVariable( "mvaId", &varmvaId );
  ele_reader_high_cb->AddVariable( "innerHits", &varinnerHits );

  ele_reader_high_fb->AddVariable( "neuRelIso", &varneuRelIso );
  ele_reader_high_fb->AddVariable( "chRelIso", &varchRelIso );
  ele_reader_high_fb->AddVariable( "jetDR_in", &varjetDR_in );
  ele_reader_high_fb->AddVariable( "jetPtRatio_in", &varjetPtRatio_in );
  ele_reader_high_fb->AddVariable( "jetBTagCSV_in", &varjetBTagCSV_in );
  ele_reader_high_fb->AddVariable( "sip3d", &varsip3d );
  ele_reader_high_fb->AddVariable( "mvaId", &varmvaId );
  ele_reader_high_fb->AddVariable( "innerHits", &varinnerHits );

  ele_reader_high_ec->AddVariable( "neuRelIso", &varneuRelIso );
  ele_reader_high_ec->AddVariable( "chRelIso", &varchRelIso );
  ele_reader_high_ec->AddVariable( "jetDR_in", &varjetDR_in );
  ele_reader_high_ec->AddVariable( "jetPtRatio_in", &varjetPtRatio_in );
  ele_reader_high_ec->AddVariable( "jetBTagCSV_in", &varjetBTagCSV_in );
  ele_reader_high_ec->AddVariable( "sip3d", &varsip3d );
  ele_reader_high_ec->AddVariable( "mvaId", &varmvaId );
  ele_reader_high_ec->AddVariable( "innerHits", &varinnerHits );

  ele_reader_low_cb->AddVariable( "neuRelIso", &varneuRelIso );
  ele_reader_low_cb->AddVariable( "chRelIso", &varchRelIso );
  ele_reader_low_cb->AddVariable( "jetDR_in", &varjetDR_in );
  ele_reader_low_cb->AddVariable( "jetPtRatio_in", &varjetPtRatio_in );
  ele_reader_low_cb->AddVariable( "jetBTagCSV_in", &varjetBTagCSV_in );
  ele_reader_low_cb->AddVariable( "sip3d", &varsip3d );
  ele_reader_low_cb->AddVariable( "mvaId", &varmvaId );
  ele_reader_low_cb->AddVariable( "innerHits", &varinnerHits );

  ele_reader_low_fb->AddVariable( "neuRelIso", &varneuRelIso );
  ele_reader_low_fb->AddVariable( "chRelIso", &varchRelIso );
  ele_reader_low_fb->AddVariable( "jetDR_in", &varjetDR_in );
  ele_reader_low_fb->AddVariable( "jetPtRatio_in", &varjetPtRatio_in );
  ele_reader_low_fb->AddVariable( "jetBTagCSV_in", &varjetBTagCSV_in );
  ele_reader_low_fb->AddVariable( "sip3d", &varsip3d );
  ele_reader_low_fb->AddVariable( "mvaId", &varmvaId );
  ele_reader_low_fb->AddVariable( "innerHits", &varinnerHits );

  ele_reader_low_ec->AddVariable( "neuRelIso", &varneuRelIso );
  ele_reader_low_ec->AddVariable( "chRelIso", &varchRelIso );
  ele_reader_low_ec->AddVariable( "jetDR_in", &varjetDR_in );
  ele_reader_low_ec->AddVariable( "jetPtRatio_in", &varjetPtRatio_in );
  ele_reader_low_ec->AddVariable( "jetBTagCSV_in", &varjetBTagCSV_in );
  ele_reader_low_ec->AddVariable( "sip3d", &varsip3d );
  ele_reader_low_ec->AddVariable( "mvaId", &varmvaId );
  ele_reader_low_ec->AddVariable( "innerHits", &varinnerHits );

  mu_reader_high_b->AddVariable( "neuRelIso", &varneuRelIso );
  mu_reader_high_b->AddVariable( "chRelIso", &varchRelIso );
  mu_reader_high_b->AddVariable( "jetDR_in", &varjetDR_in );
  mu_reader_high_b->AddVariable( "jetPtRatio_in", &varjetPtRatio_in );
  mu_reader_high_b->AddVariable( "jetBTagCSV_in", &varjetBTagCSV_in );
  mu_reader_high_b->AddVariable( "sip3d", &varsip3d );
  mu_reader_high_b->AddVariable( "dxy", &vardxy );
  mu_reader_high_b->AddVariable( "dz", &vardz );

  mu_reader_high_e->AddVariable( "neuRelIso", &varneuRelIso );
  mu_reader_high_e->AddVariable( "chRelIso", &varchRelIso );
  mu_reader_high_e->AddVariable( "jetDR_in", &varjetDR_in );
  mu_reader_high_e->AddVariable( "jetPtRatio_in", &varjetPtRatio_in );
  mu_reader_high_e->AddVariable( "jetBTagCSV_in", &varjetBTagCSV_in );
  mu_reader_high_e->AddVariable( "sip3d", &varsip3d );
  mu_reader_high_e->AddVariable( "dxy", &vardxy );
  mu_reader_high_e->AddVariable( "dz", &vardz );

  mu_reader_low_b->AddVariable( "neuRelIso", &varneuRelIso );
  mu_reader_low_b->AddVariable( "chRelIso", &varchRelIso );
  mu_reader_low_b->AddVariable( "jetDR_in", &varjetDR_in );
  mu_reader_low_b->AddVariable( "jetPtRatio_in", &varjetPtRatio_in );
  mu_reader_low_b->AddVariable( "jetBTagCSV_in", &varjetBTagCSV_in );
  mu_reader_low_b->AddVariable( "sip3d", &varsip3d );
  mu_reader_low_b->AddVariable( "dxy", &vardxy );
  mu_reader_low_b->AddVariable( "dz", &vardz );

  mu_reader_low_e->AddVariable( "neuRelIso", &varneuRelIso );
  mu_reader_low_e->AddVariable( "chRelIso", &varchRelIso );
  mu_reader_low_e->AddVariable( "jetDR_in", &varjetDR_in );
  mu_reader_low_e->AddVariable( "jetPtRatio_in", &varjetPtRatio_in );
  mu_reader_low_e->AddVariable( "jetBTagCSV_in", &varjetBTagCSV_in );
  mu_reader_low_e->AddVariable( "sip3d", &varsip3d );
  mu_reader_low_e->AddVariable( "dxy", &vardxy );
  mu_reader_low_e->AddVariable( "dz", &vardz );

  mu_reader_high_b->BookMVA( "BDTG method", string(getenv("CMSSW_BASE")) + "/src/ttHMultileptonAnalysis/TemplateMakers/data/CERN/lepMVA_weights/mu_pteta_high_b_BDTG.weights.xml");
  mu_reader_high_e->BookMVA( "BDTG method", string(getenv("CMSSW_BASE")) + "/src/ttHMultileptonAnalysis/TemplateMakers/data/CERN/lepMVA_weights/mu_pteta_high_e_BDTG.weights.xml");
  mu_reader_low_b->BookMVA( "BDTG method", string(getenv("CMSSW_BASE")) + "/src/ttHMultileptonAnalysis/TemplateMakers/data/CERN/lepMVA_weights/mu_pteta_low_b_BDTG.weights.xml");
  mu_reader_low_e->BookMVA( "BDTG method", string(getenv("CMSSW_BASE")) + "/src/ttHMultileptonAnalysis/TemplateMakers/data/CERN/lepMVA_weights/mu_pteta_low_e_BDTG.weights.xml");
  ele_reader_high_cb->BookMVA( "BDTG method", string(getenv("CMSSW_BASE")) + "/src/ttHMultileptonAnalysis/TemplateMakers/data/CERN/lepMVA_weights/el_pteta_high_cb_BDTG.weights.xml");
  ele_reader_high_fb->BookMVA( "BDTG method", string(getenv("CMSSW_BASE")) + "/src/ttHMultileptonAnalysis/TemplateMakers/data/CERN/lepMVA_weights/el_pteta_high_fb_BDTG.weights.xml");
  ele_reader_high_ec->BookMVA( "BDTG method", string(getenv("CMSSW_BASE")) + "/src/ttHMultileptonAnalysis/TemplateMakers/data/CERN/lepMVA_weights/el_pteta_high_ec_BDTG.weights.xml");
  ele_reader_low_cb->BookMVA( "BDTG method", string(getenv("CMSSW_BASE")) + "/src/ttHMultileptonAnalysis/TemplateMakers/data/CERN/lepMVA_weights/el_pteta_low_cb_BDTG.weights.xml");
  ele_reader_low_fb->BookMVA( "BDTG method", string(getenv("CMSSW_BASE")) + "/src/ttHMultileptonAnalysis/TemplateMakers/data/CERN/lepMVA_weights/el_pteta_low_fb_BDTG.weights.xml");
  ele_reader_low_ec->BookMVA( "BDTG method", string(getenv("CMSSW_BASE")) + "/src/ttHMultileptonAnalysis/TemplateMakers/data/CERN/lepMVA_weights/el_pteta_low_ec_BDTG.weights.xml");

  
}





trigRes MultileptonAna::GetTriggers (const edm::Event& event)
{
	// hltTag already got by HLTInfo ...	
	edm::Handle<edm::TriggerResults> triggerResults;
	event.getByLabel(edm::InputTag("TriggerResults","", hltTag), triggerResults);
	return triggerResults;
}


patMuons MultileptonAna::GetMuons (const edm::Event& event)
{
	string muCollection = muonparams.getParameter<string> ("muonCollection");
	patMuons muonHandle; 
	event.getByLabel(muCollection,muonHandle);
	return muonHandle;
	
}
patElectrons MultileptonAna::GetElectrons (const edm::Event& event)
{
	string elCollection = electronparams.getParameter<string> ("electronCollection");
	patElectrons electronHandle; 
	event.getByLabel(elCollection,electronHandle);
	return electronHandle;
	
}
vecPatLepton MultileptonAna::fillLeptons(vecPatMuon& muons, vecPatElectron& electrons)
{
  vecPatLepton leptons;
  for (vecPatMuon::const_iterator iMu = muons.begin(); iMu != muons.end(); iMu++)
    {
      leptons.push_back(*iMu);
    }
  for (vecPatElectron::const_iterator iEle = electrons.begin(); iEle != electrons.end(); iEle++)
    {
      leptons.push_back(*iEle);
    }
  return leptons;
}

void MultileptonAna::GetLeptons (const edm::Event& event)
{
	bool are_electrons_added_to_leptons = leptonparams.getParameter<bool> ("useElectrons");
	bool are_muons_added_to_leptons = leptonparams.getParameter<bool> ("useMuons");
	
	if (debug) cout << are_electrons_added_to_leptons << " " << are_muons_added_to_leptons << endl;
	
	
}
patJets MultileptonAna::GetJets (const edm::Event& event)
{
	string mainjetCollection = jetparams.getParameter<string> ("jetCollection");
	patJets jetsHandle; 
	event.getByLabel(mainjetCollection,jetsHandle);
	return jetsHandle;
	
	
}
patJets MultileptonAna::GetSubJets (const edm::Event& event)
{
	string subjetCollection = subjetparams.getParameter<string> ("fatjetCollection");
	patJets subjetsHandle; 
	event.getByLabel(subjetCollection,subjetsHandle);
	return subjetsHandle;
	
	
}
void MultileptonAna::GetBtags (const edm::Event& event)
{
	
}
patMETs MultileptonAna::GetMet (const edm::Event& event)
{
	
	string metCollection = metparams.getParameter<string> ("METCollection");
	patMETs METsHandle; 
	event.getByLabel(metCollection,METsHandle);
	return METsHandle;
	
	
}

int MultileptonAna::GetVertices (const edm::Event& event)
{
	
	////////////////////////////////////////////////////////////
	//
	// count pvs for miniAODhelper
	//
	////////////////////////////////////////////////////////////

	//vars for vertex loop
	reco::Vertex vertex;
	double minNDOF = 4.0;
	double maxAbsZ = 24.0;
	double maxd0 = 2.;
	int numpv = 0;

	edm::Handle<reco::VertexCollection> vtxHandle;
	event.getByLabel("offlineSlimmedPrimaryVertices",vtxHandle);
	reco::VertexCollection primaryVertices = *vtxHandle;

	numpv = 0;

	for (reco::VertexCollection::const_iterator iPV = primaryVertices.begin(); iPV != primaryVertices.end(); ++iPV)
	{
		bool isGood = ( !(iPV->isFake()) && (iPV->ndof()>=minNDOF) && (abs(iPV->z())<=maxAbsZ) && (abs(iPV->position().Rho())<=maxd0));
		if ( !isGood ) continue;
		if ( numpv == 0 ) vertex = *iPV;
		numpv++;
	}

	//cout << "numpv = " << numpv << endl;
	if ( numpv > 0 )
	{
		//cout << "setting vertex, numpv > 0" << endl;
		//miniAODhelper.SetVertex(vertex);
		SetVertex(vertex);
	}

	return numpv;
	
}


void MultileptonAna::Variables (const edm::Event& event)
{
	//vstring variable_list = variableparams.getParameter<vstring> ("thevars");
}
void MultileptonAna::Systematics (const edm::Event& event)
{
	
	//bool include_systematics = systparams.getParameter<bool> ("include_syst");
	//vstring vstring_of_systs = systparams.getParameter<vstring> ("syst_list");
	
}
void MultileptonAna::EventSelection (const edm::Event& event)
{
	//edm::ParameterSet lepselection_pset = selectionparams.getParameter<edm::ParameterSet> ("Leps");
	//string numJets_str = selectionparams.getParameter<string> ("numJets");
	//string numTags_str = selectionparams.getParameter<string> ("numTags");
	//double met_cut = selectionparams.getParameter<double> ("METcut");
	
}



vecTLorentzVector MultileptonAna::Get_vecTLorentzVector (vecPatMuon theobjs)
{
	//int nobjs = theobjcollection.size();
	vecTLorentzVector theobjs_vecTLV;
	
	for (muit iMuon= theobjs.begin(); iMuon != theobjs.end(); ++iMuon)
	{
		TLorentzVector tempTLV;
		tempTLV.SetPxPyPzE(iMuon->px(),iMuon->py(),iMuon->pz(),iMuon->energy());
		theobjs_vecTLV.push_back(tempTLV);
	}
		
	return theobjs_vecTLV;
		
}
vecTLorentzVector MultileptonAna::Get_vecTLorentzVector (vecPatElectron theobjs)
{
	vecTLorentzVector theobjs_vecTLV;
	
	for (eleit iEle= theobjs.begin(); iEle != theobjs.end(); ++iEle)
	{
		TLorentzVector tempTLV;
		tempTLV.SetPxPyPzE(iEle->px(),iEle->py(),iEle->pz(),iEle->energy());
		theobjs_vecTLV.push_back(tempTLV);
	}
		
	return theobjs_vecTLV;
}
vecTLorentzVector MultileptonAna::Get_vecTLorentzVector (vecPatJet theobjs)
{
	vecTLorentzVector theobjs_vecTLV;
	
	for (jetit iJet = theobjs.begin(); iJet != theobjs.end(); ++iJet)
	{
		TLorentzVector tempTLV;
		tempTLV.SetPxPyPzE(iJet->px(),iJet->py(),iJet->pz(),iJet->energy());
		theobjs_vecTLV.push_back(tempTLV);
	}
		
	return theobjs_vecTLV;
	
	
}
//vecTLorentzVector Get_vecTLorentzVector_SubJets ()

TLorentzVector MultileptonAna::Get_TLorentzVector (patMETs theobjs)
{
	pat::MET theMET = theobjs->front();
	TLorentzVector metTLV(theMET.px(),theMET.py(),0.0,theMET.pt());
	return metTLV;
	
}
TLorentzVector MultileptonAna::Get_TLorentzVector (pat::MET theMET)
{
	//pat::MET theMET = theobj;
	TLorentzVector metTLV(theMET.px(),theMET.py(),0.0,theMET.pt());
	return metTLV;
	
}

vecTLorentzVector MultileptonAna::Get_vecTLorentzVector_sorted_leptons (vecTLorentzVector leps1, vecTLorentzVector leps2)
{	

	vecTLorentzVector newvecTLV;
	
	// assuming they are already sorted by pt, respectively:
	
	int size1 = leps1.size();
 	int size2 = leps2.size();
 	
 	if (size1==0||size2!=0) return leps2;
	if (size2==0||size1!=0) return leps1;
	if (size1==0&&size2==0) { TLorentzVector ret(0.,0.,0.,0.); newvecTLV.push_back(ret); return newvecTLV; } // <- should figure out something else for this case..
	
	int maxsize = max(size1,size2);
	int minsize = min(size1,size2);
	
	bool smallestis1 = (size1<size2) ? true : false;
	
	int i = 0;
	int j = 0;
	
	while (true)
	{
		if ((i==minsize&&smallestis1)||(j==minsize&&(!smallestis1))) break;

		if (leps1[i].Pt() < leps2[j].Pt())
		{
			newvecTLV.push_back(leps2[j]);
			j++;
		}
		else
		{
			newvecTLV.push_back(leps1[i]);
			i++;
		}
	}		
	if (smallestis1)
	{
		while (true)
		{
			if (j==maxsize) break;
			newvecTLV.push_back(leps2[j]);
			j++;
		}								
	}
	if (!smallestis1)
	{			
		while (true)
		{
			if (i==maxsize) break;
			newvecTLV.push_back(leps1[i]);
			i++;
		}
	}
	
//// above takes a time that is linear with size of the vectors.
//// brute force (if you aren't sure if they are sorted beforehand), takes
//// a time that is quadratic in size of the vector:
// 	
// 	leps1.insert(leps1.end(),leps2.begin(),leps2.end());
// 	vecTLorentzVector newvecTLV;
// 	
// 	//int startingpoint = 0;
// 	int size = leps1.size();
// 	
// 	for (int i=0; i<size; i++)
// 	{
// 		for (int j=0; j<size; j++)
// 		{
// 			bool isprevious = false;
// 			
// 			int max
			
	
//	TLorentzVector dummy;
//	dummy.SetPxPyPzE(0.,0.,0.,0.);
//	newvecTLV.push_back(dummy);

	return newvecTLV;
	
}


bool MultileptonAna::isGoodMuon(const pat::Muon& iMuon, const float iMinPt, const muonID::muonID iMuonID){
  
  CheckVertexSetUp();
  
  double minMuonPt = iMinPt;
  
  //float maxLooseMuonAbsEta = 2.5;
  float maxLooseMuonAbsEta = muonparams.getParameter<double> ("maxLooseMuonAbsEta");

  //float maxTightMuonAbsEta = 2.1;
//float maxTightMuonAbsEta = 0.25;
  float maxTightMuonAbsEta = muonparams.getParameter<double> ("maxTightMuonAbsEta");
  
  // Be skeptical about this muon making it through
  bool passesKinematics	= false;
  bool passesIso        = false;
  bool passesID         = false;
  bool isPFMuon         = false;
  bool passesTrackerID  = false;

  bool passesGlobalTrackID   = false;
  bool passesMuonBestTrackID = false;
  bool passesInnerTrackID    = false;
  bool passesTrackID         = false;

  
  double tightRelativeIso = muonparams.getParameter<double> ("tightRelativeIso");
  double looseRelativeIso = muonparams.getParameter<double> ("looseRelativeIso");
  double tightTrackNormalizedChi2 = muonparams.getParameter<double> ("tightTrackNormalizedChi2");
  int tightTrackNumberOfValidMuonHits = muonparams.getParameter<int> ("tightTrackNumberOfValidMuonHits");
  double tightTrackDxy = muonparams.getParameter<double> ("tightTrackDxy");
  double tightTrackDz = muonparams.getParameter<double> ("tightTrackDz");
  int tightNumberOfValidPixelHits = muonparams.getParameter<int> ("tightNumberOfValidPixelHits");
  int tightTrackerLayersWithMeasurement = muonparams.getParameter<int> ("tightTrackerLayersWithMeasurement");
  
  switch(iMuonID){
  case muonID::muonSide:
  case muonID::muonSideLooseMVA:
  case muonID::muonSideTightMVA:
  case muonID::muonPtOnly:
  case muonID::muonPtEtaOnly:
  case muonID::muonPtEtaIsoOnly:
  case muonID::muonPtEtaIsoTrackerOnly:
  case muonID::muonNoCuts:
  case muonID::muonLoose:
    passesKinematics = ((iMuon.pt() >= minMuonPt) && (fabs(iMuon.eta()) <= maxLooseMuonAbsEta));
    passesIso        = (GetMuonRelIso(iMuon) < looseRelativeIso);
    isPFMuon         = true;
    passesID         = (( iMuon.isGlobalMuon() || iMuon.isTrackerMuon() ) && isPFMuon);
    break;
  case muonID::muonTight:
    passesKinematics = ((iMuon.pt() >= minMuonPt) && (fabs(iMuon.eta()) <= maxTightMuonAbsEta));
    passesIso        = (GetMuonRelIso(iMuon) < tightRelativeIso);
    isPFMuon         = true;

    if( iMuon.globalTrack().isAvailable() ){
      passesGlobalTrackID = ( (iMuon.globalTrack()->normalizedChi2() < tightTrackNormalizedChi2) 
			      && (iMuon.globalTrack()->hitPattern().numberOfValidMuonHits() > tightTrackNumberOfValidMuonHits)
			      );
    }
    if( iMuon.muonBestTrack().isAvailable() ){
      passesMuonBestTrackID = ( (fabs(iMuon.muonBestTrack()->dxy(vertex.position())) < tightTrackDxy)
				&& (fabs(iMuon.muonBestTrack()->dz(vertex.position())) < tightTrackDz)
				);
    }
    if( iMuon.innerTrack().isAvailable() )
      passesInnerTrackID = (iMuon.innerTrack()->hitPattern().numberOfValidPixelHits() > tightNumberOfValidPixelHits);
    if( iMuon.track().isAvailable() )
      passesTrackID = (iMuon.track()->hitPattern().trackerLayersWithMeasurement() > tightTrackerLayersWithMeasurement);

    passesTrackerID = ( passesGlobalTrackID && passesMuonBestTrackID && passesInnerTrackID && passesTrackID && (iMuon.numberOfMatchedStations() > 1) );

    passesID        = ((iMuon.isGlobalMuon() || iMuon.isTrackerMuon()) && isPFMuon && passesTrackerID);
    break;
  case muonID::muonPreselection:
    passesKinematics = ((iMuon.pt() > 5.) && (fabs(iMuon.eta()) < 2.4));
    passesIso        = (GetMuonRelIso(iMuon) < 0.4);
    isPFMuon         = true;
    if( iMuon.muonBestTrack().isAvailable() ){
      passesMuonBestTrackID = ( (fabs(iMuon.muonBestTrack()->dxy(vertex.position())) < 0.05)
                                && (fabs(iMuon.muonBestTrack()->dz(vertex.position())) < 0.2)
                                );
    }
    //need charge flip cuts...
    passesID         = (( iMuon.isGlobalMuon() || iMuon.isTrackerMuon() ) && isPFMuon && passesMuonBestTrackID );
    break;
  }

  return (passesKinematics && passesIso && passesID);
}

float MultileptonAna::GetMuonRelIso(const pat::Muon& iMuon) const
{
  float result = 9999;
  
  double pfIsoCharged = iMuon.pfIsolationR03().sumChargedHadronPt;
  double pfIsoNeutral = iMuon.pfIsolationR03().sumNeutralHadronEt + iMuon.pfIsolationR03().sumPhotonEt;
  
  double pfIsoPUSubtracted = std::max( 0.0, pfIsoNeutral - 0.5*iMuon.pfIsolationR03().sumPUPt );
  
  result = (pfIsoCharged + pfIsoPUSubtracted)/iMuon.pt();
  
  return result;
}




bool MultileptonAna::isGoodElectron(const pat::Electron& iElectron, const float iMinPt, const electronID::electronID iElectronID){

  CheckVertexSetUp();

  double minElectronPt = iMinPt;

  float maxLooseElectronAbsEta = electronparams.getParameter<double> ("maxLooseElectronAbsEta");
  float maxTightElectronAbsEta = electronparams.getParameter<double> ("maxTightElectronAbsEta");
  string theElectronMVA = electronparams.getParameter<string> ("theElectronMVA");
  double passMVAcut = electronparams.getParameter<double> ("passMVAcut");
  double tightElectronIso = electronparams.getParameter<double> ("tightElectronIso");
  double looseElectronIso = electronparams.getParameter<double> ("looseElectronIso");  
  double tightDxy = electronparams.getParameter<double> ("tightDxy");
  double looseDxy = electronparams.getParameter<double> ("looseDxy");
  double dz = electronparams.getParameter<double> ("dZ");
  
  // Be skeptical about this electron making it through
  bool passesKinematics	= false;
  bool passesIso        = false;
  bool passesID         = false;

  bool inCrack = false;
  if( iElectron.superCluster().isAvailable() )
    inCrack = ( fabs(iElectron.superCluster()->position().eta())>1.4442 && fabs(iElectron.superCluster()->position().eta())<1.5660 ); // doesn't change


  // below should be made less confusing ...

  bool myTrigPresel = true;

  double eleID      = iElectron.electronID(theElectronMVA);
  bool passMVAId53x = ( eleID > passMVAcut );  // For 2012_53x, tighter selection

  bool d02 = false; 
  bool d04 = false;
  bool dZ  = false;
  bool no_exp_inner_trkr_hits = true; //false; // see below
  bool passGsfTrackID = false;
  if( iElectron.gsfTrack().isAvailable() ){
    d02 = ( fabs(iElectron.gsfTrack()->dxy(vertex.position())) < tightDxy );
    d04 = ( fabs(iElectron.gsfTrack()->dxy(vertex.position())) < looseDxy );
    //no_exp_inner_trkr_hits = ( iElectron.gsfTrack()->trackerExpectedHitsInner().numberOfHits() <= 0 ); // deprecated in 7_2_0 .. replace with ..?
    dZ = ( fabs(iElectron.gsfTrack()->dz(vertex.position())) < dz );
  }

  bool notConv = ( iElectron.passConversionVeto() );
  bool id      = ( passMVAId53x && d02 && dZ && notConv );

  switch(iElectronID){
  case electronID::electronSide:
  case electronID::electronSideLooseMVA:
  case electronID::electronSideTightMVA:
  case electronID::electronLooseMinusTrigPresel:
  case electronID::electronNoCuts:
  case electronID::electronLoose:
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxLooseElectronAbsEta) && !inCrack);
    passesIso        = (GetElectronRelIso(iElectron) < looseElectronIso);
    passesID         = ( passMVAId53x && no_exp_inner_trkr_hits && d04 && notConv && myTrigPresel );
    break;
  case electronID::electronTightMinusTrigPresel:
  case electronID::electronTight:
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxTightElectronAbsEta) && !inCrack);
    passesIso        = (GetElectronRelIso(iElectron) < tightElectronIso);
    passesID         = ( id && no_exp_inner_trkr_hits && myTrigPresel );
    break;
  case electronID::electronPreselection:
    ///////
    //
    // ele mva id hack
    //
    //////
    bool passesMVA = false;
    bool useFull5x5 = true;
    bool mvaDebug = false;
    double eleMvaNonTrig = mvaID_->mvaValue(iElectron,vertex,rho,useFull5x5,mvaDebug);
    if ( iElectron.pt() < 10 ){
      if ( abs(iElectron.eta()) > 0. && abs(iElectron.eta()) < 0.8){
	passesMVA = ( eleMvaNonTrig > 0.47 );
      }
      else if ( abs(iElectron.eta()) >= 0.8 && abs(iElectron.eta()) < 1.479){
	passesMVA = ( eleMvaNonTrig > 0.004 );
      }
      else if ( abs(iElectron.eta()) >= 1.479 && abs(iElectron.eta()) <= 2.5){
	passesMVA = ( eleMvaNonTrig > 0.295 );
      }
    }
    else if ( iElectron.pt() >= 10 ) {
      if ( abs(iElectron.eta()) > 0. && abs(iElectron.eta()) < 0.8){
	passesMVA = ( eleMvaNonTrig > 0.5 );
      }
      else if ( abs(iElectron.eta()) >= 0.8 && abs(iElectron.eta()) < 1.479){
	passesMVA = ( eleMvaNonTrig > 0.12 );
      }
      else if ( abs(iElectron.eta()) >= 1.479 && abs(iElectron.eta()) <= 2.5){
	passesMVA = ( eleMvaNonTrig > 0.60 );
      }
    }
    
    passesKinematics = ((iElectron.pt() > 7.) && (fabs(iElectron.eta()) < 2.5));
    passesIso        = (GetElectronRelIso(iElectron) < 0.400);
    if( iElectron.gsfTrack().isAvailable() ){
      passGsfTrackID = ( (fabs(iElectron.gsfTrack()->dxy(vertex.position())) < 0.05) && (fabs(iElectron.gsfTrack()->dz(vertex.position())) < 0.2) && iElectron.gsfTrack()->trackerExpectedHitsInner().numberOfHits() <= 1  );
    }
    
    passesID         = ( passGsfTrackID && passMVAId53x && no_exp_inner_trkr_hits && notConv && myTrigPresel && passesMVA);    
    break;
  }
  
  return (passesKinematics && passesIso && passesID);
}

bool MultileptonAna::isGoodTau(const pat::Tau& iTau, const float iMinPt, const tauID::tauID iTauID){

  CheckVertexSetUp();
 
  double minTauPt = iMinPt;
  
  //double tauMinPt
  //double tauMaxAbsEta = 
  
  bool passesKinematics = false;
  passesKinematics = (iTau.pt() >= 20) && (fabs(iTau.eta()) <= 2.1) && (iTau.pt() > minTauPt); //minTauPt vs. 20?
  return passesKinematics;
}

bool MultileptonAna::isGoodJet(const pat::Jet& iJet, const float iMinPt, const float iMaxAbsEta, const jetID::jetID iJetID, const char iCSVworkingPoint){

  CheckVertexSetUp();

  
//   neutralHadronEnergyFraction
//   chargedEmEnergyFraction
//   neutralEmEnergyFraction
//   numberOfDaughters
//   chargedHadronEnergyFraction
//   chargedMultiplicity

  // Transverse momentum requirement
  if( iJet.pt() < iMinPt ) return false;

  // Absolute eta requirement
  if( fabs(iJet.eta()) > iMaxAbsEta ) return false;

  bool loose = (

		iJet.neutralHadronEnergyFraction() < 0.99 &&
		iJet.chargedEmEnergyFraction() < 0.99 &&
		iJet.neutralEmEnergyFraction() < 0.99 &&
		iJet.numberOfDaughters() > 1

		);

  if( fabs(iJet.eta())<2.4 ){ // endcaps
    loose = ( loose &&
	      iJet.chargedHadronEnergyFraction() > 0.0 &&
	      iJet.chargedMultiplicity() > 0
	      );
  }

  bool passesPUJetID = false;
  float puMvaId = iJet.userFloat("pileupJetId:fullDiscriminant");
  // Jet ID
  switch(iJetID){
  case jetID::none:
  case jetID::jetPU:
    if (fabs(iJet.eta()) > 0. && fabs(iJet.eta()) <2.5)
      {
        passesPUJetID = (puMvaId > -0.63);
      }
    else if (fabs(iJet.eta()) >= 2.5 && fabs(iJet.eta()) < 2.75)
      {
        passesPUJetID = (puMvaId > -0.60);
      }
    else if (fabs(iJet.eta()) >= 2.75 && fabs(iJet.eta()) < 3.0)
      {
	passesPUJetID = (puMvaId > -0.55);
      }
    else if (fabs(iJet.eta()) >= 3.0 && fabs(iJet.eta()) <= 5.2)
      {
        passesPUJetID = (puMvaId > -0.45);
      }
    if (passesPUJetID) return true;
    break;
  case jetID::jetMinimal:
  case jetID::jetLooseAOD:
  case jetID::jetLoose:
  case jetID::jetTight:
    if( !loose ) return false;
    break;
  default:
    break;
  }

  if( !PassesCSV(iJet, iCSVworkingPoint) ) return false;

  return true;
}

template <typename obj1, typename obj2>
std::vector<obj1> MultileptonAna::cleanObjects(const std::vector<obj1>& dirtyCollection,const std::vector<obj2>& soapCollection,const double coneSize)
{
  std::vector<obj1> cleanedCollection;
  bool isClean;
  for (typename std::vector<obj1>::const_iterator dirtObj = dirtyCollection.begin(); dirtObj != dirtyCollection.end(); dirtObj++)
    {
      isClean = true;
      for (typename std::vector<obj2>::const_iterator soapObj = soapCollection.begin(); soapObj != soapCollection.end(); soapObj++)
        {
          if (MiniAODHelper::DeltaR(dirtObj,soapObj) <= coneSize )
            {
              isClean = false;
              break;
            }
        }
      if (isClean)
        {
          obj1 cleanedObj = (*dirtObj);
          cleanedCollection.push_back(cleanedObj);
        }
    }
  
  return cleanedCollection;
}

template <typename particleType>
pat::Jet MultileptonAna::getClosestJet(const std::vector<pat::Jet>& jets, const particleType& object) {
  pat::Jet result;
  double minDeltaR = 999;
  //  for (auto& jet: jets) {
  for (std::vector<pat::Jet>::const_iterator iJet = jets.begin(); iJet != jets.end(); iJet++)
    {
      double dR = MiniAODHelper::DeltaR(iJet, &object);
      if (dR < minDeltaR)
	{
	  minDeltaR = dR;
	  result = (*iJet);
	}
    }
  return result;
}

float MultileptonAna::GetMuonLepMVA(const pat::Muon& iMuon, const std::vector<pat::Jet>& iJets){
  CheckSetUp();

  varchRelIso = iMuon.chargedHadronIso()/iMuon.pt();
  varneuRelIso = GetMuonRelIso(iMuon) - varchRelIso;

  pat::Jet matchedJet = getClosestJet(iJets,iMuon);
  double dR = MiniAODHelper::DeltaR(&matchedJet,&iMuon);
  varjetDR_in = min(dR,0.5);
  varjetPtRatio_in = min(iMuon.pt()/matchedJet.pt(), float(1.5));
  
  varjetBTagCSV_in = max(matchedJet.bDiscriminator("combinedSecondaryVertexBJetTags"), float(0.0));
  varsip3d = fabs(iMuon.dB(pat::Muon::PV3D)/iMuon.edB(pat::Muon::PV3D));
  vardxy = fabs(iMuon.muonBestTrack()->dxy(vertex.position()));
  vardz = fabs(iMuon.muonBestTrack()->dz(vertex.position()));

  if (iMuon.pt() > 15 && fabs(iMuon.eta()) < 1.5) {
    // std::cout << "LepMVA: " << mu_reader_high_b->EvaluateMVA( "BDTG method" ) << std::endl;
    return mu_reader_high_b->EvaluateMVA( "BDTG method" );
  }
  else if (iMuon.pt() > 15 && fabs(iMuon.eta()) >= 1.5) {
    // std::cout << "LepMVA: " << mu_reader_high_e->EvaluateMVA( "BDTG method" ) << std::endl;
    return mu_reader_high_e->EvaluateMVA( "BDTG method" );
  }
  else if (iMuon.pt() <= 15 && fabs(iMuon.eta()) < 1.5) {
    // std::cout << "LepMVA: " << mu_reader_low_b->EvaluateMVA( "BDTG method" ) << std::endl;
    return mu_reader_low_b->EvaluateMVA( "BDTG method" );
  }
  else {
    // std::cout << "LepMVA: " << mu_reader_low_e->EvaluateMVA( "BDTG method" ) << std::endl;
    return mu_reader_low_e->EvaluateMVA( "BDTG method" );
  }
}


float MultileptonAna::GetElectronLepMVA(const pat::Electron& iElectron, const std::vector<pat::Jet>& iJets){
  CheckSetUp();

  varchRelIso = iElectron.chargedHadronIso()/iElectron.pt();
  varneuRelIso = GetElectronRelIso(iElectron) - varchRelIso;

  pat::Jet matchedJet = getClosestJet(iJets,iElectron);
  double dR = MiniAODHelper::DeltaR(&matchedJet,&iElectron);
  varjetDR_in = min(dR,0.5);
  varjetPtRatio_in = min(iElectron.pt()/matchedJet.pt(), float(1.5));
  
  varjetBTagCSV_in = max(matchedJet.bDiscriminator("combinedSecondaryVertexBJetTags"), float(0.0));
  varsip3d = fabs(iElectron.dB(pat::Electron::PV3D)/iElectron.edB(pat::Electron::PV3D));
  vardxy = fabs(iElectron.gsfTrack()->dxy(vertex.position()));
  vardz = fabs(iElectron.gsfTrack()->dz(vertex.position()));

  bool useFull5x5 = true;
  bool mvaDebug = false;
  varmvaId = mvaID_->mvaValue(iElectron,vertex,rho,useFull5x5,mvaDebug);  
  
  // varmvaId = iElectron.mva;
  varinnerHits = iElectron.gsfTrack()->trackerExpectedHitsInner().numberOfHits();
  
  if (iElectron.pt() >= 10 && fabs(iElectron.eta()) <= 0.8) {
    // std::cout << "LepMVA: " << ele_reader_high_cb->EvaluateMVA( "BDTG method" ) << std::endl;
    return ele_reader_high_cb->EvaluateMVA( "BDTG method" );
  }
  else if (iElectron.pt() >= 10 && fabs(iElectron.eta()) > 0.8 && fabs(iElectron.eta()) <= 1.479) {
    // std::cout << "LepMVA: " << ele_reader_high_fb->EvaluateMVA( "BDTG method" ) << std::endl;
    return ele_reader_high_fb->EvaluateMVA( "BDTG method" );
  }
  else if (iElectron.pt() >= 10 && fabs(iElectron.eta()) > 1.479) {
    // std::cout << "LepMVA: " << ele_reader_high_ec->EvaluateMVA( "BDTG method" ) << std::endl;
    return ele_reader_high_ec->EvaluateMVA( "BDTG method" );
  }
  else if (iElectron.pt() < 10 && fabs(iElectron.eta()) <= 0.8) {
    // std::cout << "LepMVA: " << ele_reader_low_cb->EvaluateMVA( "BDTG method" ) << std::endl;
    return ele_reader_low_cb->EvaluateMVA( "BDTG method" );
  }
  else if (iElectron.pt() < 10 && fabs(iElectron.eta()) > 0.8 && fabs(iElectron.eta()) <= 1.479) {
    // std::cout << "LepMVA: " << ele_reader_low_fb->EvaluateMVA( "BDTG method" ) << std::endl;
    return ele_reader_low_fb->EvaluateMVA( "BDTG method" );
  }
  else {
    // std::cout << "LepMVA: " << ele_reader_low_ec->EvaluateMVA( "BDTG method" ) << std::endl;
    return ele_reader_low_ec->EvaluateMVA( "BDTG method" );
  }
}
