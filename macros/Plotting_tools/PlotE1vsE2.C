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

void PlotMultiplet(vector<pair<double,double>> pair_vector, vector<pair<double,double>> time_pair_vector, double prompt_low,double prompt_high,double delayed_low,double delayed_high)
{
  //Define 2D histogram for plotting delayed energy vs prompt energy of coincident events
  TH2D* coincidence_plot = new TH2D("h_coinc","E1 vs E2 (prompt)",(prompt_high-prompt_low)/25,prompt_low,prompt_high,(delayed_high-delayed_low)/25,delayed_low,delayed_high);
  TH2D* multiplet_time_1 =  new TH2D("h_coinc_2","E1 vs dT (prompt)",1800,0,1800,(prompt_high-prompt_low)/25,prompt_low,prompt_high);
  TH2D* multiplet_time_2 =  new TH2D("h_coinc_3","E2 vs dT (prompt)",1800,0,1800,(prompt_high-prompt_low)/25,prompt_low,prompt_high);
  for(auto i=0;i<pair_vector.size();++i)
    {
      //Fill plot with values in pair_vector containing coincident event energy
      coincidence_plot->Fill(pair_vector.at(i).first,pair_vector.at(i).second);
      multiplet_time_1->Fill((time_pair_vector.at(i).second)-(time_pair_vector.at(i).first), pair_vector.at(i).first);
      multiplet_time_2->Fill((time_pair_vector.at(i).second)-(time_pair_vector.at(i).first), pair_vector.at(i).second);
    }
  coincidence_plot->GetXaxis()->SetTitle("Multiplet 1 [keV]");
  coincidence_plot->GetYaxis()->SetTitle("Multiplet 2 [keV]");
  multiplet_time_1->GetXaxis()->SetTitle("dT [sec]");
  multiplet_time_1->GetYaxis()->SetTitle("Multiplet 1 [keV]");
  multiplet_time_2->GetXaxis()->SetTitle("dT [sec]");
  multiplet_time_2->GetYaxis()->SetTitle("Multiplet 2 [keV]");
  coincidence_plot->Draw();
  multiplet_time_1->Draw();
  multiplet_time_2->Draw();
  TFile* output_file = TFile::Open("../output_plots/127In_coincidence_output_wide_e1e2_prompt.root","RECREATE");
  output_file->cd();
  coincidence_plot->Write("coincidence_plot");
  multiplet_time_1->Write("multiplet_time_1");
  multiplet_time_2->Write("multiplet_time_2");
  output_file->Close();
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
  double total_energy,time; //Energy deposited, timestamp of event(local to the chain)
  int channel,multiplicity,multi_index,multiplicity_dist; //Channel number, Multiplet Index
  Long64_t multi_entry; //Multiplet Entry
  ULong64_t chain_num; //Sim chain number
  ULong64_t temp_chain_num=1;
  double multiplets [2]; 

  //Accessing branches
  //sim_tree->SetBranchStatus("*",1);
  sim_tree->SetBranchAddress("Multiplicity",&multiplicity);
  sim_tree->SetBranchAddress("TotalEnergy",&total_energy); 
  sim_tree->SetBranchAddress("Channel",&channel);
  sim_tree->SetBranchAddress("ChainNumber",&chain_num);
  sim_tree->SetBranchAddress("Time",&time);
  sim_tree->SetBranchAddress("MultipletEnergy", multiplets);
  
  //Get number of entries
  int numEntries = sim_tree->GetEntries();
  cout<<"Number of entries is "<< numEntries<<endl;

  //int temp_chain_num=0; //Variable to keep track of chain number in loop
  bool prompt_event_found=false; //Variable to keep track of prompt event

  //Variables to record coincidence information
  double prompt_energy,prompt_time;
  int prompt_channel;
  double delayed_energy,delayed_time;
  int delayed_channel;
  vector<pair<double,double>> coincidence_energy;
  vector<pair<double,double>> coincidence_time;
  vector<pair<int,int>> coincidence_channel;
  vector<pair<double, double>> multiplet_energy_prompt;
  vector<pair<double, double>> multiplet_energy_delayed;

  //Energy cuts
  double prompt_low=1500, prompt_high=6500;
  double delayed_low=400, delayed_high=3500;

  //Time cuts
  double time_low=0.1, time_high=1800;
  
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
	      //In case prompt event has already been found, look for delayed_event
	      if(prompt_event_found)
		{
		  //Applying energy and time cuts for delayed event
		  if(total_energy>delayed_low && total_energy<delayed_high && time-prompt_time>time_low && time-prompt_time<time_high)
		    {
		      delayed_energy=total_energy;
		      delayed_time=time;
		      delayed_channel=channel;
		      coincidence_energy.push_back(make_pair(prompt_energy,delayed_energy));
		      coincidence_time.push_back(make_pair(prompt_time,delayed_time));
		      coincidence_channel.push_back(make_pair(prompt_channel,delayed_channel));
		      multiplet_energy_delayed.push_back(make_pair(multiplets[0],multiplets[1]));
		      //prompt_event_found=0;
		    }
		  else
		    {
		      continue;
		    }
		}
	      else
		{
		  //Applying energy and time cuts for prompt event
		  if(total_energy>prompt_low && total_energy<prompt_high)
		    {
		      prompt_energy=total_energy;
		      prompt_time=time;
		      prompt_channel=channel;
		      prompt_event_found=1;
		      multiplet_energy_prompt.push_back(make_pair(multiplets[0],multiplets[1]));
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
	  prompt_event_found=0;
	  temp_chain_num=chain_num;
	}
    }
  
  //Plotting prompt energy and delayed energy in a 2d histogram
  PlotMultiplet(multiplet_energy_prompt,coincidence_time,delayed_low,delayed_high,delayed_low,delayed_high); 
    
  return 0;
}


