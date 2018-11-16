//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
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
//                                                          
// $Id: MJcodeTemplate.cc,v 1.3 2005-02-15 23:26:55 akbarm Exp $ 
//      
// CLASS IMPLEMENTATION:  MGGeometryImportance.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR: alexis
 * CONTACT: Alexis Schubert (alexis3@uw.edu)
 * FIRST SUBMISSION: Mon Nov 14 12:35:25 PST 2011
 * 
 * REVISION:
 *
 * 16 Nov 2011, created from MGGeometryDetector, A. Schubert
 */
//---------------------------------------------------------------------------//
//

#include "geometry/MGGeometryImportance.hh"      
#include "io/MGLogger.hh"

using namespace std;

//---------------------------------------------------------------------------//


void MGGeometryImportance::ConstructImportanceStore(G4IStore& theStore)
{
   for(map<G4VPhysicalVolume*, pair<G4double, G4int>* >::iterator i = 
      fImpSampRegionMap.begin();i != fImpSampRegionMap.end(); i++)
   {
      MGLog(trace) << "Adding "<< (i->first)->GetName() << endlog;
      
      theStore.AddImportanceGeometryCell((i->second)->first, 
         *(i->first), (i->second)->second);
      
      MGLog(routine)<<"Added "<< (i->first)->GetName() << " (Replica Number: "
	 <<(i->second)->second<< ") with importance: "<<
         (i->second)->first <<" to the Importance Store."<<endlog;
   }	
}

//---------------------------------------------------------------------------//

void MGGeometryImportance::SetImportanceValueForRegion(
  G4VPhysicalVolume* aVol,
  G4double anImpValue, 
  G4int aRepNum
) {
  
   // sets importance value (default kMinImpValue) 
   // anImpValue: 0 -> particle will be killed
   //           : > 0 -> allowed (higher means greater importance)
   //           : < 0 -> not allowed!
   
  if (anImpValue< 0) MGLog(error) << "Importance value < 0!"<< endlog;
  else
  {
    map<G4VPhysicalVolume*, pair<G4double, G4int>* > :: const_iterator anIt;
    anIt = fImpSampRegionMap.find(aVol);
    if(anIt == fImpSampRegionMap.end())
    {
      if(anImpValue < kMinImpValue && anImpValue > 0.) anImpValue = kMinImpValue;
      pair<G4double, G4int>* aPair = 
        new pair<G4double, G4int>(anImpValue, aRepNum);
      fImpSampRegionMap.insert(
        pair<G4VPhysicalVolume*, pair<G4double, G4int>* >(aVol,aPair));
      MGLog(trace) << "Set " << aVol->GetName() << " (Replica Number: " << aRepNum
        << ") with importance value: " << anImpValue << endlog;
    }
    else 
    {
      MGLog(warning) << "Importance Value for: " << aVol->GetName() 
        << " already set!" << endlog;
    }
  } 
}

//---------------------------------------------------------------------------//


