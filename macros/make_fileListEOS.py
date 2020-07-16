import subprocess
import sys,string,math,os
import ConfigParser
import glob
import numpy as np


filesPerList=50000


def checkAndMakeDir(dir):
    if not os.path.exists(dir):
        os.mkdir(dir)

def clearDir(dir):
    for fil in glob.glob(dir+"/*"):
        os.remove(fil)

prefix= "root://cmseos.fnal.gov/"
rootFileDir="/store/user/nbower/Events/ALP/m50/"
os.system('xrdfsls' + rootFileDir)
#query = 'eosls '+ rootFileDir
query = 'xrdfs ' + prefix + " ls " + rootFileDir
os.system(query)
files=os.popen(query).read().split()
out = open("/uscms_data/d3/nbower/FSU/TestLowPt/CMSSW_10_6_12/src/myLowPtGsfElectronsAnalyzer/myLowPtGsfElectronsAnalyzer/macros/fileLists/m50_ALP_fileList.txt", "w")
for nf in range(1, len(files)+1):
    filelistIdx=int((nf-1))
    out.write(prefix+files[nf-1]+"\n")
