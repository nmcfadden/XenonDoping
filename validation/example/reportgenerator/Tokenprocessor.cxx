#include "Tokenprocessor.h"
#include <fstream>
#include <iostream>
#include <utility>

using namespace std;

Tokenprocessor::Tokenprocessor() {

	tokenindex = 0;

}

Tokenprocessor::Tokenprocessor(string filename) {

	tokenindex = 0;

}

Tokenprocessor::~Tokenprocessor() {

}

void Tokenprocessor::ReadTokenFile(string filename) {

	ifstream inputfile;
	string s;
	string* ps;
	size_t endposition;

	inputfile.open(filename.c_str());
	if (inputfile.fail()) {
		cout << "(Tokenprocessor:) ERROR! Can't open file \"" << filename
				<< "\"\n";
	}

	while (getline(inputfile, s)) {

		ps = new string[2];

		endposition = s.find(":XX", 0);
		ps[0] = s.substr(0, endposition + 3);
		ps[1] = s.substr(endposition + 3);
		if (ps[1][0] == '\t')
			ps[1].erase(0, 1);

		tokenmap.insert(pair<int, string*> (tokenindex, ps));
		tokenindex++;
	}
	inputfile.close();

	/*/ Debug Info
	map<int, string*>::iterator imap;
	imap = tokenmap.begin();

	while (imap != tokenmap.end()) {
		ps = imap->second;
		cout << ps[0] << "\n";
		imap++;
	}
	//*/
}

string Tokenprocessor::GetTokenValue(string token) {
	string* ps;

	map<int, string*>::iterator imap;
	imap = tokenmap.begin();

	while (imap != tokenmap.end()) {
		ps = imap->second;
		if (token == ps[0])
			break;
		imap++;
	}

	return ps[1];
}

