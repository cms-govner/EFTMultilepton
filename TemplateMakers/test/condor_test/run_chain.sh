#!/bin/sh

sample=$1
job_no=$2

cd /afs/crc.nd.edu/user/c/cmuelle2/CMSSW_8_0_26_patch1/src/ttH-13TeVMultiLeptons/TemplateMakers/test/
export SCRAM_ARCH=slc6_amd64_gcc530
eval `scramv1 runtime -sh`
hostname
root -l -b -q runChain.C\(\""$sample"\",\""2lss_sr"\",${job_no}\)
#sleep 180
date
#printenv
