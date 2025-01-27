
std::vector<PlotObject> getMcSystShapes(PlotObject shape_template)
{  
  std::vector<PlotObject> plots;

  /* TString drawVar = shape_template.drawCommand; */
  /* TString hist_name = shape_template.hist_name; */
  /* double xmin = shape_template.xmin; */
  /* double xmax = shape_template.xmax; */
  /* double bins = shape_template.bins; */

  PlotObject finalShape_jes_up = shape_template;
  finalShape_jes_up.hist_name += "_jes_Up";
  finalShape_jes_up.btag_weight = "cms_tthl_btag_jesup";
  finalShape_jes_up.tree_name += "_jes_up";
  plots.push_back(finalShape_jes_up);

  PlotObject finalShape_jes_down = shape_template;
  finalShape_jes_down.hist_name += "_jes_Down";
  finalShape_jes_down.btag_weight = "cms_tthl_btag_jesdown";
  finalShape_jes_down.tree_name += "_jes_down";
  plots.push_back(finalShape_jes_down);

  //btag systs

  PlotObject finalShape_hfup = shape_template;
  finalShape_hfup.hist_name += "_hfUp";
  finalShape_hfup.btag_weight = "cms_tthl_btag_hfup";
  plots.push_back(finalShape_hfup);

  PlotObject finalShape_hfstats1up = shape_template;
  finalShape_hfstats1up.hist_name += "_hfstats1Up";
  finalShape_hfstats1up.btag_weight = "cms_tthl_btag_hfstats1up";
  plots.push_back(finalShape_hfstats1up);

  PlotObject finalShape_hfstats2up = shape_template;
  finalShape_hfstats2up.hist_name += "_hfstats2Up";
  finalShape_hfstats2up.btag_weight = "cms_tthl_btag_hfstats2up";
  plots.push_back(finalShape_hfstats2up);

  PlotObject finalShape_lfup = shape_template;
  finalShape_lfup.hist_name += "_lfUp";
  finalShape_lfup.btag_weight = "cms_tthl_btag_lfup";
  plots.push_back(finalShape_lfup);

  PlotObject finalShape_lfstats1up = shape_template;
  finalShape_lfstats1up.hist_name += "_lfstats1Up";
  finalShape_lfstats1up.btag_weight = "cms_tthl_btag_lfstats1up";
  plots.push_back(finalShape_lfstats1up);

  PlotObject finalShape_lfstats2up = shape_template;
  finalShape_lfstats2up.hist_name += "_lfstats2Up";
  finalShape_lfstats2up.btag_weight = "cms_tthl_btag_lfstats2up";
  plots.push_back(finalShape_lfstats2up);

  PlotObject finalShape_cerr1up = shape_template;
  finalShape_cerr1up.hist_name += "_cerr1Up";
  finalShape_cerr1up.btag_weight = "cms_tthl_btag_cerr1up";
  plots.push_back(finalShape_cerr1up);

  PlotObject finalShape_cerr2up = shape_template;
  finalShape_cerr2up.hist_name += "_cerr2Up";
  finalShape_cerr2up.btag_weight = "cms_tthl_btag_cerr2up";
  plots.push_back(finalShape_cerr2up);

  PlotObject finalShape_hfdown = shape_template;
  finalShape_hfdown.hist_name += "_hfDown";
  finalShape_hfdown.btag_weight = "cms_tthl_btag_hfdown";
  plots.push_back(finalShape_hfdown);

  PlotObject finalShape_hfstats1down = shape_template;
  finalShape_hfstats1down.hist_name += "_hfstats1Down";
  finalShape_hfstats1down.btag_weight = "cms_tthl_btag_hfstats1down";
  plots.push_back(finalShape_hfstats1down);

  PlotObject finalShape_hfstats2down = shape_template;
  finalShape_hfstats2down.hist_name += "_hfstats2Down";
  finalShape_hfstats2down.btag_weight = "cms_tthl_btag_hfstats2down";
  plots.push_back(finalShape_hfstats2down);

  PlotObject finalShape_lfdown = shape_template;
  finalShape_lfdown.hist_name += "_lfDown";
  finalShape_lfdown.btag_weight = "cms_tthl_btag_lfdown";
  plots.push_back(finalShape_lfdown);

  PlotObject finalShape_lfstats1down = shape_template;
  finalShape_lfstats1down.hist_name += "_lfstats1Down";
  finalShape_lfstats1down.btag_weight = "cms_tthl_btag_lfstats1down";
  plots.push_back(finalShape_lfstats1down);

  PlotObject finalShape_lfstats2down = shape_template;
  finalShape_lfstats2down.hist_name += "_lfstats2Down";
  finalShape_lfstats2down.btag_weight = "cms_tthl_btag_lfstats2down";
  plots.push_back(finalShape_lfstats2down);

  PlotObject finalShape_cerr1down = shape_template;
  finalShape_cerr1down.hist_name += "_cerr1Down";
  finalShape_cerr1down.btag_weight = "cms_tthl_btag_cerr1down";
  plots.push_back(finalShape_cerr1down);

  PlotObject finalShape_cerr2down = shape_template;
  finalShape_cerr2down.hist_name += "_cerr2Down";
  finalShape_cerr2down.btag_weight = "cms_tthl_btag_cerr2down";
  plots.push_back(finalShape_cerr2down);
  
  return plots;
}

std::vector<PlotObject> getFrSystShapes(PlotObject shape_template)
{  
  std::vector<PlotObject> plots;
  //fake rate systs

  PlotObject finalShape_fr_up = shape_template;
  finalShape_fr_up.hist_name += "_fr_mva090_Up";
  finalShape_fr_up.fr_weight = "fr_mva090_up";
  plots.push_back(finalShape_fr_up);

  PlotObject finalShape_fr_down = shape_template;
  finalShape_fr_down.hist_name += "_fr_mva090_Down";
  finalShape_fr_down.fr_weight = "fr_mva090_down";
  plots.push_back(finalShape_fr_down);

  PlotObject finalShape_fr_pt1 = shape_template;
  finalShape_fr_pt1.hist_name += "_fr_mva090_ptUp";
  finalShape_fr_pt1.fr_weight = "fr_mva090_pt1";
  plots.push_back(finalShape_fr_pt1);

  PlotObject finalShape_fr_pt2 = shape_template;
  finalShape_fr_pt2.hist_name += "_fr_mva090_ptDown";
  finalShape_fr_pt2.fr_weight = "fr_mva090_pt2";
  plots.push_back(finalShape_fr_pt2);

  PlotObject finalShape_fr_be1 = shape_template;
  finalShape_fr_be1.hist_name += "_fr_mva090_beUp";
  finalShape_fr_be1.fr_weight = "fr_mva090_be1";
  plots.push_back(finalShape_fr_be1);

  PlotObject finalShape_fr_be2 = shape_template;
  finalShape_fr_be2.hist_name += "_fr_mva090_beDown";
  finalShape_fr_be2.fr_weight = "fr_mva090_be2";
  plots.push_back(finalShape_fr_be2);

  return plots;
}

