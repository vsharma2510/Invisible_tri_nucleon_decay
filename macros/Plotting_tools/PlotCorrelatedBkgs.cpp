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
    vector<pair<double, double> > *energyVec;
    inputFile->GetObject("energyVec", energyVec);
    inputFile->GetObject("timeVec", timeVec);

    // 2D histogram of broad cut coincidence tags (Summed M2 energy vs Time difference)
    TH2D* energyVsTime = new TH2D("energyVsTime", "Summed M2 energy vs Time difference", 100, )
}