all: GetCoincidenceWide GetCoincidenceNarrow Get_M2_bkg_broad Get_M2_bkg_narrow GetCorrelatedBkgs
CC=g++
CFLAGS= -g -Wall `root-config --cflags`
LDFLAGS= `root-config --libs`

GetCoincidenceWide: GetCoincidencesSimWideCut.C
	$(CC) GetCoincidencesSimWideCut.C $(CFLAGS) $(LDFLAGS) -o GetCoincidenceWide

GetCoincidenceNarrow: GetCoincidencesSimNarrowCut.C
	$(CC) GetCoincidencesSimNarrowCut.C $(CFLAGS) $(LDFLAGS) -o GetCoincidenceNarrow

Get_M2_bkg_broad: Get_M2_bkg_broad.C
	$(CC)  Get_M2_bkg_broad.C $(CFLAGS) $(LDFLAGS) -o  Get_M2_bkg_broad

Get_M2_bkg_narrow:  Get_M2_bkg_narrow.C
	$(CC)  Get_M2_bkg_narrow.C $(CFLAGS) $(LDFLAGS) -o  Get_M2_bkg_narrow

GetCorrelatedBkgs: GetCorrelatedBkgs.cpp
	$(CC) GetCorrelatedBkgs.cpp $(CFLAGS) $(LDFLAGS) -o GetCorrelatedBkgs

clean:
	rm ./GetCoincidenceWide ./GetCoincidenceNarrow ./Get_M2_bkg_narrow ./Get_M2_bkg_broad ./GetCorrelatedBkgs
