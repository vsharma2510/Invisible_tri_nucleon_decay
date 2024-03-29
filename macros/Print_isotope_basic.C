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
#include <time.h>
#include <boost/filesystem.hpp>

using namespace std;

int main()
{
  double DEnergy,InputEnergy;
  ULong64_t ChainNum;
  string *PName, *DName;
  TFile* f = TFile::Open("/global/projecta/projectdirs/cuore/scratch/vsharma/MC/Invisible_tri_nucleon_decay/test_output/127_In_decay_chain_full_test.root","READ");
  TTree* t = (TTree*)f->Get("qtree");
  t->Print();
  t->Show(0);
  //cout<<"Test text"<<endl;

  t->SetBranchStatus("*",0);
  t->SetBranchStatus("DepositedEnergy",1);
  t->SetBranchStatus("ChainNumber",1);
  t->SetBranchStatus("ParticleName",1);
  t->SetBranchStatus("DaughterName",1);
  t->SetBranchStatus("ParticleInputEnergy",1);

  t->SetBranchAddress("DepositedEnergy",&DEnergy);
  t->SetBranchAddress("ChainNumber",&ChainNum);
  t->SetBranchAddress("ParticleName",&PName);
  t->SetBranchAddress("DaughterName",&DName);
  t->SetBranchAddress("ParticleInputEnergy",&InputEnergy);
  
  int numEntries = t->GetEntries();
  cout<<"---- Number of entries is "<<numEntries<<" ----"<<endl;
  ofstream isotope_info;
  isotope_info.open("Isotope_info.txt");

  for(int e=0;e<numEntries;e++){
    t->GetEntry(e);
    if((DEnergy*1000>488 && DEnergy*1000<494) || (DEnergy*1000>1594 && DEnergy*1000<1600))
      {
	isotope_info<<Form("----- Chain number %lu ----- \n",ChainNum);
	isotope_info<<Form("Energy: %f \n",DEnergy*1000);
	isotope_info<<Form("Particle Name: %s \n",PName->data());
	isotope_info<<Form("Daughter Name: %s \n",DName->data());
	isotope_info<<Form("Particle Input Energy: %f \n",InputEnergy*1000);
	isotope_info<<Form("\n");
      }
  }
    isotope_info.close();
    return 0;
}
