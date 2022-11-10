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
  TH2D* coincidence_plot = new TH2D("h_coinc","Delayed energy vs Prompt energy",prompt_high-prompt_low,prompt_low,prompt_high,delayed_high-delayed_low,delayed_low,delayed_high);
  for(auto i=0;i<pair_vector.size();++i)
    {
      coincidence_plot->Fill(pair_vector.at(i).first,pair_vector.at(i).second);
    }
  coincidence_plot->GetXaxis()->SetTitle("Prompt Energy [keV]");
  coincidence_plot->GetYaxis()->SetTitle("Delayed Energy [keV]");
  coincidence_plot->Draw("colz");
  TFile* output_file = TFile::Open("../output_plots/127In_coincidence_output_narrow.root","RECREATE");
  output_file->cd();
  coincidence_plot->Write("coincidence_plot");
}

void PlotCoincidenceTime(vector<pair<double,double>> pair_vector, double range)
{
  TH1D* coincidence_plot = new TH1D("h_coinc","Delayed Time - Prompt Time",range/15,0,range);
  for(auto i=0;i<pair_vector.size();++i)
    {
      coincidence_plot->Fill(pair_vector.at(i).second-pair_vector.at(i).first);
    }
  coincidence_plot->GetXaxis()->SetTitle("Delayed Time - Prompt Time [s]");
  coincidence_plot->GetYaxis()->SetTitle("Counts");

  //TF1* exp_fit = new TF1("exp_fit","([0]*exp(-(x)/(60*[1]))+[2])+ ([3]*exp(-(x)/(60*[4])))",40,1800);
  TF1* exp_fit = new TF1("exp_fit","([0]*exp(-(x)/(60*[1])))",40,1800);
  //exp_fit->FixParameter(1,5.2);
  exp_fit->SetParameter(0,500);
  exp_fit->SetParameter(1,5.2);
  //exp_fit->SetParameter(2,1);
  exp_fit->SetParName(1,"#tau (min)");
  //exp_fit->SetParName(4,"#tau2 (min)");
  //exp_fit->SetParameter(3,10);
  //exp_fit->SetParameter(4,540);
  //exp_fit->SetParameters(0,-0.002,1);
  exp_fit->SetLineWidth(2);
  //coincidence_plot->Sumw2(1);
  coincidence_plot->Fit(exp_fit,"LL","R");

  cout<<"Fitted half life is "<<(exp_fit->GetParameter(1))*log(2)<<" and error is "<<(exp_fit->GetParError(1))*log(2)<<endl;

  TFile* output_file = TFile::Open("../output_plots/127In_coincidence_output_narrow_timing.root","RECREATE");
  output_file->cd();
  coincidence_plot->Write("coincidence_plot_timing");
}


int main()
{
  //Opening ares simulation file
  TFile* sim_file = TFile::Open("../simulation_output/127In_output/127In_decay_chain_ares_1000000.root");
  if(!sim_file){cout<<"Error opening sim root file!"<<endl;}

  //Accessing event tree
  TTree* sim_tree = (TTree*)sim_file->Get("outTree");
  if(!sim_tree){cout<<"Error opening sim event tree!"<<endl;}

  //Variables to be used for tree access
  double energy,time; //Energy deposited, timestamp of event(local to the chain)
  int channel,multiplicity,multi_index,multiplicity_dist; //Channel number, Multiplet Index
  Long64_t multi_entry; //Multiplet Entry
  ULong64_t chain_num; //Sim chain num
  int chain_change; //Variable to keep track of chain changes
  ULong64_t temp_chain_num=1;
  double multiplet_energy [2];

  //Accessing branches
  sim_tree->SetBranchAddress("Multiplicity",&multiplicity);
  sim_tree->SetBranchAddress("Energy",&energy);
  sim_tree->SetBranchAddress("Channel",&channel);
  sim_tree->SetBranchAddress("ChainNumber",&chain_num);
  sim_tree->SetBranchAddress("Time",&time);
  sim_tree->SetBranchAddress("MultipletEnergy",&multiplet_energy);

  //Get number of entries
  int numEntries = sim_tree->GetEntries();
  cout<<"Number of entries is "<<numEntries<<endl;

  bool prompt_event_found=false; //Variable to flag prompt event
  bool delayed_event_found=false; //Variable to flag delayed event
  
  //Variables to record coincidence information
  double prompt_energy,prompt_time;
  int prompt_channel;
  double delayed_energy,delayed_time;
  int delayed_channel;
  vector<pair<double,double>> coincidence_energy;
  vector<pair<double,double>> coincidence_time;
  vector<pair<int,int>> coincidence_channel;
  
  //Energy cuts
  double prompt_low=1587, prompt_high=1607;
  double delayed_low=480, delayed_high=500;

  //Time cut
  double time_low=0.01, time_high=1800;
  
  //Looping over tree entries
  for(int e=0;e<numEntries;e++)
    {
      sim_tree->GetEntry(e);
      //cout<<"Multiplicity is "<<multiplicity<<endl;
      //Checking if the decay chain has changed
      if(chain_num==temp_chain_num)
	{
	  //Decay chain has not changed
	  //Checking if event is M2
	  if(multiplicity==2)
	    {
	      //In case prompt event has already been found, look for delayed event
	      if(prompt_event_found)
		{
		  //If either multiplet passes delayed energy cut, store multiplet energy
		  if(multiplet_energy[0]>delayed_low && multiplet_energy[0]<delayed_high && time-prompt_time>time_low && time-prompt_time<time_high) 
		    {
		      delayed_event_found=1;
		      energy=multiplet_energy[0];
		    }
		  if(multiplet_energy[1]>delayed_low && multiplet_energy[1]<delayed_high && time-prompt_time >time_low && time-prompt_time<time_high)
		    {
		      delayed_event_found=1;
		      energy=multiplet_energy[1];
		    }
		  if(delayed_event_found)
		    {
		      delayed_energy=energy;
		      delayed_time=time;
		      delayed_channel=channel;
		      coincidence_energy.push_back(make_pair(prompt_energy,delayed_energy));
		      coincidence_time.push_back(make_pair(prompt_time,delayed_time));
		      coincidence_channel.push_back(make_pair(prompt_channel,delayed_channel));
		      delayed_event_found=0;
		    }
		  else
		    {
		      continue;
		    }
		}
	      else
		{
		  //If either multiplet passes prompt energy cut, store multiplet energy
		  if(multiplet_energy[0]>prompt_low && multiplet_energy[0]<prompt_high) 
		    {
		      prompt_event_found=1;
		      energy=multiplet_energy[0];
		    }
		  if(multiplet_energy[1]>prompt_low && multiplet_energy[1]<prompt_high)
		    {
		      prompt_event_found=1;
		      energy=multiplet_energy[1];
		    }
		  if(prompt_event_found)
		    {
		      prompt_energy=energy;
		      prompt_time=time;
		      prompt_channel=channel;
		    }
		  else
		    {
		      continue;
		    }
		}
	    }
	  else
	    {
	      continue;
	    }
	}
      else
	{
	  chain_change++;
	  prompt_event_found=0;
	  temp_chain_num=chain_num;
	}
    }
  
  //Plotting prompt energy and delayed energy in a 2d histogram
  PlotCoincidence(coincidence_energy,prompt_low,prompt_high,delayed_low,delayed_high);
  PlotCoincidenceTime(coincidence_time,time_high);
  
  return 0;
}





      
    
	      
  
