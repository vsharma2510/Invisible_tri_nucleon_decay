{
  TFile* f = TFile::Open("/global/projecta/projectdirs/cuore/scratch/vsharma/MC/Invisible_tri_nucleon_decay/output_plots/127In_coincidence_output_wide.root");
  TCanvas *c1  = new TCanvas("c1","c1",0,0,1600,800);
  Int_t palette[6];
  //Int_t palette[8];
  
  palette[0] = 38;
  palette[1] = 4;
  palette[2] = 7;
  palette[3] = 8;
  palette[4] = 3;
  palette[5] = 2;
  
  /*
  palette[0] = 433;
  palette[1] = 434;
  palette[2] = 435;
  palette[3] = 436;
  palette[4] = 602;
  palette[5] = 1;
  //palette[6] = 429;
  //palette[7] = 428;
  */
  /*
  palette[0] = 417;
  palette[1] = 418;
  palette[2] = 419;
  palette[3] = 420;
  palette[4] = 421;
  palette[5] = 415;
  palette[6] = 414;
  palette[7] = 413;
  */
  gStyle->SetPalette(6, palette);
  //gStyle->SetPalette(54);
  //gStyle->SetNumberContours(99);
  coincidence_plot->Draw("colz");
}
