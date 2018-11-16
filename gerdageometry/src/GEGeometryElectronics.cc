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
 * AUTHOR: Kevin Kroeninger
 * CONTACT: kroening *at* mppmu *dot* mpg *dot* de
 * FIRST SUBMISSION: 01-19-2005
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 05-10-2007, Added new design for matrix and its counterpart and pogopins and pads, Daniel 
 * 10-15-2007, Completly changed Junctionboard and sled design, Daniel 
 * 10-16-2007  Added Cablechain and Chain, Daniel
 * 10-17-2007  Added cable to the outside world, Daniel
 * 11-26-2007  Added Signal and HV cable from detector to JB, Daniel
 * 07-09-2008  (i)  changes due to the phase-1 cable that has been added, Jens
               (ii) set some copy numbers to zero, Jens
 * 07-29-2008 (jens)
 *            - changed positions and geometry of phase-2 cables, i.e. signal cable, signal connection cable, hv cable
 *              --> removed hv connection cable (hv-vertical cable has been extended)
 * 04-10-2016 - Modified such that there is no crash for geometry-type 5 and 11, Janina
 */

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"
#include "G4RotationMatrix.hh"


//#include "gerdageometry/GEGeometrySD.hh"
#include "io/MGLogger.hh"
#include "gerdageometry/GEGeometryElectronics.hh"       
#include "gerdageometry/GEGeometryHolderPhase1Parts.hh"       
#include "gerdageometry/GEGeometryDetectorDB.hh"       
#include "gerdageometry/GEGeometryStringStuff.hh"       

// --------------------------------------------------------------------------- 

using namespace CLHEP;

GEGeometryElectronics::GEGeometryElectronics(G4int col, GEGeometryDetectorDB* detectordb)
{
  
  fDetectorDB                      = detectordb;
  fCrystalType                 = detectordb -> GetCrystalType(col, 0); 
  fDetectorParameters          = detectordb -> GetDetectorParameters(fCrystalType); 
  fCrystalGeometry             = fDetectorParameters -> GetGeometry(); 
  
  if(fCrystalGeometry == 1 || fCrystalGeometry == 4 || fCrystalGeometry== 2){
  //condition needed here as for Type 5,11 (Phase2) some of these logical volumes does not exist
  //Phase2 electronics at the moment in GEGeometryLArInstHybrid.cc 
 
  fColumnNumber                    = detectordb -> GetColumnNumber() - 1; 
  fColumnGeom1Number               = detectordb -> GetColumnGeom1Number() - 1; 
  fColumnGeom2Number               = detectordb -> GetColumnGeom2Number() - 1; 
  fElectronicsBoardHeight          = detectordb -> GetElectronicsBoardHeight(); 
  fElectronicsBoxHeight            = detectordb -> GetElectronicsBoxHeight(); 
  fElectronicsCableHeight          = detectordb -> GetElectronicsCableHeight(); 
  fHolderSideBarHeight             = detectordb -> GetHolderSideBarHeight(); 
  fHolderSideBarRadius             = detectordb -> GetHolderSideBarRadius(); 
  
  fElectronicsJunctionSledHeight       = detectordb -> GetElectronicsJunctionSledHeight();
  fElectronicsJunctionSledPinsHeight   = detectordb -> GetElectronicsJunctionSledPinsHeight();
  fElectronicsJunctionSledSpacerHeight = detectordb -> GetElectronicsJunctionSledSpacerHeight();
  fElectronicsJunctionSledCablesHeight = detectordb -> GetElectronicsJunctionSledCablesHeight();

  fElectronicsKaptonPreAmpsPinsCableHeight = detectordb ->GetElectronicsKaptonPreAmpsPinsCableHeight();

  fElectronicsCableChainHeight           = detectordb -> GetElectronicsCableChainHeight();
  fElectronicsCableChainDistance         = detectordb -> GetElectronicsCableChainDistance();

  fElectronicsChainHeight                = detectordb -> GetElectronicsChainHeight();
  fElectronicsChainRadius                = detectordb -> GetElectronicsChainRadius();

  fKaptonCableLength                     = detectordb -> GetKaptonConnectionCableLenght();
  fKaptonCableThickness                  = detectordb -> GetKaptonConnectionCableThickness();

  fDetectorMotherVolume                       = detectordb -> GetDetectorMotherVolume(); 

  fElectronicsBoxLogical                      = detectordb -> GetElectronicsBoxLogical(); 
  fElectronicsCableLogical                    = detectordb -> GetElectronicsCableLogical(); 
  fElectronicsJunctionBoardLogical            = detectordb -> GetElectronicsJunctionBoardLogical();
  fElectronicsKaptonPreAmpsPinsCableLogical   = detectordb -> GetElectronicsKaptonPreAmpsPinsCableLogical();
  fElectronicsJunctionSledLogical             = detectordb -> GetElectronicsJunctionSledLogical();
  fElectronicsJunctionSledPinsLogical         = detectordb -> GetElectronicsJunctionSledPinsLogical();
  fElectronicsJunctionSledSpacerLogical       = detectordb -> GetElectronicsJunctionSledSpacerLogical();
  fElectronicsJunctionSledCablesLogical       = detectordb -> GetElectronicsJunctionSledCablesLogical();

  fElectronicsCableChainLogical               = detectordb -> GetElectronicsCableChainLogical();
  fElectronicsChainLogical                    = detectordb -> GetElectronicsChainLogical();

  fCrystalPosition                            = detectordb -> GetCrystalPosition(col,0); 


  fKaptonConnectionCableLogical = detectordb-> GetKaptonConnectionCable(col); 
  fCopperLinesConnectionLogical = detectordb-> GetCopperLinesConnection(col);



  

  fElectronicsPhaseIHorizontalBarLogical      = detectordb -> GetElectronicsPhaseIHorizontalBarLogical();
  fElectronicsPhaseIHorizontalBar_InnerHeight = detectordb -> GetElectronicsPhaseIHorizontalBar_InnerHeight();

  fStringStuff                   = detectordb->GetStringStuff(col);
  fElectronicsSideBarLogical     = fStringStuff -> GetElectronicsSideBarLogical(); 
  fElectronicsSideBarHeight      = fStringStuff -> GetElectronicsSideBarHeight(); 
  fElectronicsSideBarZPosition   = fStringStuff -> GetElectronicsSideBarZPosition(); 
  fElectronicsSideBarXOffset     = fStringStuff -> GetElectronicsSideBarXOffset(); 
  fSegmentStartingAngleDataBase = detectordb -> GetSegmentationOffsetAngle();
}
}

