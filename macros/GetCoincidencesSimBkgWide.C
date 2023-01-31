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

void PlotCoincidence(vector<pair<double,double>> pair_vector,double prompt_low,double prompt_high,double delayed_low,double delayed_high)
{
  //Define 2D histogram for plotting delayed energy vs prompt energy of coincident events
  TH2D* coincidence_plot = new TH2D("h_coinc","Delayed energy vs Prompt energy",prompt_high-prompt_low,prompt_low,prompt_high,delayed_high-delayed_low,delayed_low,delayed_high);
  for(auto i=0;i<pair_vector.size();++i)
    {
      //Fill plot with values in pair_vector containing coincident event energy
      coincidence_plot->Fill(pair_vector.at(i).first,pair_vector.at(i).second);
    }
  coincidence_plot->GetXaxis()->SetTitle("Prompt Energy [keV]");
  coincidence_plot->GetYaxis()->SetTitle("Delayed Energy [keV]");
  coincidence_plot->Draw();
  TFile* output_file = TFile::Open("../output_plots/127In_coincidence_output_wide.root","RECREATE");
  output_file->cd();
  coincidence_plot->Write("coincidence_plot");
  output_file->Close();
}

void PlotCoincidenceTime(vector<pair<double,double>> pair_vector,double range)
{
  //Define 2D histogram for plotting time difference between delayed events and prompt events
  TH1D* coincidence_plot = new TH1D("h_coinc","Delayed Time - Prompt Time",range,0,range);
  for(auto i=0;i<pair_vector.size();++i)
    {
      //Fill plot with values in pair_vector containing time differences between coincident events
      coincidence_plot->Fill(pair_vector.at(i).second-pair_vector.at(i).first);
    }
  coincidence_plot->GetXaxis()->SetTitle("Delayed Time - Prompt Time [s]");
  coincidence_plot->GetYaxis()->SetTitle("Counts");
  
  TF1* exp_fit = new TF1("exp_fit","expo(0)",0,range);
  exp_fit->SetParameters(0,-0.003);
  coincidence_plot->Fit(exp_fit,"R");

  coincidence_plot->Draw();
  TFile* output_file = TFile::Open("../output_plots/127In_coincidence_output_wide_timing.root","RECREATE");
  output_file->cd();
  coincidence_plot->Write("coincidence_plot_timing");
  output_file->Close();
}


int main()
{
  //Opening ares simulation file
  TFile* sim_file = TFile::Open("");
  
