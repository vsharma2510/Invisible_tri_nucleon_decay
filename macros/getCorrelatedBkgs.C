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

int main(int argc, char **argv)
{

  if(argc==1)
    {
      cout<<"Input 'w' for wide cut tagging"<<endl;
      cout<<"Input 'n' for narrow cut tagging"<<endl;
      return 0;
    }

  //Opening ares simulation file
  TString filepath = argv[2];
  TFile* sim_file = TFile::Open(filepath); //TODO!! Change the sim_file path
  if(!sim_file){cout<<"Error opening sim root file!"<<endl;}

  //Accessing event tree
  TTree* sim_tree = (TTree*)sim_file->Get("outTree");
  if(!sim_tree){cout<<"Error opening sim event tree!"<<endl;}

  //Variables to be used for tree access
  double energy,time,totalEnergy; //Energy deposited, timestamp of event(local to the chain)
  int channel,multiplicity,multi_index,multiplicity_dist; //Channel number, Multiplet Index
  Long64_t multi_entry; //Multiplet Entry
  ULong64_t chain_num; //Sim chain num
  int chain_change; //Variable to keep track of chain changes
  ULong64_t temp_chain_num=1;
  double multiplet_energy [2];

  //Accessing branches
  sim_tree->SetBranchAddress("Multiplicity",&multiplicity);
  sim_tree->SetBranchAddress("Energy",&energy);
  sim_tree->SetBranchAddress("TotalEnergy",&totalEnergy);
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
  vector<pair<double,double> > coincidence_energy;
  vector<pair<double,double> > coincidence_time;
  vector<pair<int,int> > coincidence_channel;

  //Tagging procedure if narrow cut is selected
  if(strcmp(argv[1], "n"))
    {
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
      cout<<"Number of bkg tags for narrow cut is: "<<coincidence_energy.size()<<endl;
    }

  //Tagging procedure if wide cut is selected
  if(strcmp(argv[1], "w"))
    {
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
                      if(totalEnergy>delayed_low && totalEnergy<delayed_high && time-prompt_time>time_low && time-prompt_time<time_high)
                        {
                          delayed_energy=totalEnergy;
                          delayed_time=time;
                          delayed_channel=channel;
                          coincidence_energy.push_back(make_pair(prompt_energy,delayed_energy));
                          coincidence_time.push_back(make_pair(prompt_time,delayed_time));
                          coincidence_channel.push_back(make_pair(prompt_channel,delayed_channel));
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
                      if(totalEnergy>prompt_low && totalEnergy<prompt_high)
                        {
                          prompt_energy=totalEnergy;
                          prompt_time=time;
                          prompt_channel=channel;
                          prompt_event_found=1;
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
      cout<<"Number of bkg tags for broad cut is: "<<coincidence_energy.size()<<endl;
    }
}
