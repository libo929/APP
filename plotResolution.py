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
from ROOT import TText

from ROOT import RooFit
from ROOT import RooRealVar
from ROOT import RooConstVar
from ROOT import RooDataSet
from ROOT import RooDataHist
from ROOT import RooGaussian
from ROOT import RooCBShape
from ROOT import RooPlot
from ROOT import RooArgList


from math  import sin
from array import array
#-----------------------------------------------------------------

fitIndex=0


#-----------------------------------------------------------------
def getResolution ( fileName ):
    myfile = TFile( fileName )
    mychain = gDirectory.Get( 'testTree' )
    mychain.Draw("mainClustersEnergy>>tmpHist", "", "goff")
    hist = gDirectory.Get("tmpHist");
    
    print ( fileName )
    #print ('nevent: %i ' % (entries))

    meanrms = energyFit( hist )
  
    return meanrms



#-----------------------------------------------------------------
def energyFit( hist ):
    mean = hist.GetMean();
    rms  = hist.GetRMS();
    #print (' ***************** 1. mean = %f, RMS = %f \n' % (mean, rms))

    global fitIndex
    pdfName = "fit" + str(fitIndex) + ".pdf"
    fitIndex = fitIndex + 1
    #print ('mean = %f, RMS = %f \n' % (mean, rms))

    # try CB
    #x_cb = RooRealVar("Energy" , "Energy (GeV)" , 0 , mean*2)
    x_cb = RooRealVar("Energy" , "Energy (GeV)" , mean - 5 * rms , mean + 5 * rms)
    mean_cb = RooRealVar("mean","mean", mean, 0, 2*mean) ;
    sigma_cb = RooRealVar("sigma" , "sigma" , rms , 0 , 5*rms) ;
    alpha_cb = RooRealVar("alpha" , "alpha" , 10 , 0 , 50000) ;
    n_cb = RooRealVar("n" , "n" , 10 , 0 , 10000) ;
    shape_cb = RooCBShape("cb" , "cb" , x_cb , mean_cb , sigma_cb , alpha_cb , n_cb) ;

    dh = RooDataHist("histo" , "GeV" , RooArgList(x_cb) , hist) 
    frame = x_cb.frame( RooFit.Title('a frame') )
    dh.plotOn(frame)

    shape_cb.fitTo(dh)
    shape_cb.plotOn(frame)
    #frame.GetYaxis().SetTitle('')
    frame.GetYaxis().SetTitleOffset(1.5)
    frame.SetTitle('')
    #frame.Print("cbfit.pdf")
    frame.Draw()
    gPad.Print(pdfName);
    mean_cbv = mean_cb.getVal()
    sigma_cbv = sigma_cb.getVal()

    mean = mean_cbv
    rms = sigma_cbv
    err = mean_cb.getError()

    print (" ------->>>> Crystal Ball fit: mean = %f, sigma = %f" % (mean_cbv, sigma_cbv))

    return (mean, rms, err)



#-----------------------------------------------------------------
gStyle.SetPadTickX(1)
gStyle.SetPadTickY(1)
gROOT.SetBatch(True)

c1 = TCanvas( 'c1', 'linearity', 300, 400 )
c1.SetGrid()

n = 8
x_f, y_f, z_f, deltaE_f, y_err_f = array( 'd' ), array( 'd' ), array( 'd' ), array( 'd' ), array( 'd' )
x_err = array( 'd' )

for i in range( n ):
   energyNominal = (i + 1 ) * 10
   x_f.append( energyNominal )

   fileName = 'reco_gamma_' + str(energyNominal) + 'GeV.root'
   reso = getResolution( fileName )
   print ('mean = %f, RMS = %f , error = %f \n' % (reso[0], reso[1], reso[2]))
   y_f.append( reso[0] )
   #z.append( reso[1]/reso[0] )
   z_f.append( reso[1]/energyNominal )
   deltaE_f.append( (reso[0] - energyNominal)/energyNominal )
   y_err_f.append(reso[2])  
   x_err.append( 0. )
   print ('---->>>>>>>>', reso[2])

#c1.Divide(2,1)
small=1.e-5
c1.Divide(1,2,small,small);

labelSize=10

c1.cd(1)
gPad.SetPad(small, 0.3, 1.-small, 1.-small);
gPad.SetBottomMargin(small);

makerSize = 0.6

