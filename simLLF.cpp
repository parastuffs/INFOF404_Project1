#include <iostream>
#include <string>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

#include "Simulator.hpp"


using namespace std;

int countTasks(char* inputFile);
void tasksFromFile(char* inputFile,int tasksParam[][4]);

int main(int argc, char** argv)
{
	char* inputFile = argv[2];
	int delta = atoi(((string)argv[1]).c_str());
	int n = countTasks(inputFile);	
	int tasksParam[n][4];
	tasksFromFile(inputFile,tasksParam);

	new Simulator(n,tasksParam,delta);	
	return 0;
}

int countTasks(char* inputFile)
{
	int n=0;
	ifstream file(inputFile);
	string line;
	if(file.is_open()) {
		while(getline(file,line)) {
			n++;
		}
		file.close();
	}
	return n;
}

void tasksFromFile(char* inputFile, int tasksParam[][4])
{
	string line;
	ifstream file(inputFile);
	int j=0;
	if(file.is_open()) {
		while(getline(file,line)) {
			string s;
			stringstream stream(line);
			int i=0;
			while(getline(stream,s,' ')) {
				tasksParam[j][i] = atoi(((string)s).c_str());
				i++;
			}
			j++;
		}
		file.close();
	}
}