// --------------------------------------------------------------------------- 

GEGeometryElectronics::~GEGeometryElectronics()
{
  // Never delete Physical Volumes, this is done by Geant4
  // -----------------------------------------------------

  //  delete fElectronicsBoxPhysical; 
  //  delete fElectronicsLeftBarPhysical; 
  //  delete fElectronicsRightBarPhysical; 
  //  
  //  
  //  delete fElectronicsJunctionSledPhysical;
  //  delete fElectronicsJunctionSledPinsPhysical;
  //  delete fElectronicsJunctionSledCablesPhysical;
  //  
  //  delete fElectronicsCableChainPhysical;
  //  delete fElectronicsChainPhysical;
  //  
  //  
  //  delete fElectronicsCablePhysical; 
  //  
  //  
  //  delete fKaptonConnectionCablePhysical; 
  //  delete fCopperLinesConnectionPhysical;
  //  delete fHVConnectionCablePhysical;
  //  delete fElectronicsKaptonPreAmpsPinsCablePhysical;
} 

// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryElectronics::GetBoardPosition()
{



  G4ThreeVector position; 
  G4double      z = 0.0 * mm; 


  position.setX(fCrystalPosition.getX()); 
  position.setY(fCrystalPosition.getY()); 


  if(fCrystalGeometry==1 || fCrystalGeometry==4)
      z = fElectronicsSideBarZPosition + 0.5*fElectronicsSideBarHeight;
  else if(fCrystalGeometry==2)
      z = fCrystalPosition.getZ() + 0.5*fHolderSideBarHeight + fElectronicsSideBarHeight;

  position.setZ(z); 

  return position; 

} 

// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryElectronics::GetBoxPosition()
{

  G4ThreeVector position; 
  G4double      z; 
  G4double      x;  
  position = this -> GetBoardPosition(); 
  
  z = position.getZ() + 0.5 * fElectronicsBoxHeight + 0.5 * fElectronicsBoardHeight + 0.5 *mm; 
  x = position.getX() + 3.5 * cm; 

  position.setZ(z); 
  position.setX(x); 
  return position; 

} 

// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryElectronics::GetLeftBarPosition()
{
  
  G4ThreeVector position = fCrystalPosition; 

  G4double x = position.getX() - fElectronicsSideBarXOffset; 
  G4double z = fElectronicsSideBarZPosition;

  position.setX(x); 
  position.setZ(z); 

  return position; 
  
}

// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryElectronics::GetRightBarPosition()
{
  
  G4ThreeVector position = fCrystalPosition; 

  G4double x = position.getX() + fElectronicsSideBarXOffset; 
  G4double z = fElectronicsSideBarZPosition;

  position.setX(x); 
  position.setZ(z); 

  return position; 
  
}


// --------------------------------------------------------------------------- 
G4ThreeVector GEGeometryElectronics::GetPosition_ElectronicsPhaseIHorizontalBar()
{
    G4ThreeVector my3vecHB;
    my3vecHB.setX(  fCrystalPosition.getX()  );
    my3vecHB.setY(  fCrystalPosition.getY()  );
    my3vecHB.setZ(  fElectronicsSideBarZPosition - 0.5*fElectronicsSideBarHeight);

    return my3vecHB;  
}

// --------------------------------------------------------------------------- 
G4ThreeVector GEGeometryElectronics::GetJunctionSledPosition()
{
  
  G4ThreeVector position = this->GetBoardPosition();

  G4double z =  position.getZ() + fElectronicsJunctionSledHeight * 0.5 - 13.95*mm; 


  position.setZ(z);
     
  return position; 
  
}
// --------------------------------------------------------------------------- 
G4ThreeVector GEGeometryElectronics::GetKaptonPreAmpsPinsCablePosition()
{
  
  G4ThreeVector position = this->GetBoardPosition();

  G4double z =  position.getZ() + 0.5 * fElectronicsBoardHeight; 


  position.setZ(z);
     
  return position; 
  
}
// --------------------------------------------------------------------------- 

G4ThreeVector GEGeometryElectronics::GetJunctionSledPinsPosition()
{
  
  G4ThreeVector position = this->GetJunctionSledPosition(); 


  G4double z =  position.getZ() + fElectronicsJunctionSledHeight * 0.5 + fElectronicsJunctionSledPinsHeight * 0.5 + 4.4 *mm;

  position.setZ(z);
   
  return position; 
  
}
// --------------------------------------------------------------------------- 
G4ThreeVector GEGeometryElectronics::GetJunctionSledSpacerPosition()
{
  G4ThreeVector position = this->GetJunctionSledPinsPosition(); 
  G4double z =  position.getZ() + fElectronicsJunctionSledPinsHeight * 0.5  + fElectronicsJunctionSledSpacerHeight * 0.5 +11.075 *mm;   
  G4double x =  position.getX() - 3 *mm;   
  G4double y =  position.getY() - 22 *mm;   

  position.setX(x);
  position.setY(y);
  position.setZ(z);

  return position; 
  
}
// --------------------------------------------------------------------------- 
G4ThreeVector GEGeometryElectronics::GetJunctionSledCablesPosition()
{
  G4ThreeVector position = this->GetJunctionSledPinsPosition(); 
  G4double z =  position.getZ() + fElectronicsJunctionSledPinsHeight * 0.5  + fElectronicsJunctionSledSpacerHeight * 0.5  + fElectronicsJunctionSledCablesHeight * 0.5 + 8.6 * mm;   
  position.setZ(z);

  return position; 
  
}
// --------------------------------------------------------------------------- 
G4ThreeVector GEGeometryElectronics::GetCableChainPosition()
{
  G4ThreeVector position = this->GetJunctionSledCablesPosition(); 
  G4double z =  position.getZ() + fElectronicsJunctionSledCablesHeight * 0.5 + fElectronicsCableChainHeight* 0.5 + fElectronicsCableChainDistance + 7.61 *mm;   
  position.setZ(z);

  return position; 
  
}
// --------------------------------------------------------------------------- 
G4ThreeVector GEGeometryElectronics::GetChainPosition()
{
  G4ThreeVector position = this->GetJunctionSledCablesPosition(); 
  G4double z =  position.getZ() + fElectronicsJunctionSledCablesHeight * 0.5 + fElectronicsChainHeight* 0.5 + 7.61 *mm;   
  G4double y =  position.getY() - 35.5/2 * mm;   

  position.setY(y);
  position.setZ(z);

  return position; 
  
}
//--------------------------------------------------------------------------- 

