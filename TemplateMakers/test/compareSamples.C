//Charlie Mueller 4/7/2016
#include <iostream>
#include "TSystem.h"
#include <vector>
#include "TH1.h"
#include "TChain.h"
#include <string>
#include <algorithm>
#include "TString.h"
#include "TH1D.h"
#include "TFile.h"
#include "TLine.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TPad.h"
#include <cmath>
#include "TLorentzVector.h"
#include "TLegend.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "EFTMultilepton/TemplateMakers/src/classes.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "selection.h"
#include "loadSamples_notreDame.h"
#include "treeTools.h"

/////////////////////////////////////////
///
/// usage: root -l compareSamples.C+
///
/////////////////////////////////////////

class PlotObject
{
private:
  TH1D *reco_score_;

  int hist_color_;

  void FillHist(TString sample_name_input_, TString leg_name_)
  {
    TChain *chain = new TChain("extraction_tree_recobdt_v1");
    
    TString file_ = getSelectionFile(sample_name_input_);
    chain->Add(file_);
    int chainentries = chain->GetEntries();
    
    double mcwgt_intree = -999.;
    double reco_score_intree = -99.;
    bool isBtight_intree = false;
    

    chain->SetBranchAddress("mcwgt", &mcwgt_intree);
    chain->SetBranchAddress("reco_score", &reco_score_intree);
    chain->SetBranchAddress("bTight_category", &isBtight_intree);

    Int_t cachesize = 250000000;   //250 MBytes
    chain->SetCacheSize(cachesize);

    for (int i=0; i<chainentries; i++)
      {
    	chain->GetEntry(i);
    	printProgress(i,chainentries);

	/// messy
	vector<ttH::Lepton> lep_eta_collection;
	if (sample_name_input_ == "tth_aMC" && leg_name_ == "tth_btight" && isBtight_intree) reco_score_->Fill(reco_score_intree, mcwgt_intree);
	else if (sample_name_input_ == "tth_aMC" && leg_name_ == "tth_bloose" && !isBtight_intree) reco_score_->Fill(reco_score_intree, mcwgt_intree);
	else if (sample_name_input_ == "fakes" && leg_name_ == "ttbar_btight" && isBtight_intree) reco_score_->Fill(reco_score_intree, mcwgt_intree);
	else if (sample_name_input_ == "fakes" && leg_name_ == "ttbar_bloose" && !isBtight_intree) reco_score_->Fill(reco_score_intree, mcwgt_intree);
	 
      }
    
    hist_vector.push_back(reco_score_);

    for (const auto & my_hist : hist_vector)
      {
	my_hist->Sumw2();
	my_hist->Scale( 1.0 / my_hist->Integral());
	
	my_hist->SetStats(0);
	my_hist->SetLineWidth(2);
	my_hist->SetLineColor(hist_color_);
	my_hist->SetMarkerColor(hist_color_);
	my_hist->GetYaxis()->SetTitle("normalized units");
	my_hist->GetYaxis()->SetTitleOffset(1.37);
	my_hist->GetYaxis()->SetLabelSize(0.025);
      }
  }
public:
  PlotObject(TString sample_name_="sample_name", int line_color_=1, TString leg_name_="legend_name")
  { 
    hist_color_ = line_color_;

    reco_score_ = new TH1D(leg_name_,"reco_score",40,-1,1.1);
    reco_score_->GetXaxis()->SetTitle("reco score");

    FillHist(sample_name_, leg_name_);

  }//default constructor

  vector<TH1D*> hist_vector;
  virtual ~PlotObject(){};
};


