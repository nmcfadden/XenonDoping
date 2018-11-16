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
 * $Id: MJOutputShieldingData.hh,v 1.1 2005-12-28 04:48:58 kareem Exp $
 *      
 * CLASS DECLARATION:  MJOutputShieldingData.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 * Definition of ROOT tree for the shielding output. This class is in no way
 * detector-specific, so the output class itself is very general.
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
 * AUTHOR: kareem
 * CONTACT: 
 * FIRST SUBMISSION: Tue Dec 27 14:38:48 PST 2005
 * 
 * REVISION:
 * 
 * 12-27-2005, Initial submission, Kareem
 */
// --------------------------------------------------------------------------//

#ifndef _MJOUTPUTSHIELDINGDATA_HH
#define _MJOUTPUTSHIELDINGDATA_HH

//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//

class MJOutputShieldingData 
{
public:

  //default constructor
  MJOutputShieldingData() {;}

  //destructor
  virtual ~MJOutputShieldingData() {;}

  //public interface
  Char_t	Particle[20];
  Int_t		EventID;
  Double_t  Energy;

  //protected members
protected:

  //private  members
private:

};
#endif
