#!/usr/bin/env python
import sys
import socket
import time
import os
import numpy as n
import importlib
plot_helper = importlib.import_module('ttH-13TeVMultiLeptons.DrawPlots.utilities.plot_helper', None)
from argparse import ArgumentParser
import ROOT
import yaml
ROOT.gSystem.Load('libttH-13TeVMultiLeptonsTemplateMakers.so')

def main():
    ROOT.gROOT.SetBatch(1)
    parser = ArgumentParser(description='Make plots from summary trees.')
    parser.add_argument('config_file_name', nargs='?', default='multilepton.yaml', help='Configuration file to process.')
    parser.add_argument('-b', '--batch', action='store_true', help='Batch mode: this submits one sample per condor job.')
    parser.add_argument('-p', '--pdf', action='store_true', help='Save a PDF of each plot. Default is not to save a PDF.')
    parser.add_argument('-w', '--web', action='store_true', help='Post each plot to the user\'s AFS space.')
    parser.add_argument('-l', '--lepton_category', help='Run on a single lepton category.  Default is to run on all lepton categories listed in the configuration file.')
    parser.add_argument('-j', '--jet_tag_category', help='Run on a single jet tag category.  Default is to run on all jet tag categories listed in the configuration file.')
    parser.add_argument('-n', '--no_weights', action='store_true', help='Don\'t apply any normalization or weights.')
    parser.add_argument('-f', '--file', help='Run on a single file.  (Must also specify which sample it is with --sample.)')
    parser.add_argument('-s', '--sample', help='Run on a single sample.  Default is to run on all samples listed in the configuration file.')
    parser.add_argument('--label', help='Override the label set in the configuration file with LABEL')
    parser.add_argument('-lm', '--limits', action='store_true', help='Limit mode: this uses systematics and distributions for limits.')
    args = parser.parse_args()

    #print os.environ['HOSTNAME']

    with open(args.config_file_name) as config_file:
        config = yaml.load(config_file)

    if args.label:
        config['label'] = args.label

    #use common systematics and weight as default for all samples
    samples = dict((k, v if v else {'systematics':['common'], 'weights':['common']}) for k, v in config['samples'].items())
    if args.sample:
        samples = dict(((k, v) for k, v in samples.items() if k == args.sample))

    jet_tag_categories = config['jet tag categories']
    if args.jet_tag_category:
        jet_tag_categories = dict(((k, v) for k, v in config['jet tag categories'].items() if k == args.jet_tag_category))

    lepton_categories = config['lepton categories'].keys()
    if args.lepton_category:
        lepton_categories = [args.lepton_category]

    output_directory = config['output directory']
    if args.limits:
        output_directory = config['limits output directory']

    plot_helper.make_sure_directories_exist([os.path.join(output_directory, category) for category in lepton_categories])

    if args.web:
        www_plot_directories = [os.path.join('plots', config['label'], output_directory, lepton_category) for lepton_category in lepton_categories]
        plot_helper.setup_web_posting(www_plot_directories, 4, args.config_file_name)
    
    fileid = ''
    if args.file:
        tempfilestr=args.file
        strpos = tempfilestr.rfind("/")
        fileid = '_'+tempfilestr[strpos+1:-5] # keeps string between last "/" and ".root"
	print fileid
    
    if args.batch:
        #submit_batch_jobs(config, samples, lepton_categories, jet_tag_categories)
	#os.popen('printenv')
	submit_batch_jobs_lxplus(config, samples, lepton_categories, jet_tag_categories)
    else:
        make_histos(args, config, samples, lepton_categories, jet_tag_categories, fileid)

    if args.web:
        if args.batch:
            print '\nFinished submitting jobs.  After they complete, plots will be posted to: http://www.crc.nd.edu/~%s/plots/%s/' % (os.environ['USER'], config['label'])
        else:
            plot_helper.update_indexes(output_directory)
            print '\nFinished processing.  Plots will be posted to: http://www.crc.nd.edu/~%s/plots/%s/' % (os.environ['USER'], config['label'])

