/*
Macro to plot 2D histograms of correlated background tags 

Input
*****************************************************************************************************************************
Root files containing coincidence tags stored as vectors that are output of GetCorrelatedBkgs.cpp from the output directory
*****************************************************************************************************************************

Output
****************************************************************************************************************************
Root files containing 2d histograms of plotting summed M2 energy vs time difference between prompt and delayed events. The 
histograms are also stored as images
****************************************************************************************************************************
*/

#include "Riostream.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TStopwatch.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TString.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TError.h"
#include "TObject.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <map>
#include <set>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    TString inputFilename = argv[1];
    TFile* inputFile = TFile::Open(inputFilename, "READ");
    vector<pair<double, double> > *summedEvsTime;
    
    inputFile->GetObject("summedEvsTime", summedEvsTime);

    // 2D histogram of broad cut coincidence tags (Summed M2 energy vs Time difference)
    TH2D* energyVsTime = new TH2D("energyVsTime", "Summed M2 energy vs Time difference", 100, 0, 100, 100, 0, 20000);
    for(std::vector<pair<double, double> >::iterator i = summedEvsTime->begin(); i != summedEvsTime->end(); i++)
    {
      //Fill plot with values in pair_vector containing summed coincident event energy and time
      energyVsTime->Fill((*i).first,(*i).second);
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 900,900);
    gStyle->SetOptStat(0);

    TPad *center_pad = new TPad("center_pad", "center_pad", 0.0, 0.0, 0.6, 0.6);
    center_pad->Draw();
    right_pad = new TPad("right_pad", "right_pad",0.55,0.0,1.0,0.6);
    right_pad->Draw();

    TH1D* projY = energyVsTime->ProjectionY();

    energyVsTime->GetXaxis()->SetTitle("Prompt Energy [keV]");
    energyVsTime->GetYaxis()->SetTitle("Delayed Energy [keV]");
    energyVsTime->Draw();

    string fileNameExtension = ".root"
    size_t pos = str.find(fileNameExtension);
    string outputFilename = inputFilename.replace(pos, fileNameExtension.length(), "_2DH.root");
    TFile* outputFile = TFile::Open(outputFilename,"RECREATE");
    outputFile->cd();
    energyVsTime->Write("energyVsTime");
    outputFile->Close();
}