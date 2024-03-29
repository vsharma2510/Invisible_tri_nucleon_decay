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

  //Opening file containing simulation output
  TFile* sim_file = TFile::Open("../simulation_output/127In_decay_chain_ares_1000000.root");
  //TFile* sim_file = TFile::Open("../test_output/10mK-k40_savio_1_ares_official_config.root");
  //TFile* sim_file = TFile::Open("../test_output/10mK-k40_ares_official.root");
  if(!sim_file){cout<<"Error opening sim root file!"<<endl;}

  //Accessing event tree
  TTree* sim_tree = (TTree*)sim_file->Get("outTree");
  if(!sim_tree){cout<<"Error opening sim event tree!"<<endl;}

  //Variables to be used for tree access
  double energy,time; //Energy deposited, timestamp of event(local to the chain)
  int channel,multiplicity,multi_index,multiplicity_dist; //Channel number, Multiplet Index
  Long64_t multi_entry; //Multiplet Entry
  ULong64_t chain_num; //Sim chain number
  
  //Accessing branches
  sim_tree->SetBranchStatus("*",1);

  sim_tree->SetBranchAddress("Multiplicity",&multiplicity);
  //sim_tree->SetBranchAddress("Coincidence@Multiplicity_DistCut",&multiplicity_dist);
  //sim_tree->SetBranchAddress("Coincidence@MultipletIndex",&multi_index);
  //sim_tree->SetBranchAddress("Coincidence@MultipletEntry",&multi_entry);
  sim_tree->SetBranchAddress("Energy",&energy);
  sim_tree->SetBranchAddress("Channel_MC",&channel);
  sim_tree->SetBranchAddress("ChainNumber",&chain_num);
  sim_tree->SetBranchAddress("Time",&time);

  //Get number of entries
  int numEntries = sim_tree->GetEntries();
  cout<<"Number of entries is "<< numEntries<<endl;

  int temp_chain_num=0; //Variable to keep track of chain number in loop
  bool prompt_event_found=false; //Variable to keep track of prompt event

  //Variables to record coincidence information
  double prompt_energy,prompt_time;
  int prompt_channel;
  double delayed_energy,delayed_time;
  int delayed_channel;
  bool sig=0;
  
  //Looping over tree entries
  for(int e=0;e<numEntries;e++)
    {
      sim_tree->GetEntry(e);
      if(multiplicity>1){
	sig=1;
      }
      cout<<"Multiplicity is "<<multiplicity<<endl;
      //cout<<"Multiplicity_dist is "<<multiplicity_dist<<endl;
      /*if(!(e%1000000)){
      cout<<"Entry number is "<<endl;
      }*/
      //Checking if event is M2
      //if(multiplicity==2){sim_tree->Show(e);cout<<"Multiplilcity is "<<multi_entry<<endl;}
	//Checking if loop is in the same decay chain AND if prompt event has been recorded
	/*if(temp_chain_num==chain_num && !prompt_event_found) 
	  {
	    //Prompt energy cut
	    if(energy>> && energy<<)
	      {
		prompt_event_found==true;
		prompt_energy=energy;
		prompt_time=time;
		prompt_channel=channel;
	      }
	  }
	//Checking if loop is in the same decay chain and looking for delayed event in case prompt events has been found
	if(temp_chain_num==chain_num && prompt_event_found)
	  {
	    //Delayed energy cut
	    if(energy<<600 && energy>>400)
	      
	      }
	      }*/
    }
  if(sig==1){
  cout<<"Multiplicity greater than 1 found"<<endl;
  }
  cout<<"Number of entries is "<< numEntries<<endl;
  return 0;
}
