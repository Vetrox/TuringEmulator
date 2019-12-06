// TuringEmulator.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

typedef unsigned char uch;
typedef long long int lli;
using namespace std;

uch start;
uch ende;


struct Transition {
	uch zustand;
	uch element;

	uch zustand_neu;
	uch push_element;
	uch instruction;
};

uch parsec(char c) {
	if (c == '_') return 0;
	else if (c == '1') return 1;
	else if (c == '5') return 2;
}

char parseback(uch c) {
	if (c == 0) return '_';
	else if (c == 1) return '1';
	else if (c == 2) return '5';
}

void daten_einlesen(vector<Transition>& transitions, vector<uch>& band, int argc, char** argv) {
	vector<string> data;
	string line;
	string path = "";
	if (argc >= 2) {
		path = string(argv[1]);
		cout << "Dateiname aus Argumenten: " << path << endl;
	}
	else {
		cout << "Dateinamen eingeben: ";
		cin >> path;
	}
	
	
	
	ifstream myfile(path);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			data.push_back(line);
		}
		myfile.close();
	}
	start = data[0][6];
	ende = data[1][5];
	string b = data[2].substr(5);
	for (char c : b) {
		band.push_back(parseC(c));
	}

	for (uch i = 3; i < data.size(); i++) {
		string d = data[i];
		Transition t;
		t.zustand = d[0];
		t.element = parseC(d[2]);
		t.zustand_neu = d[7];
		t.push_element = parseC(d[9]);
		t.instruction = d[11];
		transitions.push_back(t);
	}
}

lli read(vector<uch>& band, const lli& head, lli& vectorMid) {
	if (head + vectorMid < 0) {
		band.insert(band.begin(), parseC('_'));
		vectorMid++;
	}
	else if (head + vectorMid >= band.size()) {
		band.push_back(parseC('_'));
	}
	return vectorMid + head;
}

void parseInstruction(const uch& instruction, lli& head) {
	if (instruction == '>') head++;
	else if (instruction == '<') head--;
}

int main(int argc, char** argv) {
	//the setup
	vector<uch> band;
	vector<Transition> transitions;
	daten_einlesen(transitions, band, argc, argv);

	cout << "Bedingungen" << endl;
	cout << "Start = " << start << " Ende = " << ende << endl;
	for (Transition t : transitions)
		cout << t.zustand << " " << parseBack(t.element) << " " << t.zustand_neu << " " << parseBack(t.push_element) << " " << t.instruction << endl;


	cout << "Band: ";
	for (uch c : band)
		cout << parseBack(c) << " ";
	cout << endl;


	//setting up the turing computation
	lli head = 0;
	lli vectorMid = 0;
	lli counter = 0;
	uch zustand = start;
	//time before calculation
	chrono::steady_clock::time_point begin = chrono::steady_clock::now();
	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	//the turing simulation
	while (zustand != ende) {
		uch& container = band[read(band, head, vectorMid)];
		for (Transition trans : transitions) {
			if (trans.zustand == zustand and trans.element == container) {
				zustand = trans.zustand_neu;
				container = trans.push_element;
				parseInstruction(trans.instruction, head);
				break;
			}
		}
		counter++;

		if (chrono::duration_cast<chrono::seconds> (chrono::steady_clock::now() - end).count() > 10) {
			cout << "Operationen: " << counter << endl;
			end = chrono::steady_clock::now();
		}
	}
	end = chrono::steady_clock::now();
	lli elapsed_secs = chrono::duration_cast<chrono::seconds> (end - begin).count();

	lli anz_zeichen = 0;
	for (uch c : band)
		if (c != 0) anz_zeichen++;


	cout << "Halt-Status nach " << elapsed_secs << "s erreicht." << counter << " Operationen, " << anz_zeichen << " Zeichen auf dem Band." << endl;
	for (uch c : band) {
		cout << parseBack(c) << " ";
	}
	cout << endl;


	system("pause");
}

