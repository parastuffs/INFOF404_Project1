/**
 * This file is intended to generate a random set of tasks.
 * They are to be periodic, the system is synchronous and
 * with constrained deadlines.
 * It recieve two parameters: the utilization and the number
 * of tasks.
 */

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <fstream>

#include "Generator.hpp"

using namespace std;


int main(int argc, char** argv)
{

	srand(time(NULL));

	int util,tasksNumber;
	char* outFile;

	//
	//Parsing the arguments
	//
	for(int i=1;i<argc-1;i+=2) {
		if((string)argv[i] == "-u") {
			util = atoi(((string)argv[i+1]).c_str());
		}
		else if((string)argv[i] == "-n") {
			tasksNumber = atoi(((string)argv[i+1]).c_str());
		}
		else if((string)argv[i] == "-o") {
			outFile = argv[i+1];
		}
		else {
			cout << "Problem with the arguments" << endl;
			exit(0);
		}
	}
	int tasks[tasksNumber][4];
	Generator* gen = new Generator(util,tasksNumber);
	gen->generateTasks(tasks);
	gen->tasksToFile(outFile,tasks);
	return 0;
}

