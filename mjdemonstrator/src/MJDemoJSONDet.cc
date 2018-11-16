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
 * $Id: MGheadertemplate.hh,v 1.1 2004-12-09 08:58:35 pandola Exp $
 *      
 * CLASS DECLARATION:  MJDemoJSONDet.cc
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *Creates a detector from parameters read in MGCrystalData
 *
 *
 */
// End class description
//
/**  
 * SPECIAL NOTES:
 *
 */
// 
// --------------------------------------------------------------------------//
/** 
 * AUTHOR: Kris Vorren
 * CONTACT: krisvorren@unc.edu
 * FIRST SUBMISSION: Oct 29, 2013
 * 
 * REVISION:
 * 
 * 10-29-2013, Created, K. Vorren
 */
// --------------------------------------------------------------------------//
#include "G4LogicalVolume.hh"	
#include "G4LogicalVolumeStore.hh"
#include "G4UIcommand.hh"
//#include "G4Tubs.hh"
//#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4ThreeVector.hh"
#include "G4Polycone.hh"

//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "mjio/MJJSONReader.hh"
#include "mjdemonstrator/MJDemoJSONDet.hh"
#include "mjdemonstrator/MJVDemoPart.hh"

//---------------------------------------------------------------------------//

using namespace CLHEP;


MJDemoJSONDet::MJDemoJSONDet(G4String partName, G4String serialNumber) :
  MJVDemoDetector(partName, serialNumber, "MJ80-02-011", "Germanium-Enr")
{
  ReadJSONFile();
}

MJDemoJSONDet::MJDemoJSONDet(const MJDemoJSONDet & rhs) : 
  MJVDemoDetector(rhs), fDetParameters(rhs.fDetParameters)
{;}

MJDemoJSONDet::~MJDemoJSONDet()
{;}

void MJDemoJSONDet::ReadJSONFile()
{
  if(fDetParameters.size())
    fDetParameters.clear();
  MJJSONReader* reader = new MJJSONReader();
  vector<double> r = reader->ReadDetectorRs(fSerialNumber);
  vector<double> z = reader->ReadDetectorZs(fSerialNumber);
  delete reader;

  if(r.size() && z.size() && r.size() == z.size())
    {
      G4int numPlanes = r.size();
      G4double maxZ = 0.0;
      G4double minZ = 0.0;
      G4double maxR = 0.0;
      for(G4int i = 0; i < numPlanes; i++)
	{
	  G4double tempZ = z.at(i);
	  G4double tempR = r.at(i);
	  maxR = (tempR > maxR) ? tempR : maxR;
	  maxZ = (tempZ > maxZ) ? tempZ : maxZ; //return the max of maxZ and temp
	  minZ = (tempZ < minZ) ? tempZ : minZ; //return the min of minZ and temp
	  G4ThreeVector tempVec = G4ThreeVector(r.at(i), tempZ);
	  fDetParameters.push_back(tempVec);
	}
      fCrystalDiameter = 2*maxR;
      
      if((maxZ - minZ) > 0)
	{
	  fCrystalHeight = maxZ - minZ;
	  MGLog(routine) << "Crystal height set to " << fCrystalHeight << "." << endlog;
	}
      else
	MGLog(error) << "Error! Non-positive crystal height!" << endlog;
    }
  else
    {
      fDetParameters.clear();
      fDetParameters.push_back(G4ThreeVector(77.0/2*mm, 33.0/2*mm));
      fDetParameters.push_back(G4ThreeVector(77.0/2*mm, -33.0/2*mm));
      fCrystalHeight = 33*mm;
    }
}

G4LogicalVolume* MJDemoJSONDet::ConstructPart()
{
  G4LogicalVolumeStore *storePtr = G4LogicalVolumeStore::GetInstance();
  G4String logicalName = fPartName + "_JSON" + "_" + fPartMaterial;  
  G4LogicalVolume* pVol = storePtr->GetVolume(logicalName, false);
  if (pVol == NULL){
    G4int numPlanes = fDetParameters.size();
    G4double *r = new G4double[numPlanes];
    G4double *z = new G4double[numPlanes];
    G4double *zeros = new G4double[numPlanes];
    
    for(G4int i = 0; i<numPlanes; i++)
      {
	//originally this class was intended to read (r,z) pairs, now the G4ThreeVector is kept to ensure the r's and z's stay together
	//crystal assembly rotates this for us
	r[i] = fDetParameters.at(/*numPlanes - 1 - */i).getX(); //getR
	z[i] = -fDetParameters.at(/*numPlanes - 1 - */i).getY(); //getZ
	zeros[i] = 0;
      }
    //G4RotationMatrix* rotTest = new G4RotationMatrix(0, pi, 0)
    G4Polycone *crystal = new G4Polycone("crystal", 0, 2.*pi, numPlanes, z, zeros ,r);
    delete[] zeros;
    delete[] r;
    delete[] z;
    
    G4VisAttributes* germaniumVisAtt = new G4VisAttributes(G4Colour(0.878,0.878,0.867)); // New germanium color
    germaniumVisAtt->SetForceWireframe( false );
    
    G4Material *material = G4Material::GetMaterial(this->GetMaterial());
    
    pVol = new G4LogicalVolume(crystal, material, logicalName);
    pVol->SetVisAttributes(germaniumVisAtt); 
    
    MGLog(debugging) << "Created crystal logical from JSON file." << endlog;
  }
  
  else  MGLog(debugging) << "Using pre-existing JSON file geometry" << endlog; 
  return pVol;
}
