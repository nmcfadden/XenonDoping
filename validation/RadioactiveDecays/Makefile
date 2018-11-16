ROOTLIBS=`root-config --libs`
ROOTINCS=`root-config --cflags`

LIBS=$(ROOTLIBS)
INCS=$(ROOTINCS)

all: DecayAnalysis

DecayAnalysis: DecayAnalysis.o RadioactiveDecay.o BetaSpectrum.o SingleDecay.o RComp.o
	g++ DecayAnalysis.o BetaSpectrum.o RadioactiveDecay.o $(LIBS) SingleDecay.o RComp.o -o DecayAnalysis

DecayAnalysis.o: DecayAnalysis.cxx 
	g++ -c -g DecayAnalysis.cxx $(INCS)
	
RadioactiveDecay.o: RadioactiveDecay.cxx
	g++ -c -g RadioactiveDecay.cxx $(INCS)
	
BetaSpectrum.o: BetaSpectrum.cxx
	g++ -c -g BetaSpectrum.cxx $(INCS)
	
SingleDecay.o: SingleDecay.cxx
	g++ -c -g SingleDecay.cxx $(INCS)
	
RComp.o: ../VaLi/RComp.cxx
	g++ -c -g ../VaLi/RComp.cxx $(INCS)
	
clean:
	rm DecayAnalysis
	rm -f *.o
