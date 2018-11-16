/*
 * LaTeXprocessor.h
 *
 *  Created on: Sep 21, 2010
 *      Author: fraenkle
 */

#ifndef LATEXPROCESSOR_H_
#define LATEXPROCESSOR_H_

#include <string>
using std::string;

class LaTeXprocessor {
public:
	LaTeXprocessor();
	LaTeXprocessor(string filename);
	int ReadTemplate(string filename);
	void WriteReport(string filename);
	int GetTokenNumber();
	string GetToken();
	void ReplaceToken(string textvalue);
	void PrintLatexString();
	virtual ~LaTeXprocessor();
private:
	string latexstring;
	int tokenposition;
};

#endif /* LATEXPROCESSOR_H_ */
