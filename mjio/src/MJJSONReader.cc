#include "mjio/MJJSONReader.hh"
#include "G4UIcommand.hh"
#include "io/MGLogger.hh"
#include <sstream>
#include <fstream>
//#include <vector>
//-------------------------------

MJJSONReader::MJJSONReader() : 
  fDelimiters("\t :\",")
{
  fCurrentLine.clear();
  char* env = std::getenv("MAGEDIR");
  std::stringstream name;
  name << env << "/MJD.json";
  const std::string tmp = name.str();
  fInputFile.open(tmp.c_str());
}

MJJSONReader::MJJSONReader(const G4String &inputFile) : 
  fDelimiters("\t :\",")
{
  fCurrentLine.clear();
  fInputFile.open(inputFile);
}

MJJSONReader::~MJJSONReader()
{
  fInputFile.seekg(0);
  fInputFile.close();
}

std::vector<G4double> MJJSONReader::ReadDetectorRs(const G4String &serialNum)
{
  std::vector<G4double> badExit;
  //MGLog(trace) << "Reading the detector R's" << endlog;
  if(fInputFile.is_open())
    {
      fInputFile.seekg(0);
      G4bool foundSerialNumFlag = false;
      G4bool isDetectorType = false;
      G4bool foundDetectorParams = false;
      std::vector<G4double> r;

      foundSerialNumFlag = SearchFromCurrentPos(serialNum);
      if(!foundSerialNumFlag)
	  return badExit;
      isDetectorType = SearchFromCurrentPos("type");
      if(isDetectorType)
	{
	  if(!(fCurrentLine.at(1)=="detector"))
	    return badExit; 
	}
      else
	return badExit;	
      
      foundDetectorParams = SearchFromCurrentPos("geometry");
      if(foundDetectorParams)
	{
	  if(!(fCurrentLine.at(1)=="["))
	    return badExit;
	  
	  G4String line;
	  while(!fInputFile.eof()) //Loop until we find a ]
	    {
	      getline(fInputFile, line);
	      fCurrentLine = Tokenize(fDelimiters, line);
	      if(fCurrentLine.at(0)=="r")
		{
		  if (fCurrentLine.size() < 2)
		    return badExit;
		  std::stringstream converter;
		  G4double tempConvert;
		  converter << fCurrentLine.at(1);
		  converter >> tempConvert;
		  r.push_back(tempConvert);
		} //end if r
	      if(fCurrentLine.at(0)=="]")
		break;
	    } //end while not eof
	  
	  if(r.size() < 1)
	    return badExit;
	
	} //end if found detector params
      else
	return badExit; //if detector parms weren't found
      
      //G4double* theRs = new G4double[r.size()];
      //for(G4int i = 0; i<r.size(); i++)
      //theRs[i] = r.at(i);
      
      return r; //congrats if you made it this far.
    
    } //end if is_open
  
  return badExit; //return NULL if file was not found
}
//-----------------------
std::vector<G4double> MJJSONReader::ReadDetectorZs(const G4String &serialNum)
{
  std::vector<G4double> badExit;
  if(fInputFile.is_open())
    {
      fInputFile.seekg(0);
      G4bool foundSerialNumFlag = false;
      G4bool isDetectorType = false;
      G4bool foundDetectorParams = false;
      std::vector<G4double> z;

      foundSerialNumFlag = SearchFromCurrentPos(serialNum);
      if(!foundSerialNumFlag)
	return badExit;
      isDetectorType = SearchFromCurrentPos("type");
      if(isDetectorType)
	{
	  if(!(fCurrentLine.at(1)=="detector"))
	    return badExit; 
	}
      else
	return badExit;	
      
      foundDetectorParams = SearchFromCurrentPos("geometry");
      if(foundDetectorParams)
	{
	  if(!(fCurrentLine.at(1)=="["))
	    return badExit;
	  
	  G4String line;
	  while(!fInputFile.eof()) //Loop until we find a ]
	    {
	      getline(fInputFile, line);
	      fCurrentLine = Tokenize(fDelimiters, line);
	      if(fCurrentLine.at(0)=="z")
		{
		  if (fCurrentLine.size() < 2)
		    return badExit;
		  std::stringstream converter;
		  G4double tempConvert;
		  converter << fCurrentLine.at(1);
		  converter >> tempConvert;
		  z.push_back(tempConvert);
		} //end if z
	      if(fCurrentLine.at(0)=="]")
		break;
	    } //end while not eof
	  
	  if(z.size() < 1)
	    return badExit;
	
	} //end if found detector params
      else
	return badExit; //if detector parms weren't found
      
      //G4double* theZs = new G4double[z.size()];
      //for(G4int i = 0; i<z.size(); i++)
      //	theZs[i] = z.at(i);
      
      return z; //congrats if you made it this far.
    
    } //end if is_open
  
  return badExit; //return NULL if file was not found
}
//-----------
std::vector<G4String> MJJSONReader::ReadCryostats(const G4String &serialNum)
{
  std::vector<G4String> badExit;
  MGLog(trace) << "Reading the cyrostats" << endlog;
  if(fInputFile.is_open())
    {
      fInputFile.seekg(0);
      std::vector<G4String> cryostatList;
      G4bool foundSerialNum = false;
      G4bool isStringType = false;
      G4bool foundParams = false;
      foundSerialNum = SearchFromCurrentPos(serialNum);
      if(!foundSerialNum)
	{
	  MGLog(trace) << "Did not find cryostat serial number: " << serialNum << "." << endlog;
	  return badExit;
	}
      isStringType = SearchFromCurrentPos("type");
      if(!(fCurrentLine.at(1)=="cryostat"))
	isStringType = false;
      if(!isStringType)
	{
	  MGLog(trace) << "Reading Cryostats, type incorrect, bad exit" << endlog;
	  return badExit;
	}
      foundParams = SearchFromCurrentPos("multistring");
      if(foundParams)
	{
	  if(!(fCurrentLine.at(1)=="["))
	    {
	      MGLog(trace) << "Parameters formatted incorrectly, bad exit" << endlog;
	      MGLog(trace) << "The line is: " << fCurrentLine.at(1) << endlog;
	      return badExit;
	    }
	  G4String line;
	  while(!fInputFile.eof())
	    {
	      getline(fInputFile, line);
	      fCurrentLine = Tokenize(fDelimiters, line);
	     
		
	      if (fCurrentLine.size() > 1)
		{
		  MGLog(trace) << "Error reading cryostat list, bad exit" << endlog;
		  return badExit;
		}
	      if(fCurrentLine.at(0)=="]")
		break;
	      cryostatList.push_back(fCurrentLine.at(0));
	    }
	  if(cryostatList.size() < 1)
	    {
	      MGLog(trace) << "Reading Cryostats, no cryostats in parameter list!" << endlog;
	      return badExit;
	    }
	}
      else
	{
	  MGLog(trace) << "Reading Cryostats, didn't find parameters, bad exit" << endlog;
	  return badExit;
	}
      MGLog(trace) << "Successfully read cryostats" << endlog;
      return cryostatList;
    }
  MGLog(error) << "Did not find the JSON file" << endlog;
  return badExit;
}
//-----------
std::vector<G4String> MJJSONReader::ReadCrystalAssemblyList(const G4String &serialNum)
{
  std::vector<G4String> badExit;
  if(fInputFile.is_open())
    {
      fInputFile.seekg(0);
      std::vector<G4String> crystalList;
      G4bool foundSerialNum = false;
      G4bool isStringType = false;
      G4bool foundDetectorParams = false;
      foundSerialNum = SearchFromCurrentPos(serialNum);
      if(!foundSerialNum)
	return badExit;
      
      isStringType = SearchFromCurrentPos("type");
      if(!(fCurrentLine.at(1)=="string"))
	isStringType = false;
      if(!isStringType)
	return badExit;
      foundDetectorParams = SearchFromCurrentPos("crystalassembly");
      if(foundDetectorParams)
	{
	  if(!(fCurrentLine.at(1)=="["))
	    return badExit;
	  
	  G4String line;
	  while(!fInputFile.eof())
	    {
	      getline(fInputFile, line);
	      fCurrentLine = Tokenize(fDelimiters, line);
	      if (fCurrentLine.size() > 1)
		return badExit;
	      if(fCurrentLine.at(0)=="]")
		break;
	      crystalList.push_back(fCurrentLine.at(0));
	    }
	  if(crystalList.size() < 1)
	    return badExit;
	}
      else
	return badExit;
      
      return crystalList;
    }
  return badExit;
}
//----------------
std::vector<G4String>MJJSONReader::ReadAssemblyStringList (const G4String &serialNum)
{
  std::vector<G4String> badExit;
  if(fInputFile.is_open())
    {
      fInputFile.seekg(0);
      std::vector<G4String> stringList;
      G4bool foundSerialNum = false;
      G4bool isMultiStrAssemType = false;
      G4bool foundParams = false;
      foundSerialNum = SearchFromCurrentPos(serialNum);
      if(!foundSerialNum)
	return badExit;
      
      isMultiStrAssemType = SearchFromCurrentPos("type");
      if(!(fCurrentLine.at(1)=="multistring"))
	isMultiStrAssemType = false;
      if(!isMultiStrAssemType)
	return badExit;
      foundParams = SearchFromCurrentPos("string");
      if(foundParams)
	{
	  if(!(fCurrentLine.at(1)=="["))
	    return badExit;
	  
	  G4String line;
	  while(!fInputFile.eof())
	    {
	      getline(fInputFile, line);
	      fCurrentLine = Tokenize(fDelimiters, line);
	      if (fCurrentLine.size() > 1)
		return badExit;
	      if(fCurrentLine.at(0)=="]")
		break;
	      stringList.push_back(fCurrentLine.at(0));
	    }
	  if(stringList.size() < 1)
	    return badExit;
	}
      else
	return badExit;
      
      return stringList;
    }
  return badExit;
}
//-------
std::vector<G4String> MJJSONReader::ReadCrystalAssembly(const G4String &serialNum)
{
  std::vector<G4String> badExit;
  if(fInputFile.is_open())
    {
      fInputFile.seekg(0);
      std::vector<G4String> crystalAssembly;
      G4bool foundSerialNum = false;
      G4bool isCrysAssem = false;
      G4bool foundParams = false;
      foundSerialNum = SearchFromCurrentPos(serialNum);
      if(!foundSerialNum)
	return badExit;
      
      isCrysAssem = SearchFromCurrentPos("type");
      if(!(fCurrentLine.at(1)=="crystalassembly"))
	isCrysAssem = false;
      if(!isCrysAssem)
	return badExit;
      foundParams = SearchFromCurrentPos("parameters");
      if(foundParams)
	{
	  if(!(fCurrentLine.at(1)=="{"))
	    return badExit;
	  
	  G4String line;
	  while(!fInputFile.eof())
	    {
	      getline(fInputFile, line);
	      fCurrentLine = Tokenize(fDelimiters, line);
	      if(fCurrentLine.at(0)=="detector")
		{
		  if (fCurrentLine.size() < 2)
		    return badExit;
		  crystalAssembly.push_back(fCurrentLine.at(1));
		}
	      if(fCurrentLine.at(0)=="}")
		break;
	    }
	  if(crystalAssembly.size() < 1)
	    return badExit;
	}
      else
	return badExit;
      
      return crystalAssembly;
    }
  return badExit;
}
//-----------
std::vector<G4String> MJJSONReader::ReadCustom(const G4String &serialNum, const G4String &type, const G4String &param)
{
   std::vector<G4String> badExit;
  if(fInputFile.is_open())
    {
      fInputFile.seekg(0);
      std::vector<G4String> customList;
      G4bool foundSerialNum = false;
      G4bool isStringType = false;
      G4bool foundParams = false;
      foundSerialNum = SearchFromCurrentPos(serialNum);
      if(!foundSerialNum)
	return badExit;
      
      isStringType = SearchFromCurrentPos("type");
      if(!(fCurrentLine.at(1) == type))
	isStringType = false;
      if(!isStringType)
	return badExit;
      foundParams = SearchFromCurrentPos("parameters");
      if(foundParams)
	{
	  if(!(fCurrentLine.at(1)=="{"))
	    return badExit;
	  
	  G4String line;
	  while(!fInputFile.eof())
	    {
	      getline(fInputFile, line);
	      fCurrentLine = Tokenize(fDelimiters, line);
	      if(fCurrentLine.at(0) == param)
		{
		  if (fCurrentLine.size() < 2)
		    return badExit;
		  customList.push_back(fCurrentLine.at(1));
		}
	      if(fCurrentLine.at(0)=="}")
		break;
	    }
	  if(customList.size() < 1)
	    return badExit;
	}
      else
	return badExit;
      
      return customList;
    }
  return badExit;
}
//------------FUNCTIONS USED BY THE CLASS ITSELF-------------------
G4bool MJJSONReader::SearchFromCurrentPos(G4String parameter) //WARNING: This changes location in the ifstream
{
  G4bool foundFlag = false;
  G4String line;
  fCurrentLine.clear();
  while(!fInputFile.eof())
    {
      if(foundFlag)
	break;
      std::getline(fInputFile, line);
      //std::cout << "This is the line: " << line << std::endl;
      fCurrentLine = Tokenize(fDelimiters, line);
      if(fCurrentLine.size() > 1)
	if(fCurrentLine.at(0) == parameter)
	  foundFlag = true;
      
      //std::cout << std::endl;
    }
  return foundFlag;
}
//----------
std::vector<G4String> MJJSONReader::Tokenize(G4String separators, G4String input)
{
  // Takes a line from an ASCII file, and converts it into a vector of strings.
  // The symbols that denote breaks in the line are provided in <separators>.

  size_t startToken = 0, endToken; // Pointers to the token pos
  std::vector<G4String> tokens;       // Vector to keep the tokens
  size_t commentPos = input.size()+1;

  if( separators.size() > 0 && input.size() > 0 ) 
  {
    // Check for comment
    for (size_t i=0;i<input.size();i++)
    {
      if (input[i]=='#' || (i<input.size()-1&&(input[i]=='/'&&input[i+1]=='/')))
      {
	commentPos=i;
	break;
      }
    }

    while( startToken < input.size() )
    {
      // Find the start of token
      startToken = input.find_first_not_of( separators, startToken );

      // Stop parsing when comment symbol is reached
      if (startToken == commentPos)
      {
	if (tokens.size()==0)
	  tokens.push_back("#");
	return tokens;
      }

      // If found...
      if( startToken != (size_t)G4String::npos)
      {
	// Find end of token
	endToken = input.find_first_of( separators, startToken );

	if( endToken == (size_t)G4String::npos )
	  // If there was no end of token, assign it to the end of string
	  endToken = input.size();

	// Extract token
	tokens.push_back( input.substr( startToken, endToken - startToken ) );

	// Update startToken
	startToken = endToken;
      }
    }
  }

  return tokens;
}
