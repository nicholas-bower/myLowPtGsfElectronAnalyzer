#!/bin/tcsh

setenv CMSSW_BASE /uscms_data/d3/nbower/FSU/TestLowPt/CMSSW_10_6_12/

cd $CMSSW_BASE/src/myLowPtGsfElectronsAnalyzer 

tar -zcvf ../../../CMSSW.tgz ../../../CMSSW_10_6_12/ --exclude="*.root" --exclude="*.pdf" --exclude="*.gif" --exclude=.git --exclude="*.log" --exclude="*stderr" --exclude="*stdout" --exclude='*.png'

eosrm /eos/uscms/store/user/nbower/CMSSW.tgz

xrdcp ../../../CMSSW.tgz root://cmseos.fnal.gov//eos/uscms/store/user/nbower/CMSSW.tgz
#setenv PYTH ./src/plotRecoIdEffQCD.py     
setenv PROC _LOWPT_
setenv SAMP QCD
setenv RUNDIR ./configs/
set FILE = './myLowPtGsfElectronsAnalyzer/configs/*.py'
#set FILE = './filelists/TCP/*.txt'  
echo "$FILE"
@ N = 1
echo $N
#cd $FILE
foreach DIR ($FILE)
    #set DIR += /*
    #foreach FI  ($DIR)
#foreach MASS (10)
    #foreach JOB (2 68 92 93 94 95 96 97 98 99 100)
    #foreach JOB (31)
    echo $DIR
    setenv INFI $DIR
#set HT = $(echo $DIR| cut -d'' -f 2)
    setenv PYTH $DIR
    setenv JOBNUMBER $SAMP$PROC$N 
    echo $PYTH 
    echo $JOBNUMBER
    @ N+=1
    condor_submit ./condor.jdl     
end