def make_histos(args, config, samples, lepton_categories, jet_tag_categories, fileid):
    for sample, sample_dict in samples.items():
        print " "
	print(socket.gethostname())
        tree_sample = sample_dict.get('tree sample', sample)
        additional_cuts = sample_dict.get('additional cuts', [])
        cuts_to_remove = sample_dict.get('cuts to remove', [])
        sample_info = plot_helper.SampleInformation(tree_sample)

        for lepton_category in lepton_categories:
            lepton_category_cut_strings = config['lepton categories'][lepton_category].get('cuts', {}).values()
            if sample_info.sample_type == 'data' or 'sideband' in sample_info.sample_type:
                if any([x==sample for x in config['lepton categories'][lepton_category].get('excluded samples', [])]):
                    config['weights'].append('0') #So we get empty histograms for hadding to get the inclusive category
                    #continue
                if not plot_helper.is_matching_data_sample(config['lepton categories'][lepton_category]['data samples'], sample):
                    continue

            for jet_tag_category, jet_tag_category_cut_strings in jet_tag_categories.items():
                systematics_list = plot_helper.customize_systematics(config['systematics'], sample_dict.get('systematics', 'common'))
                output_file_name = '%s/%s/%s_%s_%s_%s%s.root' % (config['output directory'], lepton_category, lepton_category, jet_tag_category, sample, config['output label'], fileid)
                if args.limits:
                    output_file_name = '%s/%s/%s_%s_%s_%s.root' % (config['limits output directory'], lepton_category, lepton_category, jet_tag_category, sample, config['output label'])
                    if config['limits skip systematics']:
                        systematics_list = ['nominal']
                elif config['skip systematics']:
                    systematics_list = ['nominal']

                output_file = ROOT.TFile(output_file_name, 'RECREATE')

                for systematic in systematics_list:
                    print 'Beginning next loop iteration. Sample: %10s Jet tag category: %-10s  Lepton category: %-10s Systematic: %-10s' % (sample, jet_tag_category, lepton_category, systematic)

                    systematic_weight_string, systematic_label = plot_helper.get_systematic_info(systematic)
                    #source_file_name = '%s/%s_%s_all.root' % (config['input_trees_directory'], tree_sample, config['label'])
                    source_file_name = '%s/%s/*.root' % (config['input_trees_directory'], tree_sample) ## will this work?
                    if 'JES' in systematic:
                        source_file_name = '%s/%s_%s_%s_all.root' % (config['input_trees_directory'], tree_sample, config['label'], systematic)
			
                    if args.file:
                        source_file_name = args.file
                    
		    		#source_file = ROOT.TFile(source_file_name)
		    		#tree = source_file.Get('summaryTree')
                    ## this is much better:
#                    source_chain = ROOT.TChain('OSTwoLepAna/summaryTree')
                    source_chain = ROOT.TChain('summaryTree')
                    source_chain.Add(source_file_name)
                    tree = source_chain.CloneTree()
                    thechainentries = tree.GetEntries()
                    #thechainentries = source_chain.GetEntries()
                    print thechainentries
                    draw_string_maker = plot_helper.DrawStringMaker()
                    # uncomment this eventually:
                    draw_string_maker.append_selection_requirements(config['common cuts'].values(),
                                                lepton_category_cut_strings,
                                                jet_tag_category_cut_strings,
                                                additional_cuts) #additional_cuts is empty by default

                    draw_string_maker.remove_selection_requirements(cuts_to_remove)
		    		#draw_string_maker.append_selection_requirements('true') # hack

                    if not args.no_weights:
                        weights = plot_helper.customize_list(config['weights'], sample_dict.get('weights', ['common']))
                        if sample_info.sample_type == 'MC' and 'triggerSF' in weights:
                            matched_SF = draw_string_maker.get_matched_SF(lepton_category)
                            weights = [matched_SF if x=='triggerSF' else x for x in weights]
                        draw_string_maker.multiply_by_factors(weights, [systematic_weight_string])

                    if sample_info.sample_type not in ['MC', 'data'] and 'sideband' not in sample_info.sample_type:
                        sys.exit('Invalid sample_type must be data, sideband, or MC' % (sample_info.sample_type))

                    config = plot_helper.append_integral_histo(config)
                    distribution_items = config['distributions'].items()
                    if args.limits:
                        distribution_items = config['limits distributions'].items()
		    
