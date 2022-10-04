#include <iostream>
#include <fstream>
#include <getopt.h>
#include <vector>
#include <string>
#include <sstream>
#include <ostream>
#include <iomanip>
#include <cstdlib>
#include <math.h>

#include <TH2.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TString.h>
#include <TFile.h>
#include <TLegend.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TPad.h>

using namespace std;

int main(int argc, char* argv[])
{

  //Prompt and delayed energy ranges for cuts on M2 spectrum
  double prompt_energy_high=6500, prompt_energy_low=1500;
  double delayed_energy_high=3500, delayed_energy_low=400;

  //Time window used for coincidence tagging in MC and real data
  double time_window = 7*(4.13/525600); //7 times the half-life of 127Sn metastable state

  //Exposure of real data to be used
  double exposure = 1038.4/369.9 ; //TODO: get value, for now use 1038.4 kg yr

  //Opening bkg model simulation output
  TFile* f = TFile::Open("/project/projectdirs/cuore/syncData/CUORE/simulation/BkgModel/2019/bestFit/SpectraM2sum.root");
  TDirectoryFile* d = (TDirectoryFile*)f->Get("Original\ Spectra");
  TH1F* M2_histo = (TH1F*)d->Get("Reduced_allDS_aggressive"); //Getting M2 spectra
  double bin_width = M2_histo->GetBinWidth(10);

  //Getting number of prompt events from M2 spectrum 
  double prompt_events = M2_histo->Integral(M2_histo->FindBin(prompt_energy_low), M2_histo->FindBin(prompt_energy_high));
  double delayed_events = M2_histo->Integral(M2_histo->FindBin(delayed_energy_low), M2_histo->FindBin(delayed_energy_high));
  //double delayed_event_rate = delayed_events*bin_width*((exposure/988)*12);
  //double num_coinc = prompt_events*exposure*bin_width*(1-exp((-1)*delayed_event_rate*time_window));
  double delayed_event_rate = delayed_events*((exposure/988)*12);
  double num_coinc = prompt_events*exposure*(1-exp((-1)*delayed_event_rate*time_window));

  double sensitivity = log(2)*((0.18*0.34*6.022*10e23)/159.60)*sqrt(1038.4/(num_coinc*7.8));
  
  cout<<"prompt_events is "<<prompt_events<<endl;
  cout<<"delayed_evenst is " <<delayed_events<<endl;
  cout<<"Number of coincident events are "<<num_coinc<<endl;
  cout<<"Sensitivity value is "<<sensitivity<<endl;
  return 0;

}

  
  
  
  
  