gr = TGraphErrors( n, x_f, y_f, x_err, y_err_f )
gr.SetName('gr')
gr.SetLineColor( 1 )
gr.SetLineWidth( 1 )
gr.SetLineStyle( 2 )
gr.SetMarkerColor( 4 )
gr.SetMarkerStyle( 20 )
gr.SetMarkerSize( makerSize )
#gr.SetTitle( 'linearity' )
textsize = labelSize/(gPad.GetWh()*gPad.GetAbsHNDC());
gr.SetTitle( '' )
gr.GetXaxis().SetTitle( 'E_{particle} (GeV)' )
gr.GetYaxis().SetTitle( 'E_{rec} (GeV)' )
gr.GetYaxis().SetTitleSize(textsize)
gr.GetYaxis().SetLabelSize(textsize)
gr.GetYaxis().SetTitleOffset(1.4)
gr.GetYaxis().SetRangeUser(2, 100)
gPad.SetLeftMargin(0.15);
gr.Draw( 'AP' )

func1 = TF1("fun1","x", 0., 100.);
func1.SetLineColor(1)
func1.SetLineStyle(2)
func1.Draw("same")
####

c1.cd(2)
gPad.SetPad(small, small, 1.-small, 0.3-small);
gPad.SetTopMargin(small);
#gPad.SetRightMargin(small);
gPad.SetLeftMargin(0.15);
gPad.SetBottomMargin(0.3);
gPad.SetTickx();

gr1 = TGraph( n, x_f, deltaE_f )
gr1.SetLineColor( 1 )
gr1.SetLineWidth( 1 )
gr1.SetMarkerStyle( 20 )
gr1.SetMarkerSize( makerSize )
gr1.SetMarkerColor( 4 )
#gr1.SetTitle( 'energy resolution' )
gr1.SetTitle( '' )
textsize = labelSize/(gPad.GetWh()*gPad.GetAbsHNDC());
gr1.GetXaxis().SetTitle( 'E_{particle} (GeV)' )
#gr1.GetYaxis().SetTitle( '#sigma_{E}/E_{rec}' )
gr1.GetYaxis().SetTitle( '#DeltaE/E_{particle}' )
gr1.GetXaxis().SetTitleOffset(1.2)
gr1.GetYaxis().SetTitleOffset(0.6)
gr1.GetYaxis().SetTitleSize(textsize)
gr1.GetXaxis().SetLabelSize(textsize)
gr1.GetXaxis().SetTitleSize(textsize)
gr1.GetYaxis().SetLabelSize(textsize)
gr1.GetYaxis().SetRangeUser(-0.15, 0.15)
gr1.GetYaxis().SetNdivisions(505)
gr1.Draw( 'AP' );

func2 = TF1("fun2","0", 0., 100.);
func2.SetLineStyle(2)
func2.SetLineColor(1)
func2.Draw("same")

# TCanvas.Update() draws the frame, after which one can change it
c1.Update()
#c1.GetFrame().SetFillColor( 21 )
c1.GetFrame().SetBorderSize( 12 )
c1.Modified()
c1.Update()
c1.Print("linearity.pdf")

#-------------------------------------------------
c2 = TCanvas( 'c2', 'resolution', 300, 300 )

gr2 = TGraph( n, x_f, z_f )
gr2.SetName('gr2')
gr2.SetLineColor( 1 )
gr2.SetLineWidth( 1 )
gr2.SetLineStyle( 2 )
gr2.SetMarkerColor( 4 )
gr2.SetMarkerStyle( 20 )
gr2.SetMarkerSize( makerSize )
#gr2.SetTitle( 'linearity' )
#textsize = labelSize/(gPad.GetWh()*gPad.GetAbsHNDC());
gr2.SetTitle( '' )
gr2.GetXaxis().SetTitle( 'E_{particle} (GeV)' )
gr2.GetYaxis().SetTitle( '#sigma_{E}/E_{particle}' )
gr2.GetXaxis().SetTitleOffset(1.2)
gr2.GetYaxis().SetTitleOffset(1.9)
gr2.GetYaxis().SetRangeUser(0, 0.1)
gPad.SetLeftMargin(0.15);
gr2.Draw( 'AP' )

funReso = TF1('detReso', 'sqrt([0]*[0]/x+[1]*[1])', 0., 100.);
#funReso = TF1('detReso', 'sqrt(0.22*0.22/x+0.00008)', 0., 100.);
funReso.SetLineStyle(9)
funReso.SetLineColor(2)
gr2.Fit('detReso')

#funReso.Draw('same')
a0 = funReso.GetParameter('p0')
a1 = funReso.GetParameter('p1')

print(a0, a1)

c2.Update()
c2.GetFrame().SetBorderSize( 12 )
c2.Modified()
c2.Update()
c2.Print("resolution.pdf")