#                    for distribution, parameters in distribution_items:
#                        if sample not in parameters.get('samples', [sample]):
#                            continue
#                        draw_string_maker.remove_selection_requirements(parameters.get('cuts to remove', []))
#                        draw_string_maker.append_selection_requirements(parameters.get('additional cuts', []))
#                        plot_name = '%s%s' % (distribution, systematic_label)
#                        print " " 
#                        print draw_string_maker.draw_string
#                        print " "
#					    ##################################################################################################
#                        plot = plot_helper.Plot(sample, output_file, tree, plot_name, parameters, draw_string_maker.draw_string) ## hack: replaced draw_string_maker.draw_string with ''
#					    ##################################################################################################
#					    
#                        if sample_info.sample_type == 'MC':
#                            plot.plot.Scale(sample_info.x_section * config['luminosity'] / sample_info.num_generated)
#                        output_file.Write()
#                        if args.pdf:
#					        plot.save_image('pdf')
#                        if args.web:
#                            plot.post_to_web(config, lepton_category)
		    
		    
		    if (tree.GetEntries()<=0):
		    	continue
		    
		    #failed experiment:
		    
#		    # set up pointers to branches:
#		    branchlist = tree.GetListOfBranches()
#		    branchstrlist = []
#		    for ibranch in branchlist:
#		    	branchstrlist.append(ibranch.GetName())
#		    branch = {}
#		    tree.GetEntry(0)
#		    for branchstr in branchstrlist:
#		        dummy = -9999.
#			#adummy = []
#			exec("dummy = tree.%s" % (branchstr))
#			#adummy.append(dummy)
#		    	adummy = n.array([dummy])
#			branch[branchstr] = adummy
#			print branchstr
#			print branch[branchstr]
#			tree.SetBranchAddress(branchstr, branch[branchstr])
#		    		    
#		    dummy = [0]
#		    adummy = n.array([dummy])
#		    branch["allLeptonGenGrandMotherId"] = adummy
#		    tree.SetBranchAddress("allLeptonGenGrandMotherId", branch["allLeptonGenGrandMotherId"])
#		    dummy = [0]
#		    adummy = n.array([dummy])
#		    branch["allLeptonGenMotherId"] = adummy
#		    tree.SetBranchAddress("allLeptonGenMotherId", branch["allLeptonGenMotherId"])
#		    dummy = [0]
#		    adummy = n.array([dummy])
#		    branch["allLeptonTkCharge"] = adummy
#		    tree.SetBranchAddress("allLeptonTkCharge", branch["allLeptonTkCharge"])
#		    dummy = tree.raw_electrons
#		    adummy = n.array([dummy])
#		    branch["preselected_electrons"] = adummy
#		    tree.SetBranchAddress("preselected_electrons", branch["preselected_electrons"])
#		    dummy = tree.raw_electrons
#		    adummy = n.array([dummy])
#		    branch["loose_electrons"] = adummy
#		    tree.SetBranchAddress("loose_electrons", branch["loose_electrons"])
#		    dummy = tree.loose_leptons
#		    adummy = n.array([dummy])
#		    branch["tight_leptons"] = adummy
#		    tree.SetBranchAddress("tight_leptons", branch["tight_leptons"])
#		    dummy = tree.raw_electrons
#		    adummy = n.array([dummy])
#		    branch["tight_electrons"] = adummy
#		    tree.SetBranchAddress("tight_electrons", branch["tight_electrons"])
#		    dummy = tree.raw_muons
#		    adummy = n.array([dummy])
#		    branch["tight_muons"] = adummy
#		    tree.SetBranchAddress("tight_muons", branch["tight_muons"])
#		    dummy = tree.preselected_jets
#		    adummy = n.array([dummy])
#		    branch["loose_bJets"] = adummy
#		    tree.SetBranchAddress("loose_bJets", branch["loose_bJets"])
		    
		    
		    # set up hists:
		    theplots = []
		    plotdict = {}
		    plotcount = 0
		    for distribution, parameters in distribution_items:
                	if sample not in parameters.get('samples', [sample]):
                            continue
                	
			draw_string_maker.remove_selection_requirements(parameters.get('cuts to remove', []))
                	draw_string_maker.append_selection_requirements(parameters.get('additional cuts', []))
                        plot_name = '%s%s' % (distribution, systematic_label)
			
			##################################################################################################
                	plot = plot_helper.GeoffPlot(sample, output_file, plot_name, parameters, draw_string_maker.draw_string)
			##################################################################################################
			
			plotdict[plot_name] = plotcount
			theplots.append(plot)
			plotcount += 1
		    
		    #print draw_string_maker.draw_string
		    
		    treecount = 0
		    
		    for entry in tree:
		        #print "blah2"
			treecount += 1
			thecut = plot_helper.CheckCuts(draw_string_maker.draw_string, entry)

			if thecut is 0:
			    continue;
			
			for distribution, parameters in distribution_items:
			    plot_name = '%s%s' % (distribution, systematic_label)
			    #print parameters
			    #print draw_string_maker.draw_string
			    theplots[plotdict[plot_name]].fill(parameters, draw_string_maker.draw_string, entry, thecut) #, branch)
			if ((treecount % 10000)==0):
			    print treecount
			    
		    for thisplot in theplots:
		        if sample_info.sample_type == 'MC':
                            thisplot.plot.Scale(sample_info.x_section * config['luminosity'] / sample_info.num_generated)
                    
		    output_file.Write()		    
		    
                    #source_file.Close() #end systematic
                    source_chain.Reset() #end systematic
                config_file = ROOT.TObjString(args.config_file_name)
                output_file.cd()
                config_file.Write('config_file')
                output_file.Close() #end jet tag category
    print " "
    print "done"