void drawPlots(vector<PlotObject> samples_)
{
  
  for (unsigned int i=0; i < samples_[0].hist_vector.size(); i++)
    {
      TString title = samples_[0].hist_vector[i]->GetTitle();
      TCanvas* can = new TCanvas(title,title,10,32,530,580);
      TPad* pad1 = new TPad("pad1","pad1",0,0.2,1,1);
      TLegend *leg = new TLegend(0.5454545,0.8240072,0.8787879,0.9187726,NULL,"brNDC");
      leg->SetBorderSize(0);
      double hist_max_val = 0.;
      TString can_save_name = title + ".pdf";

      pad1->SetTopMargin(0.03);
      pad1->SetBottomMargin(0);
      pad1->Draw();
      pad1->cd();
    
      for (const auto & sample : samples_)
	{
	  if ( sample.hist_vector[i]->GetMaximum() > hist_max_val) hist_max_val = sample.hist_vector[i]->GetMaximum();	  
	}
      samples_[0].hist_vector[i]->GetYaxis()->SetRangeUser(0, 1.2*hist_max_val);

      int j = 0;
      for (const auto & sample : samples_)
	{
	  sample.hist_vector[i]->SetTitle("");
	  //use max val to keep track of first iteration
	  if ( j == 0 ) sample.hist_vector[i]->DrawCopy("hist");
	  else  sample.hist_vector[i]->DrawCopy("histsame");
	
	  leg->AddEntry(sample.hist_vector[i], sample.hist_vector[i]->GetName(), "l");
	  j++;
	}  

      double x_min = samples_[0].hist_vector[i]->GetXaxis()->GetXmin();
      double x_max = samples_[0].hist_vector[i]->GetXaxis()->GetXmax();

      leg->Draw("same");
      can->cd();

      TPad* pad2 = new TPad("pad2","pad2",0,0,1,0.2);
      TLine *line = new TLine(x_min, 1., x_max, 1.);
      auto hist_ratio =  *(samples_[0].hist_vector[i]);
      hist_ratio.SetLineWidth(1);
      //      hist_ratio.Sumw2();
      hist_ratio.Divide( samples_[1].hist_vector[i] );
      pad2->SetTopMargin(0);
      pad2->SetBottomMargin(0.35);
      //      pad2->SetGridy();
      

      pad2->Draw();
      pad2->cd();
      
      hist_ratio.GetYaxis()->SetTitle("ratio");
      double offset_fraction = 1.6;//1.6;
      double hist_max = hist_ratio.GetMaximum();
      double hist_min = hist_ratio.GetMinimum();
      double y_max = 1 + offset_fraction*max(abs(hist_max - 1), abs(hist_min - 1 ));
      double y_min = 1 - offset_fraction*max(abs(hist_max - 1), abs(hist_min - 1 )); 
      //    hist_ratio.GetYaxis()->SetRangeUser( y_min, y_max );
      //      hist_ratio.GetYaxis()->SetRangeUser(-.5,2.5);
      hist_ratio.GetYaxis()->SetRangeUser(.25,1.75);
      hist_ratio.SetLineColor(1);
      hist_ratio.SetMarkerColor(1);
      hist_ratio.SetMarkerStyle(20);
      hist_ratio.SetMarkerSize(0.6);
      hist_ratio.GetYaxis()->SetLabelSize(.1);
      hist_ratio.GetXaxis()->SetLabelSize(.16);
      hist_ratio.GetXaxis()->SetTitle( samples_[0].hist_vector[i]->GetXaxis()->GetTitle() );
      hist_ratio.GetXaxis()->SetTitleSize(0.16);
      hist_ratio.GetXaxis()->SetTitleFont(42);
      hist_ratio.GetYaxis()->SetTitleOffset(.45);
      hist_ratio.GetYaxis()->SetTitleSize(.12);
      hist_ratio.GetYaxis()->CenterTitle();
      hist_ratio.DrawCopy("ep");
      line->Draw("same");
      
      can->SaveAs(can_save_name);
    }
}

void compareSamples(void)
{
  PlotObject sample1 = PlotObject("tth_aMC",2,"tth_btight"); //1 filter, train, fo
  PlotObject sample2 = PlotObject("tth_aMC",4,"tth_bloose"); //2 no filter, train, fo
  PlotObject sample3 = PlotObject("fakes",8,"ttbar_btight"); //3 filter, train, ps
  PlotObject sample4 = PlotObject("fakes",6,"ttbar_bloose"); //3 filter, train, ps

  
  vector<PlotObject> plot_vector;
  plot_vector.push_back(sample1);
  plot_vector.push_back(sample2);
  plot_vector.push_back(sample3);
  plot_vector.push_back(sample4);

  
  drawPlots(plot_vector);
}