G4ThreeVector GEGeometryElectronics::GetCablePosition()
{
  
  G4ThreeVector position = this->GetJunctionSledCablesPosition(); 
  G4double z =  position.getZ() + fElectronicsJunctionSledCablesHeight * 0.5 + fElectronicsCableHeight* 0.5 + 7.61 *mm; 
  
  position.setZ(z); 
  return position; 
  
}

// --------------------------------------------------------------------------- 
G4ThreeVector GEGeometryElectronics::GetCableConnectionPosition()
{
 
  G4ThreeVector position = fCrystalPosition; 
  G4double      z = 0.0 * mm; 
  z = GetBoardPosition().getZ() -  fKaptonCableLength * 0.5; 

  position.setZ(z); 

  return position; 

} 

// --------------------------------------------------------------------------- 

void GEGeometryElectronics::ConstructElectronics()
{
   if(fCrystalGeometry == 1 || fCrystalGeometry == 4 || fCrystalGeometry== 2){
  // volume: electronics boards and boxes 

  char volumename[100]; 
  

  sprintf(volumename, "Electronics_JunctionBoard_%d", fColumnNumber);
  
  G4ThreeVector  position = this -> GetBoardPosition();

  fElectronicsJunctionBoardPhysical = new G4PVPlacement(0,
							position, 
							fElectronicsJunctionBoardLogical, 
							volumename, 
							fDetectorMotherVolume, 
							false, 
							fColumnNumber); 


      
  sprintf(volumename, "Electronics_Box_%d", fColumnNumber); 
  
  position = this -> GetBoxPosition(); 
  
  fElectronicsBoxPhysical = new G4PVPlacement(0,
					      position, 
					      fElectronicsBoxLogical, 
					      volumename, 
					      fDetectorMotherVolume, 
					      false, 
					      fColumnNumber); 

  MGLog(debugging) << "Set the variable \"ElectronicsSideBarGeom12Logical\".  fStringStuff=="<< fStringStuff <<endlog;

  sprintf(volumename, "Electronics_LeftBar_%d", fColumnNumber); 

  position = this -> GetLeftBarPosition(); 

  MGLog(debugging) << "Start to create fElectronicsLeftBarPhysical" <<endlog;
  fElectronicsLeftBarPhysical = new G4PVPlacement(0,
						  position, 
						  fElectronicsSideBarLogical, 
						  volumename, 
						  fDetectorMotherVolume, 
						  false, 
						  fColumnNumber); 

  sprintf(volumename, "Electronics_RightBar_%d", fColumnNumber); 
  
  position = this -> GetRightBarPosition(); 
  
  MGLog(debugging) << "Start to create fElectronicsRightBarPhysical" <<endlog;
  fElectronicsRightBarPhysical = new G4PVPlacement(0,
						   position, 
                                                   fElectronicsSideBarLogical, 
						   volumename, 
						   fDetectorMotherVolume, 
						   false, 
						   fColumnNumber); 


  if(fCrystalGeometry == 1 || fCrystalGeometry == 4) {
      MGLog(debugging) << "Start to create fElectronicsPhaseIHorizontalBarPhysical" <<endlog;
      sprintf(volumename, "ElectronicsPhaseIHorizontalBar_%d", fColumnNumber); 
      fElectronicsPhaseIHorizontalBarPhysical = new G4PVPlacement( 0  /* G4Transform3D(rotation005, position)  */ ,
                                                                   GetPosition_ElectronicsPhaseIHorizontalBar(),
                                                                   fElectronicsPhaseIHorizontalBarLogical,
                                                                   volumename, 
                                                                   fDetectorMotherVolume, 
                                                                   false, 
                                                                   fColumnGeom1Number); 
  }


  G4RotationMatrix rotation005;
  rotation005.rotateZ(0 * deg);

  MGLog(debugging) << "Start to create fElectronicsJunctionSledPhysical" <<endlog;
  sprintf(volumename, "ElectronicsKaptonPreAmpsPinsCable_%d", fColumnNumber);
  
  position = this -> GetKaptonPreAmpsPinsCablePosition();
  
  fElectronicsJunctionSledPhysical = new G4PVPlacement(G4Transform3D(rotation005, position), 
						       fElectronicsKaptonPreAmpsPinsCableLogical, 
						       volumename, 
						       fDetectorMotherVolume, 
						       false, 
						       fColumnNumber);
  
   
  MGLog(debugging) << "Start to create fElectronicsJunctionSledPhysical" <<endlog;
  sprintf(volumename, "ElectronicsJunctionSled_%d", fColumnNumber);
  
  position = this -> GetJunctionSledPosition();
  
  fElectronicsJunctionSledPhysical = new G4PVPlacement(G4Transform3D(rotation005, position), 
						       fElectronicsJunctionSledLogical, 
						       volumename, 
						       fDetectorMotherVolume, 
						       false, 
						       fColumnNumber); 

  sprintf(volumename, "ElectronicsJunctionSledPins_%d", fColumnNumber);
  
  position = this -> GetJunctionSledPinsPosition();
  
  fElectronicsJunctionSledPinsPhysical = new G4PVPlacement(G4Transform3D(rotation005, position), 
							   fElectronicsJunctionSledPinsLogical, 
							   volumename, 
							   fDetectorMotherVolume, 
							   false, 
							   fColumnNumber); 


  sprintf(volumename, "ElectronicsJunctionSledSpacer_%d", fColumnNumber);
  
  position = this -> GetJunctionSledSpacerPosition();
  
  fElectronicsJunctionSledSpacerPhysical = new G4PVPlacement(G4Transform3D(rotation005, position), 
							     fElectronicsJunctionSledSpacerLogical, 
							     volumename, 
							     fDetectorMotherVolume, 
							     false, 
							     fColumnNumber); 

  sprintf(volumename, "ElectronicsJunctionSledCables_%d", fColumnNumber);
  
  position = this -> GetJunctionSledCablesPosition();
  
  fElectronicsJunctionSledCablesPhysical = new G4PVPlacement(G4Transform3D(rotation005, position), 
							     fElectronicsJunctionSledCablesLogical, 
							     volumename, 
							     fDetectorMotherVolume, 
							     false, 
							     fColumnNumber); 


  sprintf(volumename, "ElectronicsCableChain_%d", fColumnNumber);
  
  position = this -> GetCableChainPosition();
  
  fElectronicsCableChainPhysical = new G4PVPlacement(G4Transform3D(rotation005, position), 
						     fElectronicsCableChainLogical, 
						     volumename, 
						     fDetectorMotherVolume, 
						     false, 
						     fColumnNumber); 

  sprintf(volumename, "ElectronicsChain_%d", fColumnNumber);
  
  position = this -> GetChainPosition();
  
  fElectronicsChainPhysical = new G4PVPlacement(G4Transform3D(rotation005, position), 
						fElectronicsChainLogical, 
						volumename, 
						fDetectorMotherVolume, 
						false, 
						fColumnNumber); 


  MGLog(debugging) << "Start to create fElectronicsCablePhysical (if geometry-type == 2)" <<endlog;

  if (fCrystalGeometry == 2) 
    {
      sprintf(volumename, "ElectronicsCable_geometry2_%d", fColumnGeom2Number);
      
      position = this -> GetCablePosition();
      
      fElectronicsCablePhysical = new G4PVPlacement(G4Transform3D(rotation005, position), 
						    fElectronicsCableLogical, 
						    volumename, 
						    fDetectorMotherVolume, 
						    false, 
						    fColumnGeom2Number); 
      
      
      position = this -> GetCableConnectionPosition();
      
      sprintf(volumename, "CableConnection_geometry2_%d", fColumnGeom2Number); 

      G4RotationMatrix* rotConnectionCable = new G4RotationMatrix();
      rotConnectionCable->rotateZ(-fSegmentStartingAngleDataBase);
      
      fKaptonConnectionCablePhysical = new G4PVPlacement(rotConnectionCable,
							 position, 
							 fKaptonConnectionCableLogical, 
							 volumename,
							 fDetectorMotherVolume, 
							 false, 
							 0);
      
      
      
      position = this -> GetCableConnectionPosition();
      
      sprintf(volumename, "CableConnectionCopperLines_geometry2_%d", fColumnGeom2Number); 
      fCopperLinesConnectionPhysical = new G4PVPlacement(rotConnectionCable,
							 position, 
							 fCopperLinesConnectionLogical, 
							 volumename,
							 fDetectorMotherVolume, 
							 false, 
							 0);
      
      
    }
 } 
 else if (fCrystalGeometry == 5 ||fCrystalGeometry == 11){
 MGLog(trace) << "Phase2 electronics is constructed at the moment via GEGeometryLArInstHybrid.cc. "<<endlog; 
 MGLog(trace) << "It is only constructed if the flag for the veto is set. "<<endlog;
 }
 
  
} 

// --------------------------------------------------------------------------- 
 
