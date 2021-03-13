#!/usr/bin/env python

from __future__ import print_function

from ROOT import TCanvas, TGraph, TGraphErrors
from ROOT import gROOT
from ROOT import TLegend
from ROOT import TFile
from ROOT import TGaxis
from ROOT import gDirectory
from ROOT import gStyle
from ROOT import gPad
from ROOT import TH1F
from ROOT import TF1
from ROOT import TLatex
from ROOT import TPad


from math  import sin
from array import array
#-----------------------------------------------------------------


#-----------------------------------------------------------------
def getResolution ( fileName ):
    myfile = TFile( fileName )
    mychain = gDirectory.Get( 'testTree' )
    mychain.Draw('mainClustersEnergy>>tmpHist', '', 'goff')
    hist = gDirectory.Get('tmpHist')
    
    #print ( fileName )
    #print ('nevent: %i ' % (entries))
  
    return energyFit( hist )



#-----------------------------------------------------------------
def energyFit( hist ):
    mean = hist.GetMean()
    rms  = hist.GetRMS()
    #print (' ***************** 1. mean = %f, RMS = %f \n' % (mean, rms))

    func = TF1('myfun', '[0]*exp(-0.5*((x-[1])/[2])**2)')
    func.SetParameter(0, 100)
    func.SetParameter(1, mean)
    func.SetParameter(2, rms)

    hist.Fit('myfun', 'q', '', mean - 1.5 * rms, mean + 1.5 * rms)

    mean = func.GetParameter('p1')
    rms = func.GetParameter('p2')

    #print(func.GetParError(0), func.GetParError(1), func.GetParError(1))
    meanerr = func.GetParError(1)
    rmserr  = func.GetParError(2)

    return (mean, rms, meanerr, rmserr)



#-----------------------------------------------------------------
gStyle.SetPadTickX(1)
gStyle.SetPadTickY(1)
gROOT.SetBatch(True)

c1 = TCanvas( 'c1', 'linearity', 300, 400 )
c1.SetGrid()

n = 8
energyVec = array( 'f' )
energyErrorVec = array( 'f' )

recoEnergyVec = array( 'f' )
recoEnergyErrorVec = array( 'f' )

deltaE_f = array( 'f' ) 
relEnergyErrorVec = array( 'f' ) 
   
relEnergyErrorErrorVec = array( 'f' )

for i in range( n ):
   energyNominal = (i + 1 ) * 10

   fileName = 'reco_gamma_' + str(energyNominal) + 'GeV.root'

   # mean, rms, meanerr, rmserr
   reso = getResolution( fileName )

   energyVec.append( energyNominal )
   energyErrorVec.append( 0. )

   # mean
   recoEnergyVec.append( reso[0] )

   # meanerr
   recoEnergyErrorVec.append( reso[2] )
   #print(reso[2])

   # 
   deltaE_f.append( (reso[0] - energyNominal)/energyNominal )

   # resolution (relative error)
   relEnergyErrorVec.append( reso[1]/reso[0] )

   #
   relEnergyErrorErrorVec.append( reso[3]/reso[0] )

small=1.e-5
c1.Divide(1,2,small,small)

c1.cd(1)
gPad.SetPad(small, 0.3, 1.-small, 1.-small)
gPad.SetBottomMargin(small)

labelSize=10
makerSize = 0.3

gStyle.SetEndErrorSize(0)

gr = TGraphErrors( n, energyVec, recoEnergyVec, energyErrorVec, recoEnergyErrorVec )
gr.SetName('gr')
gr.SetLineColor( 1 )
gr.SetLineWidth( 1 )
gr.SetLineStyle( 2 )
gr.SetMarkerColor( 2 )
gr.SetMarkerStyle( 20 )
gr.SetMarkerSize( makerSize )
textsize = labelSize/(gPad.GetWh()*gPad.GetAbsHNDC())
gr.SetTitle( '' )
gr.GetXaxis().SetTitle( 'E_{particle} (GeV)' )
gr.GetYaxis().SetTitle( 'E_{reco} (GeV)' )
gr.GetYaxis().SetTitleSize(textsize)
gr.GetYaxis().SetLabelSize(textsize)
gr.GetYaxis().SetTitleOffset(1.4)
gr.GetYaxis().SetRangeUser(2, 100)
gPad.SetLeftMargin(0.15)
gr.Draw( 'AP' )

