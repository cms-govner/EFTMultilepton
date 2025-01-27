#!/bin/bash
#usage: ./run_limits.sh fatStackPlots.root

hists_file=$1
file_path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/$hists_file"

if [ $hists_file ] && [ -a $file_path ]
    then
    cd ~/CMSSW_7_4_16_patch2_combine/src/
    export SCRAM_ARCH=slc6_amd64_gcc491
    eval `scramv1 runtime -sh`
#    ./mk_datacard $file_path
    combine -M Asymptotic -m125 -d limits/125/ttH_ttl.txt #limits
    combine -M MaxLikelihoodFit -m125 -d limits/125/ttH_ttl.txt #best fit mu
    echo -e "\n ======= Observed Significance ======= \n"
    combine -M ProfileLikelihood -v 1 --significance -m125 -d limits/125/ttH_ttl.txt
    echo -e "\n ======= Expected Significance ======= \n"
    combine -M ProfileLikelihood -v 1 --significance --expectSignal=1 -t -1 -m125 -n Expected -d limits/125/ttH_ttl.txt
    
else
    echo "Please specify file containing hists to calculate limits from."
    echo "Example: ./run_limits.sh card_hists.root"
fi