ROOTLIBS=`root-config --libs`
ROOTINCS=`root-config --cflags`

LIBS=$(ROOTLIBS)
INCS=$(ROOTINCS)

all:
	g++ DemoAnalysis.cxx -o DemoAnalysis $(LIBS) $(INCS)

clean:
	rm DemoAnalysis
