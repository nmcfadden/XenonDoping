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

class Tokenprocessor {
public:
	Tokenprocessor();
	Tokenprocessor(string filename);
	virtual ~Tokenprocessor();
	void ReadTokenFile(string filename);
	string GetTokenValue(string token);
private:
	std::map<int, string*> tokenmap;
	int tokenindex;
};

#endif /* TOKENPROCESSOR_H_ */
