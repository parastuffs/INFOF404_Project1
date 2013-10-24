#include "Generator.hpp"

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <fstream>

using namespace std;

Generator::Generator(int u, int tN)
{
	this->util = u;
	this->tasksNumber = tN;

}	

void Generator::generateTasks(int tasks[][4])
{
	for(int i=0;i<this->tasksNumber;i++) {
		tasks[i][0]=0;//Offset
		tasks[i][3]=rand()%30+1;//WCET
		tasks[i][1]=tasks[i][3]*(this->tasksNumber*100/util);//Period
		tasks[i][2]=tasks[i][3]+rand()%(tasks[i][1]-tasks[i][3]-1);//Deadline
	}
}

void Generator::tasksToFile(char* outFile, int tasks[][4])
{
	ofstream file;
	file.open(outFile);
	for(int i=0;i<this->tasksNumber;i++) {
		for(int j=0;j<4;j++) {
			file << tasks[i][j] << " ";
		}
		file << "\n";
	}
	file.close();
}
