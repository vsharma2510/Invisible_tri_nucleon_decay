all: GetCoincidenceWide GetCoincidenceNarrow GetCoincidenceWideAcc PlotE1vsE2 Get_M2_bkg_broad Get_M2_bkg_narrow
CC=g++
CFLAGS= -g -Wall `root-config --cflags`
LDFLAGS= `root-config --libs`

GetCoincidenceWide: GetCoincidencesSimWideCut.C
	$(CC) GetCoincidencesSimWideCut.C $(CFLAGS) $(LDFLAGS) -o GetCoincidenceWide

GetCoincidenceNarrow: GetCoincidencesSimNarrowCut.C
	$(CC) GetCoincidencesSimNarrowCut.C $(CFLAGS) $(LDFLAGS) -o GetCoincidenceNarrow

GetCoincidenceWideAcc: GetCoincidencesSimWideCutAccidental.C
	$(CC) GetCoincidencesSimWideCutAccidental.C $(CFLAGS) $(LDFLAGS) -o GetCoincidenceWideAcc

PlotE1vsE2: PlotE1vsE2.C
	$(CC) PlotE1vsE2.C $(CFLAGS) $(LDFLAGS) -o PlotE1vsE2

Get_M2_bkg_broad: Get_M2_bkg_broad.C
	$(CC)  Get_M2_bkg_broad.C $(CFLAGS) $(LDFLAGS) -o  Get_M2_bkg_broad

Get_M2_bkg_narrow:  Get_M2_bkg_narrow.C
	$(CC)  Get_M2_bkg_narrow.C $(CFLAGS) $(LDFLAGS) -o  Get_M2_bkg_narrow

clean:
	rm ./GetCoincidenceWide ./GetCoincidenceNarrow ./GetCoincidenceWideAcc ./PlotE1vsE2 ./Get_M2_bkg_narrow  ./Get_M2_bkg_broad