func1 = TF1('fun1', 'x', 0., 100.)
func1.SetLineColor(4)
func1.SetLineStyle(2)
func1.Draw('same')
####


c1.cd(2)
gPad.SetPad(small, small, 1.-small, 0.3-small)
gPad.SetTopMargin(small)
gPad.SetLeftMargin(0.15)
gPad.SetBottomMargin(0.3)
gPad.SetTickx()

gr1 = TGraph( n, energyVec, deltaE_f )
gr1.SetLineColor( 1 )
gr1.SetLineWidth( 1 )
gr1.SetMarkerStyle( 20 )
gr1.SetMarkerSize( makerSize )
gr1.SetMarkerColor( 2 )
gr1.SetTitle( '' )
textsize = labelSize/(gPad.GetWh()*gPad.GetAbsHNDC())
gr1.GetXaxis().SetTitle( 'E_{particle} (GeV)' )
gr1.GetYaxis().SetTitle( '#DeltaE/E_{particle}' )
gr1.GetXaxis().SetTitleOffset(1.2)
gr1.GetYaxis().SetTitleOffset(0.6)
gr1.GetYaxis().SetTitleSize(textsize)
gr1.GetXaxis().SetLabelSize(textsize)
gr1.GetXaxis().SetTitleSize(textsize)
gr1.GetYaxis().SetLabelSize(textsize)
gr1.GetYaxis().SetRangeUser(-0.15, 0.15)
gr1.GetYaxis().SetNdivisions(505)
gr1.Draw( 'AP' )

func2 = TF1('fun2','0', 0., 100.)
func2.SetLineColor(4)
func2.SetLineStyle(2)
func2.Draw('same')

# TCanvas.Update() draws the frame, after which one can change it
c1.Update()
c1.GetFrame().SetBorderSize( 12 )
c1.Modified()
c1.Update()
c1.Print('linearity.eps')



#-------------------------------------------------
c2 = TCanvas( 'c2', 'resolution', 300, 400 )

gr2 = TGraphErrors( n, energyVec, relEnergyErrorVec , energyErrorVec, relEnergyErrorErrorVec)
gr2.SetName('gr2')
gr2.SetLineColor( 1 )
gr2.SetLineWidth( 1 )
gr2.SetLineStyle( 2 )
gr2.SetMarkerColor( 2 )
gr2.SetMarkerStyle( 20 )
gr2.SetMarkerSize( makerSize )
gr2.SetTitle( '' )
gr2.GetXaxis().SetTitle( 'E_{particle} (GeV)' )
gr2.GetYaxis().SetTitle( '#sigma_{reco}/E_{reco}' )
gr2.GetXaxis().SetTitleOffset(1.2)
gr2.GetYaxis().SetTitleOffset(1.9)
gr2.GetYaxis().SetRangeUser(0, 0.1)
gPad.SetLeftMargin(0.15)
gr2.Draw( 'AP' )

funReso = TF1('detReso', 'sqrt([0]*[0]/x+[1]*[1])', 0., 100.)
funReso.SetLineStyle(9)
funReso.SetLineColor(4)
gr2.Fit('detReso', 'q')

a0 = funReso.GetParameter('p0') * 100.
a1 = funReso.GetParameter('p1') * 100.

resoFormula = '#frac{#sigma}{E} = #frac{' + str( round(a0, 1) ) + '%}{#sqrt{E}} #oplus ' + str( round(a1, 1) ) + '%'

txt = TLatex()
txt.SetTextSize(0.035)
txt.DrawLatex(35., 0.08, resoFormula)


c2.Print('resolution.eps')

print('Fitting results:', a0, a1)