def submit_batch_jobs(config, samples, lepton_categories, jet_tag_categories):
    plot_helper.make_sure_directories_exist(['batch_logs/%s' % config['label']])

    argument_string = ' '.join([a for a in sys.argv[1:] if a != '-b' and a != '-batch'])

    condor_header = 'universe = vanilla \nexecutable = make_histos.py \nnotification = Never \ngetenv = True \n+IsExpressJob = False'
    ### Setting to run on 32 open cores on earth
    #condor_header = 'universe = vanilla \nexecutable = make_histos.py \nnotification = Never \ngetenv = True \n+IsExpressJob = True'
    for sample in samples:
        for lepton_category in lepton_categories:
            for jet_tag_category in jet_tag_categories:
                condor_submit_file = open('make_histos_batch.submit', 'w')
                condor_submit_file.write(condor_header)
                condor_submit_file.write('\narguments = -s %s -l %s -j %s %s' % (sample, lepton_category, jet_tag_category, argument_string))
                condor_submit_file.write('\nlog = batch_logs/%s/%s_%s_%s_%s.log' % (config['label'], config['label'], sample, jet_tag_category, lepton_category))
                condor_submit_file.write('\noutput = batch_logs/%s/%s_%s_%s_%s.stdout' % (config['label'], config['label'], sample, jet_tag_category, lepton_category))
                condor_submit_file.write('\nerror = batch_logs/%s/%s_%s_%s_%s.stderr' % (config['label'], config['label'], sample, jet_tag_category, lepton_category))
                #condor_submit_file.write('\nrequirements = (machine != "skynet005.crc.nd.edu" && machine != "skynet006.crc.nd.edu" && machine != "skynet008.crc.nd.edu")')
                condor_submit_file.write('\nRequestMemory = 1000')
                condor_submit_file.write('\nqueue 1')
                condor_submit_file.close()

                os.popen('condor_submit make_histos_batch.submit')
                print '\nSubmitting batch jobs for sample %s, lepton category %s, jet tag category %s... ' % (sample, lepton_category, jet_tag_category)


def submit_batch_jobs_lxplus(config, samples, lepton_categories, jet_tag_categories):
    
    thispwd = os.getcwd()
    plot_helper.make_sure_directories_exist(['batch_logs/%s' % config['label']])
    argument_string = ' '.join([a for a in sys.argv[1:] if a != '-b' and a != '-batch'])
    
    for sample in samples:
        
        for lepton_category in lepton_categories:
            for jet_tag_category in jet_tag_categories:             
		thecommand = 'ssh -f lxplus "cd %s; cmsenv; ./make_histos.py -s %s -l %s -j %s %s >& batch_logs/%s/%s_%s_%s_%s.log"' % (thispwd, sample, lepton_category, jet_tag_category, argument_string, config['label'], config['label'], sample, jet_tag_category, lepton_category)
                os.popen(thecommand)
                print '\nSubmitting batch jobs for sample %s, lepton category %s, jet tag category %s... ' % (sample, lepton_category, jet_tag_category)
		time.sleep(5)
	time.sleep(60)


if __name__ == '__main__':
    main()
