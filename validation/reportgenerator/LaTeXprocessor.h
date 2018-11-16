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

//! LaTeX Processor
/*!
 * This class contains methods to process a LaTeX template file and to substitute tokens with their actual value.
 */

class LaTeXprocessor {
public:
	//! Default constructor
	/*!
	 * Constructs a LaTeXProcessor object.
	 */
	LaTeXprocessor();
	//! Extended constructor
	/*!
	 * Constructs a LaTeXProcessor object and opens the LaTeX template file.
	 *\param filename filename of the LaTeX template file
	 */
	LaTeXprocessor(string filename);
	//! Read LaTeX template File
	/*!
	 * Opens the LaTeX template file and saves the content in latexstring.
	 *\param filename filename of the LaTeX template file
	 */
	void ReadTemplate(string filename);
	//! Write LaTeX string to file
	/*!
	 * Writes the content of latexstring into a file
	 *\param filename filename of the LaTeX output file
	 */
	void WriteReport(string filename);
	//! Get token number
	/*!
	 * Returns number of tokens in latexstring
	 *\return number of tokens in latexstring
	 */
	int GetTokenNumber();
	//! Get token
	/*!
	 * Returns the actual token within latexstring
	 *\return name of token
	 */
	string GetToken();
	//! Replace token with textvalue
	/*!
	 * Replaces the token within latexstring with textvalue
	 *\param textvalue replacement string for token
	 */
	void ReplaceToken(string textvalue);
	//! Print latexstring
	/*!
	 * Prints latexstring to standard output
	 */
	void PrintLatexString();
	void CheckLatexString();
	virtual ~LaTeXprocessor();
private:
	string latexstring;
	int tokenposition;
};

#endif /* LATEXPROCESSOR_H_ */
