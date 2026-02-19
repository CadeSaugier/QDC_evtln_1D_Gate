//Gate QDC .evtln Event Data Based on 1D Histogram Plot of desired data.
//Program by: Cade Saugier, INPP EAL Ohio University

/*
This C++ code is open source.
Do whatever you want with it.
I don't mind.

For support: Go to room 115 EAL or E-mail cs192623@ohio.edu
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>

#include "include/histxy.h"

using std::ifstream;
using std::ofstream;
using std::string;
using std::to_string;
using std::stoi;
using std::cout;
using std::endl;
using std::cin;


///////////////////////////////////// Build Functions ////////////////////////////////////
int get(string line, int stop)
{
	int i=0, k=0, last=0;
	while(k<stop and k!=4)
	{
		if(line[i]==',' and k!=stop-1) {k++; last=i;}
		else if(line[i]==',') {k++;}
		i++;
	}
	int adj=0;
	if(stop>1) {adj+=1;}
	return stoi(line.substr(last+adj,i-last-1-adj));
}

int find(string name)
{
	int send;
	if(name=="short") {send=1;}
	else if(name=="long") {send=2;}
	else if(name=="psd") {send=3;}
	else if(name=="tof") {send=4;}
	else if(name=="chan") {send=5;}
	return send;
}


/////////////////////////////////////// Build Main ///////////////////////////////////////
int main(int argc, char* argv[])
{
	cout << "\n<><><><><><><><><><> 1D HISTOGRAM GATE <><><><><><><><><><>\n";
	cout << "\t--->Version 1.0.0\n";
	cout << "\t--->By: Cade S.\n\n";
	
	//Record Inputs
		string fileLoc, dataName;
		fileLoc=argv[1];
		dataName=argv[2];
		int ID=find(dataName);
	
	//Get File Name
		string fileName;
		int fileMarker, lastDir=0, val=0;
		for(int i=0; i<fileLoc.length(); i++)
		{
			if(fileLoc[i]=='/') {lastDir=i; val=1;}
			if(fileLoc[i]=='.') {fileMarker=i;}
		}
		fileName=fileLoc.substr(lastDir+val,fileMarker-lastDir-val);
	
	//Display
		cout << "\n\n>>> Gating File " << fileName << " via " << dataName << endl;
	
	//Build Requested xy File
	ifstream dataFile;
	dataFile.open(fileLoc);
	ofstream dataOut;
	dataOut.open("temp.xy");
	histo dataHist;
	string read;
	for(int i=0; i<3; i++) {dataFile >> read;}
	while(!dataFile.eof())
	{
		dataHist.plug(get(read,ID));
		dataFile >> read;
	}
	dataFile.close();
	for(int i=0; i<65536; i++)
	{
		dataOut << to_string(i) << "  " << to_string(dataHist.get(i)) << "\n";
	}
	dataOut.close();
	
	//Plot and Ask for Domain Values
	system("python3 ./include/plotonly1d.py ./temp.xy &");
	std::this_thread::sleep_for(std::chrono::seconds(4));
	int a, b;
	cout << "---> Please Select Lower and Upper Domain Values" << endl;
	cout << "Low Value: "; cin >> a;
	while(a<0 or a>65535)
	{
		cout << "Invalid Value for Low! Please Enter Another Value...";
		cout << "Low Value: "; cin >> a;
	}
	cout << "High Value: "; cin >> b;
	while(b<a or b<0 or b>65535)
	{
		cout << "Invalid Value for High! Please Enter Another Value...";
		cout << "High Value: "; cin >> b;
	}
	
	//Make Gate Plot on Original Data
	system(("python3 ./include/plotgate1d.py ./temp.xy "+to_string(a)+" "+to_string(b)).c_str());
	
	//Make Output Files and Start Gate Loop
	dataFile.open(fileLoc);
	for(int i=0; i<3; i++) {dataFile >> read;}
	system("mkdir output");
	//here continue
	
	dataFile.close();
	
	//Clean Up
	system("rm temp.xy");
	
	
	
	
	
	
	
	
	return 0;
}
