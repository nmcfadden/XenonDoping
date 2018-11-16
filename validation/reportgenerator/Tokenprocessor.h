/*
 * Tokenprocessor.h
 *
 *  Created on: Sep 24, 2010
 *      Author: fraenkle
 */

#ifndef TOKENPROCESSOR_H_
#define TOKENPROCESSOR_H_

#include <string>
#include <map>
using std::string;

//! Token Processor
/*!
 * This class contains methods to process tokens.
 */

class Tokenprocessor {
public:
	//! Default constructor
	/*!
	 * Constructs a TokenProcessor object.
	 */
	Tokenprocessor();
	virtual ~Tokenprocessor();
	//! Read token file and stores tokens and the corresponding values in tokenmap
	/*!
	 * \param filename name of the file that will be opened
	 */
	void ReadTokenFile(string filename);
	//! Returns the value of the specified token.
	/*!
	 * \param token name of the token
	 * \return returns the value of the token as a string or "??" in case the token is not found
	 */
	string GetTokenValue(string token);
private:
	std::map<int, string*> tokenmap;
	int tokenindex;
};

#endif /* TOKENPROCESSOR_H_ */
