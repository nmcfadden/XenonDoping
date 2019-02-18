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
 * AUTHOR: akbar
 * CONTACT:
 * FIRST SUBMISSION: Fri Apr  9 11:09:35 PDT 2004
 *
 * REVISION:
 *
 * mm-dd-yyyy, What is changed, Whoami
 * 30-10-2018, Enable colored output, L. Pertoldi
 * 29-11-2018, remove char arrays, restyle the class
 *             redirect to cerr if warning, error or fatal
 *             use exit() instead of abort() for fatal messages
 *             now fatal messages are actually printed, exit() is called when the
 *             following endlog is invoked
 */
//---------------------------------------------------------------------------//

#include "io/MGLogger.hh"
#include "io/MaGeSVNInfo.hh"

#include <sstream>
#include <cstdlib>

static struct nullstream:
  std::ostream {
    struct nullbuf: std::streambuf {
      int overflow(int c) { return traits_type::not_eof(c); }
    } m_sbuf;
    nullstream(): std::ios(&m_sbuf), std::ostream(&m_sbuf) {}
  } devnull;

MGLogger::Severity MGLogger::_min_severity = MGLogger::debugging;

std::ostream* MGLogger::_outstream  (&std::cout);
std::ostream* MGLogger::_errstream  (&std::cerr);
std::ostream* MGLogger::_nullstream (&devnull);

bool MGLogger::_do_print     = true;
bool MGLogger::_is_fatal     = false;
bool MGLogger::_color_output = true;

MGLogger::MGLogger() {}
MGLogger::MGLogger(const MGLogger&) {}
MGLogger::~MGLogger() {}

std::ostream& MGLogger::msg(MGLogger::Severity severity, const std::string facility) {

  _do_print = true;

  if (severity >= _min_severity) {
    if (severity == MGLogger::warning or
        severity == MGLogger::error or
        severity == MGLogger::fatal) {

      if (_color_output) *_errstream << _to_string(severity) << ":\033[1m" << facility << "\033[0m: ";
      else               *_errstream << _to_string(severity) <<               facility;
    }
    else {
      if (_color_output) *_outstream << _to_string(severity) << ":\033[1m" << facility << "\033[0m: ";
      else               *_outstream << _to_string(severity) <<               facility;
    }


    if (severity == MGLogger::fatal) _is_fatal = true;
  }
  else {
    _do_print = false;
    return *_nullstream ;
  }

  return *_outstream;
}

// call exit() if last severity is fatal
// this allows to actually print something about the error
// obviously the program does not terminate until endlog is called...
void MGLogger::endlog(std::ostream&) {
  if(_do_print) *_outstream << std::endl;
  if(_is_fatal) {
      *_errstream << "Aborting..." << std::endl;
      exit(1);
  }
}

std::ostream& endlog(std::ostream& os){
  MGLogger::endlog(os);
  return devnull;
}

std::string MGLogger::_to_string(MGLogger::Severity sever) {
  switch (sever) {
    case -1: return _color_output ? "\033[35mDebug\033[m"     : "Debug";
    case  0: return _color_output ? "\033[34mTrace\033[m"     : "Trace";
    case  1: return _color_output ? "\033[32mRoutine\033[m"   : "Routine";
    case  2: return _color_output ? "\033[1;33mWarning\033[m" : "Warning";
    case  3: return _color_output ? "\033[1;31mError\033[m"   : "Error";
    case  4: return _color_output ? "\033[7;31mFatal\033[m"   : "Fatal";
  }
  return _color_output ? "\033[7;31mFatal\033[m" : "Fatal";
}

const std::string MGLogger::GetMaGeTag()      { return MAGESVNTAG;  }
const std::string MGLogger::GetMaGeRevision() { return MAGESVNREV;  }
const std::string MGLogger::GetMaGeDate()     { return MAGESVNDATE; }
