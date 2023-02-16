all: PlotE1vsE2 Get_M2_bkg_broad Get_M2_bkg_narrow PlotCorrelatedBkgs
CC=g++
CFLAGS= -g -Wall `root-config --cflags`
LDFLAGS= `root-config --libs`

PlotE1vsE2: PlotE1vsE2.C
	$(CC) PlotE1vsE2.C $(CFLAGS) $(LDFLAGS) -o PlotE1vsE2

Get_M2_bkg_broad: Get_M2_bkg_broad.C
	$(CC)  Get_M2_bkg_broad.C $(CFLAGS) $(LDFLAGS) -o  Get_M2_bkg_broad

Get_M2_bkg_narrow:  Get_M2_bkg_narrow.C
	$(CC)  Get_M2_bkg_narrow.C $(CFLAGS) $(LDFLAGS) -o  Get_M2_bkg_narrow

PlotCorrelatedBkgs: PlotCorrelatedBkgs.cpp
	$(CC) PlotCorrelatedBkgs.cpp $(CFLAGS) $(LDFLAGS) -o PlotCorrelatedBkgs

clean:
	rm ./PlotE1vsE2 ./Get_M2_bkg_narrow  ./Get_M2_bkg_broad ./PlotCorrelatedBkgs
