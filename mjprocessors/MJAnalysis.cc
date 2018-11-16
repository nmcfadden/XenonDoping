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
 * $Id: MJAnalysis.cc,v 1.3 2006-03-27 21:31:27 mgmarino Exp $
 *      
 * CLASS DECLARATION:  MGProcessorMCReadTree.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Sample analysis program for Majorana.
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
 * AUTHOR: R.Henning
 * CONTACT: 
 * FIRST SUBMISSION: Wed Jan  4 15:01:33 PST 2006
 * 
 * REVISION:
 * 
 * 01-04-2006, Created, R. Henning
 *
 */
// --------------------------------------------------------------------------//

#include <string>

#include "io/MGLogger.hh"
#include "mjprocessors/MGAnalysisMCSegmentation.hh"

// --------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
  MGLog(routine) << "Going..." << endlog;
  if(argc == 1) {
    MGLog(error) << "No file specified!" << endlog;
  } else {
    MGAnalysisMCSegmentation *analysis = new MGAnalysisMCSegmentation();
    for(int i = 1; i < argc; i++) {
      analysis->AddMCFile(argv[i]);
      MGLog(routine) << "Adding file : " << argv[i] << endlog;
    }
    string outFileName = argv[1];
    //string outFileNameBase = "/auto/majorana1/MJ/data/ProposedGeometries/ReferenceDesign/ActiveRegions/";
    //string newOutFileNameBase = "/auto/majorana1/users/mgmarino/ReycoPropStuff/";
    outFileName = outFileName.substr(0, outFileName.find(".root")) + "_seg.root";
    //outFileName = outFileName.replace(outFileName.find(outFileNameBase),  outFileNameBase.length(), newOutFileNameBase); 
    // these already exist so I changed the name to _seg1 ...prob not necessary
    analysis->SetOutputFileName(outFileName);
    analysis->RunAllProcessors();
    MGLog(routine) << "Done..." << endlog;
    delete analysis;
  }
}

// --------------------------------------------------------------------------//
// --------------------------------------------------------------------------//
