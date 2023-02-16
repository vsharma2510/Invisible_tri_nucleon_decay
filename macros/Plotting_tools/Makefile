all: PlotE1vsE2 PlotCorrelatedBkgs
CC=g++
CFLAGS= -g -Wall `root-config --cflags`
LDFLAGS= `root-config --libs`

PlotE1vsE2: PlotE1vsE2.C
	$(CC) PlotE1vsE2.C $(CFLAGS) $(LDFLAGS) -o PlotE1vsE2

PlotCorrelatedBkgs: PlotCorrelatedBkgs.cpp
	$(CC) PlotCorrelatedBkgs.cpp $(CFLAGS) $(LDFLAGS) -o PlotCorrelatedBkgs

clean:
	rm ./PlotE1vsE2 ./PlotCorrelatedBkgs
