{
  TFile* f = TFile::Open("/global/projecta/projectdirs/cuore/scratch/vsharma/MC/Invisible_tri_nucleon_decay/output_plots/127In_coincidence_output_wide.root");
  TCanvas *c1  = new TCanvas("c1","c1",0,0,1600,800);
  Int_t palette[6];
  palette[0] = 38;
  palette[1] = 4;
  palette[2] = 7;
  palette[3] = 8;
  palette[4] = 3;
  palette[5] = 2;
  gStyle->SetPalette(6,palette);
  coincidence_plot->Draw("colz");
}
