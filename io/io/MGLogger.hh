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
 * $Id: MGLogger.hh,v 1.3 2006-06-02 06:52:04 jasondet Exp $
 *      
 * CLASS DECLARATION:  MGLogger.hh
 *
 *---------------------------------------------------------------------------//
 *
 * DESCRIPTION: 
 *
 */ 
// Begin description of class here
/**
 *
 *
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
 * AUTHOR: akbar
 * CONTACT: 
 * FIRST SUBMISSION: Fri Apr  9 11:09:35 PDT 2004
 * 
 * REVISION:
 * 
 * 07-15-2004, Added GetSeverity(), R. Henning
 */
// --------------------------------------------------------------------------//

#ifndef _MGLOGGER_HH
#define _MGLOGGER_HH

#include <iostream>

//---------------------------------------------------------------------------//
#define ERRLINE_HACK_1(line)   #line
#define ERRLINE_HACK_2(line)   ERRLINE_HACK_1(line)

#ifdef MGLog
#undef MGLog
#endif
#define MGLog(sev) MGLogger::msg( MGLogger::sev, __FILE__ "(" ERRLINE_HACK_2(__LINE__) ")", 0 )

//---------------------------------------------------------------------------//

class MGLogger 
{
public:

  // a severity enum is defined; use only these values
  enum Severity {debugging=-1, trace=0, routine, warning, error, fatal};
   //	fatal:		The message is related to a condition preventing
  //			further execution of the program.  ErrLogger will
  //			terminate the program.  Programmers should not call
  //			abort or exit themselves.

  //	error:		A condition exists such that requested result
  //			or action can not be produced.  This is a serious

  //	warning:	The result is produced, but may not be
  //			what's desired due to an unexpected condition

  //	routine:	Nothing known to be wrong with the result;
  //			messages that are always produced in normal
  //			operation

  //	trace:		Messages about the flow of program control
  //			and which optional operations took place.
  //			(This is the default if nothing is defined)
  
  //	debugging:	Information in addition to the above

 // members
  static Severity GetSeverity() { return _minSeverity; }
  static std::ostream& msg( MGLogger::Severity severity, 
		       const char* facility, 
		       const char* code );

  static void SetSeverity(Severity sever){ _minSeverity = sever;}
  static void endlog(std::ostream& s);

  static const char* GetMaGeTag();
  static const char* GetMaGeRevision();
  static const char* GetMaGeDate();

protected:
 //default constructor
  MGLogger();

  //copy constructor
  MGLogger(const MGLogger &);

  //destructor
  ~MGLogger();

  //public interface

  //protected members
protected:


  //private  members
private:
  static const char* toString(Severity);

  static std::ostream* _myOstream;
  static std::ostream* _myErrstream;
  static std::ostream* _myNullstream;

  static Severity _minSeverity;

  static bool _doPrint;
};
//void endlog(std::ostream& s);
std::ostream& endlog(std::ostream& s);
//std::ostream& operator << (std::ostream& os, string str)

#endif
