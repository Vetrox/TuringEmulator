#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <unordered_map>
#include <tuple>


typedef unsigned char uch;
typedef long long int lli;
using namespace std;

uch start, ende;
vector<uch> band;


void daten_einlesen(unordered_map<uch, unordered_map<uch, tuple<uch, uch, uch>>>& transitions, vector<uch>& band, int argc, char** argv) {
	string line, path;
	if (argc >= 2) {
		path = string(argv[1]);
		cout << "Dateiname aus Argumenten: " << path << endl;
	} else {
		cout << "Dateinamen eingeben: ";
		cin >> path;
	}

	vector<string> data;
	ifstream myfile(path);
	if (myfile.is_open()) {
		while (getline(myfile, line))
			data.push_back(line);
		myfile.close();
	}

	start = data[0][6];
	ende = data[1][5];
	string b = data[2].substr(5);
	for (char c : b)
		band.push_back(c);

	for (uch i = 3; i < data.size(); i++)
		transitions[data[i][0]][data[i][2]] = tuple<uch, uch, uch>(data[i][7], data[i][9], data[i][11]);
}

uch& read(vector<uch>& band, const lli& head, lli& vectorMid, uch zustand) {
	if (head + vectorMid < 0) {
		band.insert(band.begin(), '_');
		vectorMid++;
	} else if (head + vectorMid >= band.size()) 
		band.push_back('_');
	return band[vectorMid + head];
}

void parseInstruction(const uch& instruction, lli& head) {
	if (instruction == '>') head++;
	else if (instruction == '<') head--;
}

int main(int argc, char** argv) {
	//the setup
	unordered_map<uch, unordered_map<uch, tuple<uch, uch, uch>>> transitions;
	daten_einlesen(transitions, band, argc, argv);

	cout << "Bedingungen" << endl;
	cout << "Start = " << start << " Ende = " << ende << endl;

	//setting up the turing computation
	lli head = 0, vectorMid = 0, counter = 0;
	uch zustand = start;

	//time before calculation
	auto begin = chrono::steady_clock::now();

	//the turing simulation
	while (zustand != ende) {
		uch& container = read(band, head, vectorMid, zustand);
		auto c = transitions[zustand][container];
		zustand = get<0>(c);
		container = get<1>(c);
		parseInstruction(get<2>(c), head);
		counter++;
	}
	auto end = chrono::steady_clock::now();


	//berechnungen machen
	lli anz_zeichen = 0;
	for (uch c : band)
		if (c != '_') anz_zeichen++;
	lli elapsed_ms = chrono::duration_cast<chrono::milliseconds> (end - begin).count();


	//ausgabe an den user
	cout << "Halt-Status nach " << elapsed_ms << "ms erreicht." << counter << " Operationen, " << anz_zeichen << " Zeichen auf dem Band." << endl;
	for (uch c : band) {
		cout << c << " ";
	}
	cout << endl;

	system("pause");
}

