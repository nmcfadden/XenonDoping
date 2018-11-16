ROOTLIBS=$(shell root-config --libs)
ROOTINC=-I$(shell root-config --incdir)

all: CERNanalysis

CERNanalysis: CERNanalysis.cxx
	g++ -std=c++11 -g CERNanalysis.cxx $(ROOTLIBS) $(ROOTINC) -o CERNanalysis

clean:
	rm CERNanalysis
