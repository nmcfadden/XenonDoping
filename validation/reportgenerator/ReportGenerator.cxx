#include <iostream>
#include "Tokenprocessor.h"
#include "LaTeXprocessor.h"

using namespace std;

//** ReportGenerator
/**
 * do some funny stuff
 */

int main(int argc, char* argv[]) {

	if (argc < 4) {
		std::cout << "Usage: ReportGenerator <parameterfile> <latextemplate> <outputfile>\n";
		return 0;
	}

	std::string parameterfile(argv[1]);
	std::string latextemplate(argv[2]);
	std::string outputfile(argv[3]);

	std::string stringcheck = "0";
	if(argc == 5)
		stringcheck = argv[4];

	cout << parameterfile << "\t" << latextemplate << "\t" << outputfile << "\n";

	LaTeXprocessor* lxp = new LaTeXprocessor(latextemplate.c_str());
	Tokenprocessor* tp = new Tokenprocessor();

	tp->ReadTokenFile(parameterfile.c_str());

	//lxp->ReadTemplate("template.tex");
	while(lxp->GetTokenNumber()){
		//cout << lxp->GetToken() << "\n";
		lxp->ReplaceToken(tp->GetTokenValue(lxp->GetToken()));
	}

	if(stringcheck == "1")
		lxp->CheckLatexString();
	lxp->WriteReport(outputfile.c_str());

	delete lxp;
	delete tp;

	return 0;
}
