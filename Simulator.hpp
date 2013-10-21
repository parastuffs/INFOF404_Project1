#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "Task.hpp"

class Simulator
{
public:
	Simulator(char* inputFile,int delta);
	~Simulator(){}
	int countTasks(char* inputFile);
	void tasksFromFile(char* inputFile,Task* tasks[]);
	int gcd(int a,int b);
	int lcm(int a,int b);
	int computeStudInt(Task* tasks[],int n);
	void simulation(Task* tasks[],int n,int delta);
	void setPriorities(Task* tasks[],int taskPrior[],int n,int time);

private:
//	Task* tasks[];
};

#endif
