import ROOT
import sys
if len(sys.argv) != 3:
   print " USAGE : %s <input file > <output file >"%( sys . argv [0])
inFileName = sys.argv [1]
mass = sys.argv [2]
print " Reading from ", inFileName , "Mass point is ", mass
inFile = ROOT.TFile.Open(inFileName ,"READ") 
histos = {}
lines= {'EB_hOverE_2D' : .236, 'EB_hOverE_2D_pass' : .236, 'EE_hOverE_2D' : .0801, 'EE_hOverE_2D_pass' : .0801}
c = ROOT.TCanvas("c","c",600,600)      
c.SetBorderSize(0)   
c.SetFrameBorderMode(0)
c.SetLogz()
ROOT.gStyle.SetTitleBorderSize(0)
ROOT.gStyle.SetOptStat(0)
#ROOT.TColor.Colorpalettes(195)
#ROOT.gStyle.SetPalette(156)
#inFile.cd('simple')

outfolder = ('/uscms_data/d3/nbower/FSU/TestLowPt/CMSSW_10_6_12/src/myLowPtGsfElectronsAnalyzer/myLowPtGsfElectronsAnalyzer/graphs/'+mass+'/'+mass+'_')

nevent = inFile.Get('simple/nEvent')
nevent.Draw()
c.SaveAs(outfolder+'_NEvent.png')

idHist = inFile.Get('simple/idHist')
idHist.Draw()
c.SaveAs(outfolder+'_ID.png')

nElectron = inFile.Get('simple/nElectron')
nElectron.Draw()
c.SaveAs(outfolder+'_nElectron.png')

#idHoverE= inFile.Get('simple/idHoverE')
#idHoverE.GetXaxis().SetTitle("H/E")
#idHoverE.GetYaxis().SetTitle("Id")
#idHoverE.Draw("COL Z CJUST")
#c.SaveAs(outfolder+'_idHoverE.png')


#idPt = inFile.Get('simple/idPt')
#idPt.GetXaxis().SetTitle("pt")
#idPt.GetYaxis().SetTitle("Id")
#idPt.Draw("COL Z CJUST")
#c.SaveAs(outfolder+'_idPt.png')
N = 1/nElectron.GetBinContent(2)

#EEId = inFile.Get('simple/EEId')
#EEId.Draw()
#c.SaveAs(outfolder+'_EEId.png')

#EEIdPt = inFile.Get('simple/EEIdPt')
#EEIdPt.GetXaxis().SetTitle("pt")
#EEIdPt.GetYaxis().SetTitle("Id")
#EEIdPt.Draw("COL Z CJUST")
#c.SaveAs(outfolder+'_EEIdPt.png')

#EBIdPt = inFile.Get('simple/EBIdPt')
#EBIdPt.GetXaxis().SetTitle("pt")
#EBIdPt.GetYaxis().SetTitle("Id")
#EBIdPt.Draw("COL Z CJUST")
#c.SaveAs(outfolder+'_EBIdPt.png')



#EEIdHoverE = inFile.Get('simple/EEIdHoverE')
#EEIdHoverE.GetXaxis().SetTitle("H/E")
#EEIdHoverE.GetYaxis().SetTitle("Id")
#EEIdHoverE.Draw("COL Z CJUST")
#c.SaveAs(outfolder+'_EEIdHoverE.png')


#EBIdHoverE = inFile.Get('simple/EBIdHoverE')
#EBIdHoverE.GetXaxis().SetTitle("H/E")
#EBIdHoverE.GetYaxis().SetTitle("Id")
#EBIdHoverE.Draw("COL Z CJUST")
#c.SaveAs(outfolder+'_EBIdHoverE.png')

pt = inFile.Get('simple/pt')
pt.Draw()
c.SaveAs(outfolder+'_pt.png')

#idsigmaIetaIeta = inFile.Get('simple/idsigmaIetaIeta')
#idsigmaIetaIeta.GetXaxis().SetTitle("SigIhIh")
#idsigmaIetaIeta.GetYaxis().SetTitle("Id")
#idsigmaIetaIeta.Draw("COL Z CJUST")
#c.SaveAs(outfolder+'_idsigmaIetaIeta.png')

nLowPtEle = inFile.Get('simple/nLowPtEle')
nLowPtEle.Draw()
c.SaveAs(outfolder+'_nLowPtEle.png')

idWorkingPoints = inFile.Get('simple/idWorkingPoints')
idWorkingPoints.Scale(N)
idWorkingPoints.Draw()
c.SaveAs(outfolder+'_ID_WP.png')

c.SetLogy()
pt.SetTitle('Electron Pt (workingPoints')
pt.Draw()
veto_LowPT = inFile.Get('simple/vetoPt')
veto_LowPT.SetLineColor(600)
veto_LowPT.Draw('SAME')
loose_LowPT = inFile.Get('simple/loosePt')
loose_LowPT.SetLineColor(632)
loose_LowPT.Draw('SAME')
medium_LowPT = inFile.Get('simple/mediumPt')
medium_LowPT.SetLineColor(416)
medium_LowPT.Draw('SAME')
tight_LowPT = inFile.Get('simple/tightPt')
tight_LowPT.SetLineColor(800)
tight_LowPT.Draw('SAME')

legend_LowPT = ROOT.TLegend((0.4,0.6,0.89,0.89))
legend_LowPT.SetHeader("Legend", "C")
legend_LowPT.AddEntry(pt, 'No ID', 'l')
legend_LowPT.AddEntry(veto_LowPT, "ID>2", "l")
legend_LowPT.AddEntry(loose_LowPT, "ID>3.5", "l")
legend_LowPT.AddEntry(medium_LowPT, "ID>5", "l")
legend_LowPT.AddEntry(tight_LowPT, "ID>5.5", "l")
legend_LowPT.Draw()
c.SaveAs(outfolder+'_eptID_LowPT.png')

print 'There are ' +str( nevent.GetBinContent(1))+' Events in bin 1\n'
print 'There are ' + str(nevent.GetBinContent(2))+' Events in bin 2\n'
print 'There are ' + str(nevent.GetBinContent(0))+' Events in bin 0\n'
print 'There are ' + str(nLowPtEle.GetBinContent(1)) + ' matched Electrons Total \n'
print 'of those ' + str(nLowPtEle.GetBinContent(2)) + ' electrons have pt less than 7 \n'
