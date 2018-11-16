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
 * DESCRIPTION:
 * 	This module includes misc. I/O functions.
 * 	Based on signal_calc_utils.h by Karin Lagergren
 * 
 * AUTHOR:
 * 	Khang Nguyen & Jason Detwiler
 *
 * CONTACT:
 * 	khangdnguyen@lbl.gov, jadetwiler@lbl.gov
 *
 * FIRST SUBMISSION:
 *
 * REVISION: MM-DD-YYYY
 *
 */

#ifndef _MGSiggenUtils_HH
#define _MGSiggenUtils_HH

#include <fstream>
#include <string.h>

#include "MGCrystalData.hh"


class MGSiggenUtils
{
  public:
    /* ReadSetupLine()
     * Read one line from config file as a string.
     * All comments beginning with a # (pound sign/hash) are cleared.
     * Empty lines are skipped. Whitespaces at the beginning and end 
     * of each line are removed. Returns the string. 
     */
    static std::string ReadSetupLine(std::ifstream &file);

    /* InitializePPCGeometry()
     * Reads information about PPC detector geometry from file given by 
     * geometryFileName. Returns the object MGCrystalData::crystalData,
     * which contains information about crystal dimensions such as
     * crystal height, well depth, inner radius, outer radius, etc. 
     */
    static MGCrystalData* InitializeNewPPCGeometry( const char* geometryFileName, int crystalID,
                                                    double globalX = 0.0, double globalY = 0.0, double globalZ = 0.0, 
                                                    double wrapAroundDepth= 0.0 );

    /* InitializeSymmetricGeometry()
     * Reads information about symmetric detector geometry from file given
     * by geometryFileName. Returns the object MGCrystalData::crystalData,
     * which contains information about crystal dimensions such as
     * crystal height, well depth, inner radius, outer radius, etc. 
     */
    static MGCrystalData* InitializeNewSymmetricGeometry( const char* geometryFileName, int crystalID,
                                                          MGCrystalData::EDiodeType diodeType, MGCrystalData::EGeometryType geomType,
                                                          double globalX = 0.0, double globalY = 0.0, double globalZ = 0.0,
                                                          double wrapAroundDepth = 0.0);

};

#endif /*#ifndef _MGSiggenUtils_HH*/
