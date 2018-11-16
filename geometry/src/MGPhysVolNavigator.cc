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

#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4VisExtent.hh"

#include "geometry/MGPhysVolNavigator.hh"
#include "io/MGLogger.hh"

#include <map>

using namespace std;

G4VPhysicalVolume* MGPhysVolNavigator::GetVolumeNamed(G4String volName, G4int copyNumber)
{
  G4VPhysicalVolume* volume = NULL;
  G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();

  for(size_t i=0; i<volumeStore->size(); i++) {
    if((*volumeStore)[i]->GetName() == volName) {

      // if copyNumber == -1, we haven't supplied our own copy number,
      // which means we expect there to be only one volume of this name. If
      // we find more than one, emit an error and return the first one that
      // was found.
      if(copyNumber == -1) {
        if(volume != NULL) {
          MGLog(warning) << "Found more than one volume named " << volName << endlog;
          MGLog(warning) << "You should specify a copy number for this volume." << endlog;
          MGLog(warning) << "Returning first volume found." << endlog;
          return volume;
        }
        else volume = (*volumeStore)[i];
      }

      // if copyNumber != -1, we have asked for a specific copy. Return it
      // as soon as it's found.
      else if((*volumeStore)[i]->GetCopyNo() == copyNumber) return (*volumeStore)[i];
    }
  }
  if(volume == NULL) {
    MGLog(warning) << "Did not find volume " << volName;
    if(copyNumber != -1) cout << " copy number " << copyNumber;
    cout << " in volume store. Returning NULL..." << endlog;
    return NULL;
  }
 
  // if we make it here, copyNumber == -1 and only one volume was found.
  return volume;
}


std::vector<G4VPhysicalVolume*> MGPhysVolNavigator::GetVolumesMatching(G4String volName)
{
  G4String volumeRetrieved = "";
  vector<G4VPhysicalVolume*> volumes;
  G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
  for(size_t i=0; i < volumeStore->size(); i++) {
    volumeRetrieved = (*volumeStore)[i]->GetName();
    if(volName == "all"){
      volumes.push_back((*volumeStore)[i]);
    }
    else {
      size_t found = volumeRetrieved.find(volName);
      if(found != string::npos) {
        MGLog(trace) << "Found an instance of \"" << volName << "\" in the volume " << volumeRetrieved << endlog;
        volumes.push_back((*volumeStore)[i]);
      }
    }
  } 

  MGLog(routine) << "====================================================================" << endlog;
  MGLog(routine) << "Number of matching volumes found for \"" << volName << "\": " << volumes.size() << endlog;
  MGLog(routine) << "====================================================================" << endlog;
  
  return volumes;
}


G4double MGPhysVolNavigator::GetBoundingRadius(G4VPhysicalVolume* volume)
{
  return volume->GetLogicalVolume()->GetSolid()->GetExtent().GetExtentRadius();
}

G4ThreeVector MGPhysVolNavigator::GetGlobalTranslation(G4VPhysicalVolume* volume)
{
  G4ThreeVector globalTranslation(0, 0, 0);
  G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4VPhysicalVolume* world = navigator->GetWorldVolume();
  while(volume != world && volume != NULL) {
    globalTranslation += volume->GetObjectTranslation();
    volume = MGPhysVolNavigator::FindDirectMother(volume);
  }
  if(volume == NULL) return G4ThreeVector(0,0,0);
  else return globalTranslation; 
}

G4VPhysicalVolume* MGPhysVolNavigator::FindDirectMother(G4VPhysicalVolume* volume)
{
  MGLog(debugging) << "entering FindTheDirectMother(" << volume->GetName() << ")" << endlog;

  G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
  size_t nVolumes = volumeStore->size();
  vector<G4VPhysicalVolume*> auxVect; 

  //1st step: look for all the ancestors (not only in the upper level)
  for (size_t i=0; i<nVolumes; i++) {
    if ((*volumeStore)[i]->GetLogicalVolume()->IsAncestor(volume)) {
	auxVect.push_back((*volumeStore)[i]);
	MGLog(debugging) << "Ancestor of " << volume->GetName() << ": " 
	                 << (*volumeStore)[i]->GetName() << endlog;
    }
  }
  size_t nAncestors = auxVect.size();
  if(nAncestors == 0) {
    MGLog(error) << "No ancestors found for physical volume " 
                 << volume->GetName() << ". Is " << volume->GetName() 
                 << " an unplaced template volume?" << endl;
    return NULL;
  }
  MGLog(debugging) << "Initial number of ancestors: " << nAncestors << endlog;

  //2nd step: remove the non-direct ancestors
  std::vector<G4VPhysicalVolume*>::iterator iter = auxVect.begin();
  G4bool erased = false;
  while (nAncestors > 1) {
    erased = false;
    MGLog(debugging) << "Number of ancestors: " << nAncestors << endlog;
    for (size_t i=0; i<nAncestors; i++) {
      for (size_t j=0; j<nAncestors; j++) {
        iter = auxVect.begin();
        G4VPhysicalVolume* vol1 = auxVect[i];
        G4VPhysicalVolume* vol2 = auxVect[j];
        if (vol1->GetLogicalVolume()->IsAncestor(vol2)) {
          //vol1 is ancestor of vol2
          //so vol1 is not a direct ancestor of the target volume!
          iter += i; //moves to the i-th element of the vector
          auxVect.erase(iter); //removes vol1 from the list
          erased = true;
        }
        if (erased) break;
      }
      if (erased) break;
    }
    nAncestors = auxVect.size(); //updates the dimension of the list
  }
  return auxVect[0];
}

G4String MGPhysVolNavigator::GetListOfVolumes()
{
  G4String volList = "";
  G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
  for(size_t i=0; i<volumeStore->size(); i++) {
    volList += (*volumeStore)[i]->GetName();
    if(i<volumeStore->size()-1) volList += " ";
  }
  return volList;
}

void MGPhysVolNavigator::PrintListOfPhysicalVolumes()
{
  vector<G4String> list;
  G4String physicalVolumeName = "";
  G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
  for(size_t i = 0; i < volumeStore->size(); i++) {
    physicalVolumeName = (*volumeStore)[i]->GetName();
    list.push_back(physicalVolumeName);
  }
  sort(list.begin(), list.end());

  MGLog(routine) << "=============================================================================" << endlog;
  MGLog(routine) << "Names of physical volumes found (truncated if longer than 100 characters)... " << endlog;
  MGLog(routine) << "=============================================================================" << endlog;
  MGLog(routine) << "                             PHYSICAL VOLUME NAME                            " << endlog;
  MGLog(routine) << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endlog;
  int number = 1;
  char printout[150];
  for( size_t k = 0; k < list.size(); k++ ){
    sprintf(printout, "%i. %-100.100s", number, (list[k]).c_str());
    MGLog(routine) << printout << endlog;
    number++;
  }
  MGLog(routine) << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endlog;
  MGLog(routine) << "Total number of physical volumes: " << (int)list.size() << endlog;

}
