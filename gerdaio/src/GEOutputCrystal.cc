//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//
/**
 * CLASS IMPLEMENTATION:
 *   GEOutputCrystal.cc
 *
 * DESCRIPTION:
 *   It is a class not only for output but also trying to get full
 *   control of the simulation in a general way. Detailed information
 *   please find in the header file.
 *
 * REVISION: MM-DD-YYYY
 *
 * 11-01-2007: created, Jing Liu
 * 04-08-2008: added TimeWindow functions to chop event, Jing Liu
 * 05-29-2008: added Super-Siegfried segmentation, Jing Liu
 * 10-11-2010: 1) in GEOutputCrystal::saveCrystalInfo. Crystal names
 *                not necessarily are "olandI" etc. anymore since gdml is a part of Geant4. Alex
 *             2) in GEOutputCrystal::saveCrystalInfo. Segment number for Siegfried and SuSie
 *                is now defined in more optimized way using calculations, not 'if's
 *
 */

// Geant4 headers
#include "Randomize.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4ParticleDefinition.hh"
// MaGe headers
#include "io/MGLogger.hh"
#include "gerdaio/GEOutputCrystal.hh"

// #include "G4PhysicalVolumeStore.hh" // For tests: Alex

using namespace CLHEP;

GEOutputCrystal::GEOutputCrystal(G4bool isMother):
   MGOutputRoot(isMother),
   saveSource(false),
   saveHitsInSensitiveVolume(false),
   saveHitsInOtherVolume(false)
{
   SetSchemaDefined(false);
}
//---------------------------------------------------------
GEOutputCrystal::~GEOutputCrystal()
{
}
//---------------------------------------------------------
void GEOutputCrystal::BeginOfRunAction()
{
   if(IsMother()) OpenFile();
   DefineSchema();
   
   ranSeed = CLHEP::HepRandom::getTheSeed();
}
//---------------------------------------------------------
void GEOutputCrystal::DefineSchema()
{
   if(!SchemaDefined()) {
      if(fTree == NULL) {
         if(!IsMother())
            MGLog(warning)
               <<"No tree assigned to child output class." 
               <<endlog;
         fTree = new TTree("fTree", "Crystal Output Tree");
      }
      
      fTree->Branch("ranSeed", &ranSeed, "ranSeed/I");
      fTree->Branch("eventID", &eventID, "eventID/I");

      if (saveSource) {
         fTree->Branch("Nsrc", &Nsrc, "Nsrc/I");

         fTree->Branch("srcX", srcX, "srcX[Nsrc]/F");
         fTree->Branch("srcY", srcY, "srcY[Nsrc]/F");
         fTree->Branch("srcZ", srcZ, "srcZ[Nsrc]/F");
         fTree->Branch("srcT", srcT, "srcT[Nsrc]/F");

         fTree->Branch("srcPx", srcPx, "srcPx[Nsrc]/F");
         fTree->Branch("srcPy", srcPy, "srcPy[Nsrc]/F");
         fTree->Branch("srcPz", srcPz, "srcPz[Nsrc]/F");
         fTree->Branch("srcEk", srcEk, "srcEk[Nsrc]/F");      

         fTree->Branch("srcMass", srcMass, "srcMass[Nsrc]/F");
         fTree->Branch("srcCharge", srcCharge, "srcCharge[Nsrc]/F");
         fTree->Branch("srcPDG", srcPDG, "srcPDG[Nsrc]/I");
      }
      
      if (saveHitsInSensitiveVolume) {
         fTree->Branch("Nhits", &Nhits, "Nhits/I");

         fTree->Branch("hitWorldX", hitWorldX, "hitWorldX[Nhits]/F");
         fTree->Branch("hitWorldY", hitWorldY, "hitWorldY[Nhits]/F");
         fTree->Branch("hitWorldZ", hitWorldZ, "hitWorldZ[Nhits]/F");
         
         fTree->Branch("hitX", hitX, "hitX[Nhits]/F");
         fTree->Branch("hitY", hitY, "hitY[Nhits]/F");
         fTree->Branch("hitZ", hitZ, "hitZ[Nhits]/F");
         
         fTree->Branch("hitR", hitR, "hitR[Nhits]/F");
         fTree->Branch("hitPhi", hitPhi, "hitPhi[Nhits]/F");
      
         fTree->Branch("hitPx", hitPx, "hitPx[Nhits]/F");
         fTree->Branch("hitPy", hitPy, "hitPy[Nhits]/F");
         fTree->Branch("hitPz", hitPz, "hitPz[Nhits]/F");
      
         fTree->Branch("hitDeltaE", hitDeltaE, "hitDeltaE[Nhits]/F");
         fTree->Branch("hitE", hitE, "hitE[Nhits]/F");
         fTree->Branch("hitT", hitT, "hitT[Nhits]/F");

         fTree->Branch("hitCharge",hitCharge,"hitCharge[Nhits]/F");
         fTree->Branch("hitPDG", hitPDG, "hitPDG[Nhits]/I");
         fTree->Branch("hitBaryonNumber",hitBaryonNumber,"hitBaryonNumber[Nhits]/I");

         fTree->Branch("hitProcess", hitProcess, "hitProcess[Nhits]/I");
      
         fTree->Branch("hitStepLength", hitStepLength, "hitStepLength[Nhits]/F");
         fTree->Branch("hitTrackLength", hitTrackLength, "hitTrackLength[Nhits]/F");

         fTree->Branch("hitTrackParentID", hitTrackParentID, "hitTrackParentID[Nhits]/I");
         fTree->Branch("hitTrackID", hitTrackID, "hitTrackID[Nhits]/I");

         fTree->Branch("hitCrystalType", hitCrystalType, "hitCrystalType[Nhits]/I");
         fTree->Branch("hitCrystalID", hitCrystalID, "hitCrystalID[Nhits]/I");
         fTree->Branch("hitSegmentID", hitSegmentID, "hitSegmentID[Nhits]/I");
         
         fTree->Branch("segEnergy", segEnergy, "segEnergy[4][20]/F");
         fTree->Branch("segNhits", segNhits, "segNhits[4][20]/I");
      }
      
      SetSchemaDefined(true);
   }
}
//---------------------------------------------------------
void GEOutputCrystal::BeginOfEventAction(const G4Event* event)
{
   eventID = event->GetEventID();

// initialization:
   if (saveSource) Nsrc = 0;
   if (saveHitsInSensitiveVolume) {
      Nhits  = 0;
      
      for (G4int i=0; i<maxNcrystals; i++) 
         for (G4int j=0; j<maxNsegments; j++) {
            segEnergy[i][j] = 0;
            segNhits[i][j] = 0;
         }
   }
   if (fUseTimeWindow) Nnuclei = 0;
   
// fill source information:
   if (saveSource) {
      G4int srcCounter = 0;
      G4int Nvtx = event->GetNumberOfPrimaryVertex();
      for (G4int i=0; i<Nvtx; i++) {
         G4PrimaryVertex* primaryVertex = event->GetPrimaryVertex(i);
         if(!primaryVertex) MGLog(fatal)<<"Bad primary vertex pointer!"<<endlog;
      
         G4int Npar = primaryVertex->GetNumberOfParticle();
         for (G4int j=0; j<Npar; j++) {
            srcCounter++;
            if (srcCounter>maxNsrc) {
               MGLog(warning)
                  <<"\n"
                  <<"\t"<<Nvtx<<" primary vertex generated.\n"
                  <<"\t"<<"In each vertex, "<<Npar<<" primary particles generated.\n"
                  <<"\t"<<"Maximum only "<<maxNsrc<<" primary particles per event are supported.\n"
                  <<"\t"<<"The information of extra primary particles will not be saved."
                  <<endlog;
               goto theEndOfLoop;
            }
            
            G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary(j);
            if(!primaryParticle) MGLog(fatal)<<"Bad primary particle pointer!"<<endlog;
               
            srcX[Nsrc] = primaryVertex->GetX0()/cm;
            srcY[Nsrc] = primaryVertex->GetY0()/cm;
            srcZ[Nsrc] = primaryVertex->GetZ0()/cm;
            srcT[Nsrc] = primaryVertex->GetT0()/ns;           
            
            G4ThreeVector P = primaryParticle->GetMomentum()/keV;
            srcPx[Nsrc] = P.x();
            srcPy[Nsrc] = P.y();
            srcPz[Nsrc] = P.z();
               
            G4double M = primaryParticle->GetMass()/keV;
            srcMass[Nsrc] = M;

            if (P.mag() < M/100)
               srcEk[Nsrc] = P.mag2()/(2*M);
            else
               srcEk[Nsrc] = sqrt(P.mag2()+M*M)-M;

            srcCharge[Nsrc] = primaryParticle->GetCharge();
            srcPDG[Nsrc] = primaryParticle->GetPDGcode();

            Nsrc++;
         }
      }
   }
theEndOfLoop:
   MGLog(debugging)<<"beginning of event "<<eventID<<endlog;
}
//---------------------------------------------------------
void GEOutputCrystal::RootSteppingAction(const G4Step* step)
{
   if (saveHitsInSensitiveVolume && Nhits<maxNhits) {
      G4float Edeposited = step->GetTotalEnergyDeposit();
      G4String volumeName = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();

/*
  // Left over here for tests. Alex.

      G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
      G4int nVolumes = (G4int) volumeStore->size();
      G4String candidateList;
      for(G4int i=0;i<nVolumes;i++) {
        candidateList += (*volumeStore)[i]->GetName();
        candidateList += ", ";
      }
*/

      if (saveHitsInOtherVolume) {
         saveHitsInfo(step);
         if ((volumeName.find("ensitive")!=string::npos) && (Edeposited>0.0))
            saveCrystalInfo(step,volumeName);
         Nhits++;
      }
      else{
         if ((volumeName.find("ensitive")!=string::npos) && (Edeposited>0.0)) {
            saveHitsInfo(step);
            saveCrystalInfo(step,volumeName);
            Nhits++;
         }
      }
      
      if (Nhits == maxNhits) {
         MGLog(warning)
            <<"\n"
            <<"\t"<<Nhits<<" hits are created.\n"
            <<"\t"<<"Maximum only this amount of hits per event are supported.\n"
            <<"\t"<<"Extra hits in event "<<eventID<<" will not be saved."
            <<endlog;
      }
   }
   else
      step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
}
//---------------------------------------------------------
void GEOutputCrystal::EndOfEventAction(const G4Event*)
{
   if(IsMother() && (saveSource || Nhits>0)) FillTree();
   fOffsetTime = 0.0;
   MGLog(debugging)<<"end of event "<<eventID<<endlog;
}
//---------------------------------------------------------
void GEOutputCrystal::EndOfRunAction()
{
   if(IsMother()) CloseFile();
}
//---------------------------------------------------------
void GEOutputCrystal::WritePartialEvent(const G4Event*)
{
   MGLog(debugging)<<"writing part of the event..."<<endlog;
   MGLog(debugging)<<"fOffsetTime = "<<fOffsetTime<<endlog;
   if(IsMother() && (saveSource || Nhits>0)) FillTree();
}
//---------------------------------------------------------
void GEOutputCrystal::ResetPartialEvent(const G4Event*)
{
   Nsrc = Nnuclei;
   for (G4int i=0; i<Nnuclei; i++) {
      srcX[i] = MotherNuclearX[i];
      srcY[i] = MotherNuclearY[i];
      srcZ[i] = MotherNuclearZ[i];
      srcT[i] = fOffsetTime; // former fOffsetTime overlapped if Nnuclei>1
   
      srcPx[i] = 0;
      srcPy[i] = 0;
      srcPz[i] = 0;
      srcEk[i] = 0;
   
      srcMass[i] = MotherNuclearMass[i];
      srcCharge[i] = MotherNuclearCharge[i];
      srcPDG[i] = MotherNuclearPDG[i];
   }

   if (saveHitsInSensitiveVolume) {
      Nhits  = 0;
      
      for (G4int i=0; i<maxNcrystals; i++) 
         for (G4int j=0; j<maxNsegments; j++) {
            segEnergy[i][j] = 0;
            segNhits[i][j] = 0;
         }
   }
   Nnuclei = 0;

   MGLog(debugging)<<"processing tracks in waiting stack..."<<endlog;
}
//---------------------------------------------------------
void GEOutputCrystal::saveHitsInfo(const G4Step* step)
{
   G4Track* track = step->GetTrack();

   G4ThreeVector position = track->GetPosition();
   hitWorldX[Nhits] = position.x()/cm;
   hitWorldY[Nhits] = position.y()/cm;
   hitWorldZ[Nhits] = position.z()/cm;
      
   G4ThreeVector localPosition = track->GetTouchableHandle()->GetHistory()
      ->GetTopTransform().TransformPoint(position);  
   hitX[Nhits] = localPosition.x()/cm;
   hitY[Nhits] = localPosition.y()/cm;
   hitZ[Nhits] = localPosition.z()/cm;
   hitR[Nhits] = localPosition.perp()/cm;
   hitPhi[Nhits] = localPosition.phi()/rad;


   G4ThreeVector momentum = track->GetMomentum()/keV;
   hitPx[Nhits] = momentum.x();
   hitPy[Nhits] = momentum.y();
   hitPz[Nhits] = momentum.z();

   hitDeltaE[Nhits] = step->GetDeltaEnergy()/keV;
   hitE[Nhits] = step->GetTotalEnergyDeposit()/keV;
   hitT[Nhits] = track->GetGlobalTime()/ns;

   G4ParticleDefinition* particle = track->GetDefinition();
   hitCharge[Nhits] = particle->GetPDGCharge();      
   hitPDG[Nhits] = particle->GetPDGEncoding();      
   hitBaryonNumber[Nhits] = particle->GetBaryonNumber();
   
   G4String processName = step->GetPostStepPoint()
      ->GetProcessDefinedStep()->GetProcessName();   
   hitProcess[Nhits] = ProcessIndex(processName);

   hitStepLength[Nhits] = step->GetStepLength()/cm;
   hitTrackLength[Nhits] = track->GetTrackLength()/cm;         
      
   hitTrackParentID[Nhits] = track->GetParentID();
   hitTrackID[Nhits] = track->GetTrackID();

   // initialize crystal information
   hitCrystalType[Nhits] = -1;
   hitCrystalID[Nhits] = -1;
   hitSegmentID[Nhits] = -1;

   // save info of delayed radioactive decay
   if (hitProcess[Nhits]==31 && fUseTimeWindow && particle->GetPDGLifeTime()>fTimeWindow) {
      MotherNuclearX[Nnuclei] = hitWorldX[Nhits];
      MotherNuclearY[Nnuclei] = hitWorldY[Nhits];
      MotherNuclearZ[Nnuclei] = hitWorldZ[Nhits];

      MotherNuclearMass[Nnuclei] = G4float(hitBaryonNumber[Nhits]);
      MotherNuclearCharge[Nnuclei] = hitCharge[Nhits];
      MotherNuclearPDG[Nnuclei] = hitPDG[Nhits];
      
      Nnuclei++;
      if (Nnuclei>5) {
	 MGLog(warning)<<"\n\t More than 6 radioactive nuclei created in event "
	    <<eventID<<".\n\t This is not supported currently."
	    <<"\n\t Information of some of the nuclei will be lost."<<endlog;
	 Nnuclei = 5;
      }
   }
}
//---------------------------------------------------------
void GEOutputCrystal::saveCrystalInfo(const G4Step* step, G4String volumeName)
{
   G4String crystalName = volumeName.substr(volumeName.find("_")+2);
   //cout<<crystalName<<" = crystalName"<<endl;
   //cout<<volumeName<<" = volumeName"<<endl;
   G4int crystalID = step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo()+1;
   
   if (crystalID>maxNcrystals) {
      MGLog(warning)<<"\n\t Current crystal ID = "<<crystalID
                    <<"\n\t Maximum only "<<maxNcrystals<<" crystals are supported."
                    <<"\n\t crystal and segment IDs are assigned to be 0 for hits in extra crystals."
                    <<endlog;

      crystalID = 0;
   }

                      // For segments 1-18 of Siegfried and Susie:
   G4int    lID = -1; // Layer ID: 0 for bottom (seg.  1 -  6)
                      //           1 for middle (seg.  7 - 12)
                      //           2 for top    (seg. 13 - 18)

   G4int    cID = -1; // Column ID: 1 for phi=[ 0,     pi/3)
                      //            2 for phi=[ pi/3,  2pi/3)
                      //            3 for phi=[ 2pi/3, pi)
                      //            4 for phi=[-pi,   -2pi/3)
                      //            5 for phi=[-2pi/3,-pi/3)
                      //            6 for phi=[-pi/3,  0)


   // For RolandI:
   if ((crystalID != 0) && (crystalName.contains("olandI"))) {
      hitCrystalType[Nhits] = 1;
      hitCrystalID[Nhits] = crystalID;
      hitSegmentID[Nhits] = 0;

      segEnergy[crystalID][0] += hitE[Nhits]; // energy deposited in the whole crystal
      segNhits[crystalID][0]++; // number of hits in the whole crystal
   }

   // For Siegfried:
   else if ((crystalID != 0) && (crystalName.contains("iegfried"))) {
      hitCrystalType[Nhits] = 4;
      hitCrystalID[Nhits] = crystalID;

      G4double H = 7.0 * cm;
      G4double Pi = 3.14159265358979323846 * rad;

      // Get layer ID of the hit: divide by the segment height
      lID = int((hitZ[Nhits]*cm+H/2.0)/(H/3.0));
      if (lID>2) lID=2; // To be safe, should not happen
      if (lID<0) lID=0; // To be safe, should not happen

      // Get column ID of the hit: divide by 60 deg.
      cID = int((hitPhi[Nhits]*rad+2*Pi)/(Pi/3.))+1;// +2Pi -> make the angle positive and rotate the scheme as defined above (in cID)
                                                    // +1   -> starting from 1st segment, not from 0th
      while (cID>6) cID-=6;
      while (cID<1) cID+=6; // To be safe, should not happen

      hitSegmentID[Nhits] = lID * 6 + cID;

      segEnergy[crystalID][0] += hitE[Nhits]; // energy deposited in the whole crystal
      segEnergy[crystalID][hitSegmentID[Nhits]] += hitE[Nhits]; // energy deposited in a segment

      segNhits[crystalID][0]++; // number of hits in the whole crystal
      segNhits[crystalID][hitSegmentID[Nhits]]++; // number of hits in a segment
   }

   // For Super-Siegfried:
   else if ((crystalID != 0) && (crystalName.contains("uSie"))) {
      hitCrystalType[Nhits] = 5;
      hitCrystalID[Nhits] = crystalID;

      G4double H = 7.0 * cm;
      G4double Pi = 3.14159265358979323846 * rad;

      if (hitZ[Nhits]*cm > H/2-0.5*cm)
      {
         hitSegmentID[Nhits] = 19;
      }
      else
      { 
         // Get layer ID of the hit: divide by the segment height
         lID = int((hitZ[Nhits]*cm+H/2.0)/(H/3.0));
         if (lID>2) lID=2; // To be safe, should not happen
         if (lID<0) lID=0; // To be safe, should not happen

         // Get column ID of the hit: divide by 60 deg.
         cID = int((hitPhi[Nhits]*rad+2*Pi)/(Pi/3.))+1;// +2Pi -> make the angle positive and rotate the scheme as defined above (in cID)
                                                       // +1   -> starting from 1st segment, not from 0th
         while (cID>6) cID-=6;
         while (cID<1) cID+=6; // To be safe, should not happen

         hitSegmentID[Nhits] = lID * 6 + cID;
      }

      segEnergy[crystalID][0] += hitE[Nhits]; // energy deposited in the whole crystal
      segEnergy[crystalID][hitSegmentID[Nhits]] += hitE[Nhits]; // energy deposited in a segment

      segNhits[crystalID][0]++; // number of hits in the whole crystal
      segNhits[crystalID][hitSegmentID[Nhits]]++; // number of hits in a segment
   }
   // for sensitive volumes other than a crystal
   else {
      hitCrystalType[Nhits] = 0;
      hitCrystalID[Nhits] = 0;
      hitSegmentID[Nhits] = 0;

      segEnergy[0][0] += hitE[Nhits]; // energy deposited in the whole volume
      segNhits[0][0]++; // number of hits in the whole volume
   }
}
//---------------------------------------------------------
G4int GEOutputCrystal::ProcessIndex(G4String processName) 
{
   if      (processName == "AlphaInelastic")                return 1;
   else if (processName == "AntiNeutronAnnihilationAtRest") return 2;
   else if (processName == "AntiNeutronInelasti")           return 3;
   else if (processName == "AntiProtonAnnihilationAtRest")  return 4;
   else if (processName == "AntiProtonInelastic")           return 5;
   else if (processName == "Decay")                         return 6;
   else if (processName == "DeuteronInelastic")             return 7;
   else if (processName == "ElectroNuclear")                return 8;
   else if (processName == "HadronCapture")                 return 9;
   else if (processName == "HadronElastic")                 return 10;
   else if (processName == "KaonMinusAbsorptionAtRest")     return 11;
   else if (processName == "KaonMinusInelastic")            return 12;
   else if (processName == "KaonPlusInelastic")             return 13;
   else if (processName == "KaonZeroLInelastic")            return 14;
   else if (processName == "KaonZeroSInelastic")            return 15;
   else if (processName == "LowEnBrem")                     return 16;
   else if (processName == "LowEnCompton")                  return 17;
   else if (processName == "LowEnConversion")               return 18;
   else if (processName == "LowEnPhotoElec")                return 19;
   else if (processName == "LowEnRayleigh")                 return 20;
   else if (processName == "LowEnergyIoni")                 return 21;
   else if (processName == "MGProcessesMaxTimeCuts")        return 22;
   else if (processName == "MGProcessesMinEkineCuts")       return 23;
   else if (processName == "NeutronInelastic")              return 24;
   else if (processName == "PhotonInelastic")               return 25;
   else if (processName == "PiMinusAbsorptionAtRest")       return 26;
   else if (processName == "PionMinusInelastic")            return 27;
   else if (processName == "PionPlusInelastic")             return 28;
   else if (processName == "PositronNuclear")               return 29;
   else if (processName == "ProtonInelastic")               return 30;
   else if (processName == "RadioactiveDecay")              return 31;
   else if (processName == "SynRad")                        return 32;
   else if (processName == "Transportation")                return 33;
   else if (processName == "TritonInelastic")               return 34;
   else if (processName == "annihil")                       return 35;
   else if (processName == "eBrem")                         return 36;
   else if (processName == "eIoni")                         return 37;
   else if (processName == "hIoni")                         return 38;
   else if (processName == "hLowEIoni")                     return 39;
   else if (processName == "msc")                           return 40;
   else if (processName == "muBrems")                       return 41;
   else if (processName == "muIoni")                        return 42;
   else if (processName == "muMinusCaptureAtRest")          return 43;
   else if (processName == "muNuclear")                     return 44;
   else if (processName == "muPairProd")                    return 45;
   else                                                     return -1;
}
