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

using namespace std;

void generateTasks(int tasksNumber, int utilization, int tasks[][4]);
void tasksToFile(char* outFile, int tasks[][4], int tasksNumber);

int main(int argc, char** argv)
{

	srand(time(NULL));

	int util,tasksNumber;
	char* outFile;

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
		ncludes reference to array of unknown bound}
	}
	int tasks[tasksNumber][4];	
	generateTasks(tasksNumber,util,tasks);
	tasksToFile(outFile,tasks,tasksNumber);
	return 0;
}

void generateTasks(int tasksNumber, int util, int tasks[][4])
{
	for(int i=0;i<tasksNumber;i++) {
		tasks[i][0]=0;//Offset
		tasks[i][3]=rand()%30+1;//WCET
		tasks[i][1]=tasks[i][3]*(tasksNumber*100/util);//Period
		tasks[i][2]=tasks[i][3]+rand()%(tasks[i][1]-tasks[i][3]-1);//Deadline
	}
}

void tasksToFile(char* outFile, int tasks[][4], int tasksNumber)
{
	ofstream file;
	file.open(outFile);
	for(int i=0;i<tasksNumber;i++) {
		for(int j=0;j<4;j++) {
			file << tasks[i][j] << " ";
		}
		file << "\n";
	}
	file.close();
}
