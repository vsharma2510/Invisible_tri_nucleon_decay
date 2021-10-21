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

int main()
{
  //Opening ares simulation file
  TFile* sim_file = TFile::Open("../simulation_output/127In_decay_chain_ares_1000000.root");
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

  bool prompt_event_found=false; //Variable to keep flag prompt event
  bool delayed_event_found=false; //Variable to keep flag delayed event
  
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

  //Looping over tree entries
  for(int e=0;e<numEntries;e++)
    {
      sim_tree->GetEntry(e);
      //cout<<"Multiplicity is "<<multiplicity<<endl;
      //Checking if the decay chain has changed
      if(chain_num==temp_chain_num)
	{
	  //Checking if event is M2
	  if(multiplicity==2)
	    {
	      //In case prompt event has already been found, look for delayed event
	      if(prompt_event_found)
		{
		  if(multiplet_energy[0]>delayed_low && multiplet_energy[0]<delayed_high && time-prompt_time<720) 
		    {
		      delayed_event_found=1;
		      energy=multiplet_energy[0];
		    }
		  if(multiplet_energy[1]>delayed_low && multiplet_energy[1]<delayed_high && time-prompt_time<720)
		    {
		      delayed_event_found=1;
		      energy=multiplet_energy[1];
		    }
		  if(delayed_event_found=1)
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
		  if(prompt_event_found=1)
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
  return 0;
}





      
    
	      
  
