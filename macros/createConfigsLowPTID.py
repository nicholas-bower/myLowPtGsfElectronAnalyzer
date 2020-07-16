import os, sys, random
import numpy as np



#pufile='file:root://xrootd.unl.edu/'+pufilelist[random.randint(0,len(pufilelist))]

#print pufile

configDir="./configs/"

outputPrefix="root://cmseos.fnal.gov//store/user/nbower/Events/ALP/HLT/"
inputPrefix = "root://cmseos.fnal.gov//store/user/nbower/Events/ALP/HLT/"
#masses=[30, 50]
masses=[10]

jobs=np.linspace(100,1,100)
#jobs=[2,68,92,93,94,95,96,97,98,99,100]
#jobs=[41, 49]
listdirectory = '/uscms_data/d3/nbower/FSU/TestLowPt/CMSSW_10_6_12/src/myLowPtGsfElectronsAnalyzer/myLowPtGsfElectronsAnalyzer/macros/fileLists/QCD_AOD/'
for file in os.listdir(listdirectory):
    cfg=open(configDir+file.replace(".txt","_cfg.py"),"w")
    cfg.writelines("""
# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: test2 -s RAW2DIGI,L1Reco,RECO --datatier RECO --era=Run2_2018 --conditions auto:phase1_2018_realistic --eventcontent RECO --filein file:test.root --no_exec
import FWCore.ParameterSet.Config as cms
#f = open("/uscms_data/d3/nbower/FSU/TestLowPt/CMSSW_10_6_12/src/myLowPtGsfElectronsAnalyzer/myLowPtGsfElectronsAnalyzer/macros/fileLists/m50_ALP_fileList.txt","r")
f = open('./myLowPtGsfElectronsAnalyzer/macros/fileLists/QCD_AOD/"""+file+"""','r')

infiles = f.readlines()
f.close()
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')
options.setDefault('maxEvents',-1)
options.setDefault('inputFiles',infiles)


options.parseArguments()



process = cms.Process('TEST') # ,eras.bParkingOpen

# import of standard configurations


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)



# Output definition


# Path and EndPath definitions
process.load('myLowPtGsfElectronsAnalyzer/myLowPtGsfElectronsAnalyzer/myLowPtGsfElectronsAnalyzer_cfi')



# Schedule definition

process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string('root://cmseos.fnal.gov//store/user/nbower/plots/QCD_LowPT/"""+file.replace("txt","root")+"""')
                                   )


process.p = cms.Path(process.simple)
# Customisation from command line


# End adding early deletion

#open('pydump.py','w').write(process.dumpPython())

""")
