all: GetCoincidenceWide GetCoincidenceNarrow GetCoincidenceWideAcc
CC=g++
CFLAGS= -g -Wall `root-config --cflags`
LDFLAGS= `root-config --libs`

GetCoincidenceWide: GetCoincidencesSimWideCut.C
	$(CC) GetCoincidencesSimWideCut.C $(CFLAGS) $(LDFLAGS) -o GetCoincidenceWide

GetCoincidenceNarrow: GetCoincidencesSimNarrowCut.C
	$(CC) GetCoincidencesSimNarrowCut.C $(CFLAGS) $(LDFLAGS) -o GetCoincidenceNarrow

GetCoincidenceWideAcc: GetCoincidencesSimWideCutAccidental.C
	$(CC) GetCoincidencesSimWideCutAccidental.C $(CFLAGS) $(LDFLAGS) -o GetCoincidenceWideAcc

clean:
	rm ./GetCoincidenceWide ./GetCoincidenceNarrow ./GetCoincidenceWideAcc
