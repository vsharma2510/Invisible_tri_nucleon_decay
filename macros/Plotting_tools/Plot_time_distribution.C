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

using namespace std;

int main()
{
  double DEnergy,DTime;
  //ULong64_t ChainNum;
  //string *PName, *DName;
  TFile* f = TFile::Open("/global/projecta/projectdirs/cuore/scratch/vsharma/MC/Invisible_tri_nucleon_decay/test_output/127In_decay_chain_full_test.root","READ");
  TTree* t = (TTree*)f->Get("qtree");
  t->Print();
  t->Show(0);
  //cout<<"Test text"<<endl;
  
  t->SetBranchStatus("*",0);
  t->SetBranchStatus("DepositedEnergy",1);
  t->SetBranchStatus("Time",1);

  t->SetBranchAddress("DepositedEnergy",&DEnergy);
  t->SetBranchAddress("Time",&DTime);

  int numEntries = t->GetEntries();
  cout<<"----- Number of entries is "<<numEntries<<" -----"<<endl;

  TH1D* h_time = new TH1D("Time distribution","Time distribution",100,0,2000);

  for(int e=0;e<numEntries;e++){
    t->GetEntry(e);
    if(DEnergy*1000>490 && DEnergy*1000<491)
      {
	h_time->Fill(DTime);
	if(e%1000){cout<<"Energy of "<<DEnergy*1000<<" deposited at time "<<DTime<<endl;}
      } 
  }

  double MaxBinContent = h_time->GetBinContent(1);
  cout<<"MaxBinContent is "<<MaxBinContent<<endl;
  
  TF1* func = new TF1("func","[0]*exp([1]*x)",-10,2000);
  func->SetParameters(100          ,-0.00289);
  h_time->Fit("func");
  
  TFile* fout = TFile::Open("/global/projecta/projectdirs/cuore/scratch/vsharma/MC/Invisible_tri_nucleon_decay/test_output/127In_decay_chain_time_output.root","RECREATE");
  fout->cd();
  h_time->GetXaxis()->SetTitle("Time(s)");
  h_time->GetYaxis()->SetTitle("Counts");
  h_time->Write("time_histogram");
  fout->Close();

  return 0;
}
