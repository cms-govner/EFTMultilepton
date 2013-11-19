#!/usr/bin/env python
import sys
import os
from ttHMultileptonAnalysis.DrawPlots.utilities.configparser import *
import ttHMultileptonAnalysis.DrawPlots.utilities.plot_helper as plot_helper
from argparse import ArgumentParser
import ROOT
import yaml

def main():
    parser = ArgumentParser(description='Make plots from summary trees.')
    parser.add_argument('config_file_name', nargs='?', default='multilepton.yaml', help='Configuration file to process.')
    parser.add_argument('-b', '--batch', action='store_true', help='Batch mode: this submits one sample per condor job.')
    parser.add_argument('-p', '--pdf', action='store_true', help='Save a PDF of each plot. Default is not to save a PDF.')
    parser.add_argument('-w', '--web', action='store_true', help='Post each plot to the user\'s AFS space.')
    parser.add_argument('-l', '--lepton_category', action='append', help='Run on a single lepton category.  Default is to run on all lepton categories listed in the configuration file.')
    parser.add_argument('-n', '--no_weights', action='store_true', help='Don\'t apply any normalization or weights.')
    parser.add_argument('-f', '--file', help='Run on a single file.  (Must also specify which sample it is with --sample.)')
    parser.add_argument('-s', '--sample', action='append', help='Run on a single sample.  Default is to run on all samples listed in the configuration file.')
    parser.add_argument('--label', help='Override the label set in the configuration file with LABEL')
    args = parser.parse_args()

    with open(args.config_file_name) as config_file:
        config = yaml.load(config_file)

    if args.label:
        config['label'] = args.label

    samples = config['samples'].keys()
    if args.sample:
        samples = args.sample

    lepton_categories = config['lepton categories']
    if args.lepton_category:
        lepton_categories = args.lepton_category

    plot_helper.make_sure_directories_exist([os.path.join('histos', category) for category in lepton_categories])
    if args.web:
        www_plot_directories = [os.path.join('plots', config['label'], lepton_category) for lepton_category in lepton_categories]
        plot_helper.setup_web_posting(www_plot_directories, 4, args.config_file_name)

    if args.batch:
        submit_batch_jobs(config, samples, lepton_categories)
    else:
        make_histos(args, config, samples, lepton_categories)

    if args.web:
        if args.batch:
            print '\nFinished submitting jobs.  After they complete, plots will be posted to: http://www.crc.nd.edu/~%s/plots/%s/' % (os.environ['USER'], config['label'])
        else:
            print '\nFinished processing.  Plots will be posted to: http://www.crc.nd.edu/~%s/plots/%s/' % (os.environ['USER'], config['label'])

def make_histos(args, config, samples, lepton_categories):
    for sample in samples:
        sample_dict = config['samples'][sample]
        sample_info = plot_helper.SampleInformation(sample, sample_dict)

        for lepton_category in lepton_categories:
            lepton_category_cut_strings = config['%s cuts' % lepton_category].values()
            if sample_info.sample_type == 'data' or 'sideband' in sample_info.sample_type:
                if not plot_helper.get_data_sample_name(lepton_category) in sample:
                    continue

            for jet_tag_category in config['jet tag categories']:
                output_file_name = 'histos/%s/%s_%s_%s_%s.root' % (lepton_category, lepton_category, jet_tag_category, sample, config['label'])
                output_file = ROOT.TFile(output_file_name, 'RECREATE')

                systematics_list = plot_helper.customize_systematics(config['systematics'], sample_info.systematics)
                for systematic in systematics_list:
                    print 'Beginning next loop iteration. Sample: %10s Jet tag category: %-10s  Lepton category: %-10s Systematic: %-10s' % (sample, jet_tag_category, lepton_category, systematic)

                    systematic_weight_string, systematic_label = plot_helper.get_systematic_info(systematic)
                    source_file_name = '%s/%s_%s_all.root' % (config['input_trees_directory'], sample, config['label'])
                    if args.file:
                        source_file_name = args.file
                    source_file = ROOT.TFile(source_file_name)
                    tree = source_file.Get('summaryTree')

                    draw_string_maker = plot_helper.DrawStringMaker()
                    draw_string_maker.append_selection_requirements(config['common cuts'].values(), lepton_category_cut_strings)
                    if sample_info.sample_type == 'NP_sideband':
                        draw_string_maker.append_selection_requirements(config['NP sideband cuts'].values())
                    elif sample_info.sample_type == 'QF_sideband':
                        draw_string_maker.append_selection_requirements(config['QF sideband cuts'].values())
                    else:
                        draw_string_maker.append_selection_requirements(config['regular selection cuts'].values())
                    draw_string_maker.append_jet_tag_category_requirements(jet_tag_category)

                    if not args.no_weights:
                        draw_string_maker.multiply_by_factor(systematic_weight_string)
                    if sample_info.sample_type == 'MC':
                        if 'triggerSF' in config['weights']:
                            matched_SF = draw_string_maker.get_matched_SF(lepton_category)
                            config['weights'] = [matched_SF if x=='triggerSF' else x for x in config['weights']]

                    weights = plot_helper.customize_list(config['weights'], sample_info.weights)
                    draw_string_maker.multiply_by_factors(weights)

                    if sample_info.sample_type not in ['MC', 'data'] and 'sideband' not in sample_info.sample_type:
                        print 'Invalid sample_type %s is neither data, sideband, nor MC' % (sample_info.sample_type)
                        sys.exit()

                    for distribution in config['distributions'].keys():
                        plot_name = '%s_%s_%s_%s%s' % (sample, lepton_category, jet_tag_category, distribution, systematic_label)
                        plot = plot_helper.Plot(sample, output_file, tree, plot_name, config['distributions'][distribution], draw_string_maker.draw_string)
                        if sample_info.sample_type == 'MC':
                            plot.plot.Scale(sample_info.x_section * config['luminosity'] / sample_info.num_generated)
                        if args.pdf:
                            plot.save_image('pdf')
                        if args.web:
                            plot.post_to_web(config, lepton_category)

                    source_file.Close() #end systematic
                output_file.Close() #end jet tag category

def submit_batch_jobs(config, samples, lepton_categories):
    plot_helper.make_sure_directories_exist(['batch_logs'])

    argument_string = ''
    for argument in sys.argv[1:]:
        if argument != '-b' and argument != '-batch':
            argument_string += argument + ' '

    condor_header = 'universe = vanilla \nexecutable = make_histos.py \nnotification = Never \ngetenv = True \n+IsExpressJob = True'
    for sample in samples:
        for lepton_category in lepton_categories:
            condor_submit_file = open('make_histos_batch.submit', 'w')
            condor_submit_file.write(condor_header)
            condor_submit_file.write('\narguments = -s %s -l %s %s' % (sample, lepton_category, argument_string))
            condor_submit_file.write('\nlog = batch_logs/%s_%s_%s.log' % (config['label'], sample, lepton_category))
            condor_submit_file.write('\noutput = batch_logs/%s_%s_%s.stdout' % (config['label'], sample, lepton_category))
            condor_submit_file.write('\nerror = batch_logs/%s_%s_%s.stderr' % (config['label'], sample, lepton_category))
            condor_submit_file.write('\nqueue 1')
            condor_submit_file.close()

            os.popen('condor_submit make_histos_batch.submit')
            print '\nSubmitting batch jobs for sample %s, lepton category %s... ' % (sample, lepton_category)

if __name__ == '__main__':
    main()