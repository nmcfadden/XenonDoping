#include <iostream>
#include "Tokenprocessor.h"
#include "LaTeXprocessor.h"

using namespace std;

int main(){

	LaTeXprocessor* lxp = new LaTeXprocessor("template.tex");
	Tokenprocessor* tp = new Tokenprocessor();

	tp->ReadTokenFile("parameterlist.txt");

	//lxp->ReadTemplate("template.tex");
	while(lxp->GetTokenNumber()){
		//cout << lxp->GetToken() << "\n";
		lxp->ReplaceToken(tp->GetTokenValue(lxp->GetToken()));
	}

	//lxp->PrintLatexString();
	lxp->WriteReport("report.tex");

	delete lxp;
	delete tp;

	return 0;
}
