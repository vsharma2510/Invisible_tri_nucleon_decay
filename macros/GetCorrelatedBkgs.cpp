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

/*
void StoreCoincidences(vector<pair<double,double> > energyVec, vector<pair<double, double> > timeVec, vector<pair<int, int> > channelVec, 
vector<pair<double, double> > summedEvsTime, TString outputFilepath)
{
  TFile* outputFile = TFile::Open(outputFilepath, "RECREATE");
  outputFile->cd();
  outputFile->WriteObject(&energyVec, "energyVec");
  outputFile->WriteObject(&timeVec, "energyTime");
  outputFile->WriteObject(&channelVec, "channelVec");
  outputFile->WriteObject(&summedEvsTime, "summedEvsTime");
  outputFile->Close();
}
*/

int main(int argc, char **argv)
{

  if(argc<4)
    {
      cout<<"Input 'b' for broad cut tagging"<<endl;
      cout<<"Input 'n' for narrow cut tagging"<<endl;
      cout<<"Followed by path of input file (Ares) and output file (stores coincidences in TTrees)"<<endl;
      return 0;
    }

  //Opening ares simulation file
  TString inputFilepath = argv[2];
  TString outputFilepath = argv[3];
  TFile* sim_file = TFile::Open(inputFilepath);
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
  double multiplet_energy [988];

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
  double summedEnergy, timeDiff;
  //vector<pair<double,double> > coincidence_energy;
  //vector<pair<double,double> > coincidence_time;
  //vector<pair<double, double> > summed_energy_time; // Stores summed energy and time difference betweeen prompt and delayed events 
  //vector<pair<int,int> > coincidence_channel;

  // Creating output file
  TFile* outputFile = TFile::Open(outputFilepath, "RECREATE");

  // Creating output TTree to store coincidence data
  TTree* outTree = new TTree("outTree", "outTree");
  outTree->Branch("promptChannel", &prompt_channel);
  outTree->Branch("delayedChannel", &delayed_channel);
  outTree->Branch("promptEnergy", &prompt_energy);
  outTree->Branch("delayedEnergy", &delayed_energy);
  outTree->Branch("promptTime", &prompt_time);
  outTree->Branch("delayedTime", &delayed_time);
  outTree->Branch("summedEnergy", &summedEnergy);
  outTree->Branch("timeDifference", &timeDiff);
  outTree->Branch("Multiplicity", &multiplicity);
  //Tagging procedure if narrow cut is selected
  if(!strcmp(argv[1], "n"))
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
                          summedEnergy = delayed_energy + prompt_energy;
                          timeDiff = delayed_time - prompt_time;
                          //coincidence_energy.push_back(make_pair(prompt_energy,delayed_energy));
                          //coincidence_time.push_back(make_pair(prompt_time,delayed_time));
                          //coincidence_channel.push_back(make_pair(prompt_channel,delayed_channel));
                          //summed_energy_time.push_back(make_pair(prompt_energy+delayed_energy, delayed_time-prompt_time));
                          //delayed_event_found=0;
                          outTree->Fill();
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
      cout<<"Number of bkg tags for narrow cut is: "<<outTree->GetEntries()<<endl;
    }

  //Tagging procedure if broad cut is selected
  if(!strcmp(argv[1], "b"))
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
                          summedEnergy = delayed_energy + prompt_energy;
                          timeDiff = delayed_time - prompt_time;
                          //coincidence_energy.push_back(make_pair(prompt_energy,delayed_energy));
                          //coincidence_time.push_back(make_pair(prompt_time,delayed_time));
                          //coincidence_channel.push_back(make_pair(prompt_channel,delayed_channel));
                          //prompt_event_found=0;
                          outTree->Fill();
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
      cout<<"Number of bkg tags for broad cut is: "<<outTree->GetEntries()<<endl;
    }

  cout<<"Storing coincidences in output root file"<<endl;
  outputFile->cd();
  outputFile->Write("outTree");
  outputFile->Close();
  //StoreCoincidences(coincidence_energy, coincidence_time, coincidence_channel, summed_energy_time, outputFilepath);

  return 0;
}
