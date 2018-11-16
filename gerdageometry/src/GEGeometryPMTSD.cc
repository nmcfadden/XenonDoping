//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                            MaGe Simulation                                //
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
//                                                          
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Original desing by Alexander Klimenko
 */
// 
//---------------------------------------------------------------------------//
/**
 *
 * AUTHOR:  Markus Knapp
 * CONTACT: @CONTACT@
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 *
 */
//---------------------------------------------------------------------------//
//



#include "gerdageometry/GEGeometryPMTSD.hh"
#include "gerdageometry/GEGeometryPMTSDHit.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "Randomize.hh"

#include "io/MGLogger.hh"

G4ThreeVector pos;

GEGeometryPMTSD::GEGeometryPMTSD(G4String name, G4int nCells, G4String colName)
:G4VSensitiveDetector(name),numberOfCells(nCells),HCID(-1)
{
  G4String HCname;
  collectionName.insert(HCname=colName);
  CellID = new int[numberOfCells];
  MGLog(trace) << " ==--> "<< colName << "  "<< name<<"  GEGeometryPMTSD  nCells= "<<nCells <<" SD name " << name<<"  \n" << endlog;

}

GEGeometryPMTSD::~GEGeometryPMTSD()
{
  delete [] CellID;
}

void GEGeometryPMTSD::Initialize(G4HCofThisEvent* /*HCE*/)
{
//	MGLog(trace) << " ==--> GEGeometryPMTSD  collection initialized \n" << endlog;
  PMTCollection = new GEGeometryPMTSDHitsCollection
                      (SensitiveDetectorName,collectionName[0]); 

  for(int j=0;j<numberOfCells;j++)   
    { 
      CellID[j] = -1;
    }
  TimeInit=-10.0;

}

G4bool GEGeometryPMTSD::ProcessHits(G4Step* aStep, G4TouchableHistory* /*ROhist*/)
{

  G4ParticleDefinition* particleType = aStep->GetTrack()->GetDefinition();
  G4String particleName = particleType->GetParticleName();
  G4double edep = aStep->GetTotalEnergyDeposit();
  //  G4double time = aStep->GetPostStepPoint()->GetGlobalTime();   //measured in nanoseconds;   
  //  G4double time = aStep->GetTrack()->GetGlobalTime();
  //  G4cout << "testpoint " << time <<  "\n";  

  if(edep<=0. && (particleName != "opticalphoton")) 
    {
      return false;
    }

  const G4VPhysicalVolume* physVol = aStep->GetPreStepPoint()->GetPhysicalVolume();

  G4int copyID = physVol->GetCopyNo();

  if(CellID[copyID]==-1)
  {
    GEGeometryPMTSDHit* pmtHit = new GEGeometryPMTSDHit(physVol->GetLogicalVolume());
    G4RotationMatrix rotM;

    if(physVol->GetObjectRotation()) 
      {
	rotM = *(physVol->GetObjectRotation());
      }

    pmtHit->SetEdep( edep );
    pmtHit->Set0Ndet(); // Hits number for initial time

    pmtHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    pmtHit->SetTotEnergy(aStep->GetTrack()->GetTotalEnergy() );
    pmtHit->SetHittime( aStep->GetTrack()->GetGlobalTime() );
    pmtHit->SetRot( rotM );
    pmtHit->SetNdet( copyID );
    pmtHit->SetParticleDirection((aStep->GetPreStepPoint())->GetMomentumDirection());
    int icell = PMTCollection->insert( pmtHit );

    CellID[copyID] = icell - 1;

  
//     if( TimeInit == -10.0 ) 
//       {
// 	pmtHit->SetTimeInit(time);
// 	TimeInit = time;
//       }
  }

  else
  { 
    GEGeometryPMTSDHit* pmtHit = new GEGeometryPMTSDHit(physVol->GetLogicalVolume());
    pmtHit->NumDet();
    pmtHit->AddEdep(edep);
    //   pmtHit->AddTime(time);
    pmtHit->SetNdet(copyID+1);
    pmtHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    pmtHit->SetTotEnergy(aStep->GetTrack()->GetTotalEnergy() );    
    pmtHit->SetHittime( aStep->GetTrack()->GetGlobalTime() );
    pmtHit->SetParticleDirection((aStep->GetPreStepPoint())->GetMomentumDirection());
    PMTCollection->insert( pmtHit );
  }

  return true;
}

G4bool GEGeometryPMTSD::ProcessHits_constStep(const G4Step* aStep, G4TouchableHistory* /*ROhist*/)
{

  G4ParticleDefinition* particleType = aStep->GetTrack()->GetDefinition();
  G4String particleName = particleType->GetParticleName();
  G4double edep = aStep->GetTotalEnergyDeposit();
  //  G4double time = aStep->GetPostStepPoint()->GetGlobalTime();   //measured in nanoseconds;
  //  G4double time = aStep->GetTrack()->GetGlobalTime();
  //  G4cout << "testpoint " << time <<  "\n";

  if(edep<=0. && (particleName != "opticalphoton"))
    {
      return false;
    }

  const G4VPhysicalVolume* physVol = aStep->GetPostStepPoint()->GetPhysicalVolume();
  G4int copyID = physVol->GetCopyNo();

  if(CellID[copyID]==-1)
  {
    GEGeometryPMTSDHit* pmtHit = new GEGeometryPMTSDHit(physVol->GetLogicalVolume());
    G4RotationMatrix rotM;

    if(physVol->GetObjectRotation())
      {
	rotM = *(physVol->GetObjectRotation());
      }

    pmtHit->SetEdep( edep );
    pmtHit->Set0Ndet(); // Hits number for initial time

    pmtHit->SetPos(aStep->GetPostStepPoint()->GetPosition());

    //pmtHit->SetTotEnergy(edep);//aStep->GetTrack()->GetTotalEnergy() );
    pmtHit->SetTotEnergy(aStep->GetTrack()->GetTotalEnergy() );
    pmtHit->SetHittime( aStep->GetTrack()->GetGlobalTime() );
    pmtHit->SetRot( rotM );
    pmtHit->SetNdet( copyID );
    pmtHit->SetParticleDirection((aStep->GetPreStepPoint())->GetMomentumDirection());
    int icell = PMTCollection->insert( pmtHit );

    CellID[copyID] = icell - 1;


//     if( TimeInit == -10.0 )
//       {
// 	pmtHit->SetTimeInit(time);
// 	TimeInit = time;
//       }
  }

  else
  {
    GEGeometryPMTSDHit* pmtHit = new GEGeometryPMTSDHit(physVol->GetLogicalVolume());
    pmtHit->NumDet();
    pmtHit->AddEdep(edep);
    //   pmtHit->AddTime(time);
    pmtHit->SetNdet(copyID+1);
    pmtHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
   // pmtHit->SetTotEnergy(edep); //aStep->GetTrack()->GetTotalEnergy() );
    pmtHit->SetTotEnergy(aStep->GetTrack()->GetTotalEnergy() );
    pmtHit->SetHittime( aStep->GetTrack()->GetGlobalTime() );
    pmtHit->SetParticleDirection((aStep->GetPreStepPoint())->GetMomentumDirection());
    PMTCollection->insert( pmtHit );
  }

  return true;
}

void GEGeometryPMTSD::EndOfEvent(G4HCofThisEvent*HCE)
{
  if(HCID<0)
    {
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }

  HCE->AddHitsCollection( HCID, PMTCollection );
}

void GEGeometryPMTSD::clear()
{

} 

void GEGeometryPMTSD::DrawAll()
{
  
} 

void GEGeometryPMTSD::PrintAll()
{

} 




