import ROOT
import sys
import matplotlib.pyplot as plt
import numpy as np
BFile = "/uscms_data/d3/nbower/FSU/TestLowPt/CMSSW_10_6_12/src/myLowPtGsfElectronsAnalyzer/QCD_LowPTAOD_hists.root"
SFile = "/uscms_data/d3/nbower/FSU/TestLowPt/CMSSW_10_6_12/src/myLowPtGsfElectronsAnalyzer/h_ALP_M10ID_Hists.root"
signal =ROOT.TFile.Open(SFile ,"READ")
background =ROOT.TFile.Open(BFile ,"READ")

sig_Background = ROOT.TH1F ("sig_Background", " Signal vs Sqrt(Background)", 50 ,0, 9.5  )
sig = ROOT.TH1F('sig', 'Signal Integrals',  50 ,0, 9.5)
bkg = ROOT.TH1F('bkg', 'BKG Integrals',  50 ,0, 9.5)

c = ROOT.TCanvas("c","c",600,600)
c.SetBorderSize(0)
c.SetFrameBorderMode(0)
ROOT.gStyle.SetTitleBorderSize(0)
ROOT.gStyle.SetOptStat(0)

QCD_ID = background.Get('simple/idHist')
QCD_ID.Draw()
QCD_ID.Scale(1/QCD_ID.Integral())
TCP_ID=signal.Get('simple/idHist')
print "TCP integral = " +str( TCP_ID.Integral())
TCP_ID.Scale(1/TCP_ID.Integral())

binwidth = 9.5/50

for i in range (1,50):
    s_point=TCP_ID.Integral(i,50)
    sig.SetBinContent(i, s_point)
    b_point = QCD_ID.Integral(i,50)
    bkg.SetBinContent(i,b_point)
    b_point = np.sqrt(b_point)
    print str(b_point.item())
    #print "sqrt bkg = "+str(type(b_point))+ "spoint = "+ str(type(s_point))
    if b_point!=0:
        sob=s_point/b_point.item()
    else:
        sob=0
    sig_Background.SetBinContent(i,sob)

print 'Max Sig/ background occurs at  = '+ str(sig_Background.GetMaximumBin()) + ', ID Val = ' + str((sig_Background.GetMaximumBin()-1)*binwidth)
sig_Background.Draw()
c.SaveAs('/uscms_data/d3/nbower/FSU/TestLowPt/CMSSW_10_6_12/src/myLowPtGsfElectronsAnalyzer/myLowPtGsfElectronsAnalyzer/graphs/M10_SignalVBackground.png')
sig.Draw()
c.SaveAs('/uscms_data/d3/nbower/FSU/TestLowPt/CMSSW_10_6_12/src/myLowPtGsfElectronsAnalyzer/myLowPtGsfElectronsAnalyzer/graphs/M10_Sig_Int.png')
bkg.Draw()
c.SaveAs('/uscms_data/d3/nbower/FSU/TestLowPt/CMSSW_10_6_12/src/myLowPtGsfElectronsAnalyzer/myLowPtGsfElectronsAnalyzer/graphs/M10_Bkg.png')



