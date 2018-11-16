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
//                                                          
// $Id: MGLogger.cc,v 1.6 2008-08-19 23:06:23 mgmarino Exp $ 
//      
// CLASS IMPLEMENTATION:  MGLogger.cc
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
 * AUTHOR: akbar
 * CONTACT: 
 * FIRST SUBMISSION: Fri Apr  9 11:09:35 PDT 2004
 * 
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 */
//---------------------------------------------------------------------------//
//


//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"      //Present MG Class Headers 
#include "io/MaGeSVNInfo.hh"

//---------------------------------------------------------------------------//


#include <sstream>
#include <cstdlib>

using namespace std;

//static ostringstream devnull;
static struct nullstream:
    std::ostream {
    struct nullbuf: std::streambuf {
      int overflow(int c) { return traits_type::not_eof(c); }
    } m_sbuf;
    nullstream(): std::ios(&m_sbuf), std::ostream(&m_sbuf) {}
  } devnull;

MGLogger::Severity MGLogger::_minSeverity = MGLogger::debugging;
ostream * MGLogger::_myOstream (&cout);
ostream * MGLogger::_myErrstream (&cerr);
ostream * MGLogger::_myNullstream (&devnull);
bool MGLogger::_doPrint = true;

MGLogger::MGLogger()
{
}
MGLogger::MGLogger(const MGLogger&)
{
}

MGLogger::~MGLogger()
{
}

ostream& 
MGLogger::msg(MGLogger::Severity severity, 
		       const char* facility, 
		       const char*)
{
  _doPrint = true;
  if(severity >= _minSeverity){
    *_myOstream << toString(severity) << ":" << facility << ":";
  }else{
    _doPrint =false;
    return *_myNullstream ;
  }

  if ( severity == fatal ){
    *_myOstream << ::endlog;
    ::abort();
  }
  return *_myOstream;
}
void 
MGLogger::endlog(ostream&){
  if(_doPrint)*_myOstream << endl;
}
const char*
MGLogger::toString(MGLogger::Severity sever){
  switch (sever) {
  case -1:
    return "Debug";
    break;
  case 0:
    return "Trace";
    break;
  case 1:
    return "Routine";
    break;
  case 2:
    return "Warning";
    break;
  case 3:
    return "Error";
    break;
  case 4:
    return "Fatal";
    break;
  }
  return "Fatal";
}

ostream& endlog(ostream& os){
  MGLogger::endlog(os);
  return devnull;
}

const char* MGLogger::GetMaGeTag() { return MAGESVNTAG; }
const char* MGLogger::GetMaGeRevision() { return MAGESVNREV; }
const char* MGLogger::GetMaGeDate() { return MAGESVNDATE; }
