ROOTLIBS=`root-config --libs`
ROOTINCS=`root-config --cflags`

LIBS=$(ROOTLIBS)
INCS=$(ROOTINCS)

G4 = -L$(G4LIB)/$(G4SYSTEM) -lG4digits_hits -lG4error_propagation -lG4event -lG4FR -lG4geometry -lG4global -lG4GMocren -lG4graphics_reps -lG4intercoms -lG4interfaces -lG4materials -lG4modeling -lG4parmodels -lG4particles -lG4persistency -lG4physicslists -lG4processes -lG4RayTracer -lG4readout -lG4run -lG4tracking -lG4track -lG4Tree -lG4visHepRep -lG4vis_management -lG4visXXX -lG4VRML -lG4OpenGL
# G4 = -L$(G4LIB)/$(G4SYSTEM) -lG4digits_hits -lG4error_propagation -lG4event -lG4FR -lG4geometry -lG4gl2ps -lG4global -lG4GMocren -lG4graphics_reps -lG4intercoms -lG4interfaces -lG4materials -lG4modeling -lG4OpenGL -lG4parmodels -lG4particles -lG4persistency -lG4physicslists -lG4processes -lG4RayTracer -lG4readout -lG4run -lG4tracking -lG4track -lG4Tree -lG4visHepRep -lG4vis_management -lG4visXXX -lG4VRML
CLHEP = -L$(CLHEP_LIB_DIR) -lCLHEP
MAGE = -L../../lib -lgenerators -lgeometry -lgerdageometry -lgerdaio -lio -llngsgesetups -lmanagement -lmaterials -lmjdemonstrator -lmjgeometry -lmjio -lmunichteststand -lprocesses -lsandbox -ltonscalegeometry -lwaveform -llegendgeometry -llegendio
MGDO = -L$(MGDODIR)/lib -lMGDOBase -lMGDOGerda -lMGDOGerdaTransforms -lMGDOMajorana -lMGDORoot -lMGDOTransforms
XERCESC = -lxerces-c

all: NeutronInteractionAnalysis

NeutronInteractionAnalysis: NeutronInteractionAnalysis.o ParticleTrack.o MaterialInteraction.o MedianOmatic.o RComp.o
	g++ -std=c++11 NeutronInteractionAnalysis.o ParticleTrack.o MaterialInteraction.o MedianOmatic.o RComp.o $(LIBS) $(MAGE) $(MGDO) $(G4) $(CLHEP) $(XERCESC) -o NeutronInteractionAnalysis

NeutronInteractionAnalysis.o: NeutronInteractionAnalysis.cxx
	g++ -std=c++11 -c -g NeutronInteractionAnalysis.cxx $(INCS)
	
ParticleTrack.o: ../VaLi/ParticleTrack.cxx
	g++ -std=c++11 -c -g ../VaLi/ParticleTrack.cxx $(INCS)
	
MaterialInteraction.o: MaterialInteraction.cxx
	g++ -std=c++11 -c -g MaterialInteraction.cxx $(INCS)
	
MedianOmatic.o: MedianOmatic.cxx
	g++ -std=c++11 -c -g MedianOmatic.cxx $(INCS)
	
RComp.o: ../VaLi/RComp.cxx
	g++ -std=c++11 -c -g ../VaLi/RComp.cxx $(INCS)

clean:
	rm NeutronInteractionAnalysis
	rm -f *.o
