# Root variables
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs) -lMinuit
ROOTGLIBS    := $(shell root-config --glibs)

# Programs
CXX          = g++
CXXFLAGS     = -g -Wall -fPIC -Wno-deprecated -O2 
LD           = g++
LDFLAGS      = -g -O2 
SOFLAGS      = -shared

RM           = rm -f 
MV           = mv 
ECHO         = echo
CINT         = rootcint

# Assign or Add variables
CXXFLAGS    += $(ROOTCFLAGS) 
CXXFLAGS    += -I./include -I./src -I./
LIBS        += $(ROOTLIBS) -L. 
GLIBS       += $(ROOTGLIBS) -L. 

# CXSRCS      = TPSAnalysis.cxx TPulseshape.cxx 

# CXXSRCS      = $(patsubst %.cxx,src/%.cxx,$(CXSRCS))

# DICTSRCS     = $(patsubst %.cxx,src/%Dict.cxx,$(CXSRCS))

# DICTINCS     = $(patsubst %.cxx,src/%Dict.h,$(CXSRCS))

# CXXOBJS      = $(patsubst %.cxx,obj/%.o,$(CXSRCS))

# DICTOBJS     = $(patsubst %.cxx,obj/%Dict.o,$(CXSRCS))

EXEOBJS       = 

GARBAGE      = reconstruct.o eventdisplay.o 

all : reconstruct  

link.d : $(patsubst %.cxx,include/%.h,$(CXSRCS))
	$(CXX) -MM $(CXXFLAGS) $(CXXSRCS) > link.d; 


include link.d

clean :
	$(RM) $(GARBAGE)

reconstruct : reconstruct.c 
	$(CXX) $(CXXFLAGS) -c $<
	$(CXX) $(LDFLAGS) $(ROOTLIBS) reconstruct.o -o reconstruct.exe 

eventdisplay : eventdisplay.c 
	$(CXX) $(CXXFLAGS) -c $<
	$(CXX) $(LDFLAGS) $(ROOTLIBS) eventdisplay.o -o eventdisplay.exe 

print :
	echo compiler  : $(CXX)
	echo compiler  : $(CXSRCS)
	echo c++ srcs  : $(CXXSRCS)
	echo c++ objs  : $(CXXOBJS)
	echo Dictionary: $(DICTSRCS)
	echo c++ flags : $(CXXFLAGS)
	echo libs      : $(LIBS)
	echo so flags  : $(SOFLAGS)

	echo rootlibs  : $(ROOTLIBS)
	echo rootglibs : $(ROOTGLIBS)


