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
//using namespace Cuore;

int main()
{

  double DEnergy,NTDEnergy;
  TFile* f = TFile::Open("/global/projecta/projectdirs/cuore/scratch/vsharma/MC/Invisible_tri_nucleon_decay/simulation_output/127In_decay_chain_qshields_1000000.root","READ");
  TTree* t = (TTree*)f->Get("qtree");
  t->Print();
  t->Show(0);
  //cout<<"Test text"<<endl;

  t->SetBranchStatus("*",0);
  t->SetBranchStatus("DepositedEnergy",1);
  //t->SetBranchStatus("EnergyDepositedInNTD",1);

  t->SetBranchAddress("DepositedEnergy",&DEnergy);
  //t->SetBranchAddress("EnergyDepositedInNTD",&NTDEnergy);
  
  int numEntries = t->GetEntries();
  cout<<"----- Number of entries is "<<numEntries<<" -----"<<endl;

  TH1D* h_total = new TH1D("Deposited Energy","Deposited Energy",10000,0,5000);
  //TH1D* h_NTD = new TH1D("NTD Energy","NTD Energy",3000,0,3000);
  
  for(int e=0;e<numEntries;e++){
    t->GetEntry(e);
    /* if(e%10==0){
      cout<<"Deposited energy is "<<1000*DEnergy<<endl;
      } */
    h_total->Fill(1000*DEnergy);
    //h_NTD->Fill(1000*NTDEnergy);
  }

  TFile* fout = TFile::Open("/global/projecta/projectdirs/cuore/scratch/vsharma/MC/Invisible_tri_nucleon_decay/test_output/deposited_energy_histogram_1000000.root","RECREATE");
  fout->cd();
  h_total->Write("histogram_total");
  //h_NTD->Write("histogram_NTD");
  
  fout->Close();
  return 0;
}
