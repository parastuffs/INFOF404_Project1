#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "Task.hpp"

class Simulator
{
public:
	Simulator(int n,int tasksParam[][4],int delta);
	~Simulator(){}
	void createTasks(int tasksParam[][4],Task* tasks[],int n);
	int gcd(int a,int b);
	int lcm(int a,int b);
	int computeStudInt(Task* tasks[],int n);
	void simulation(Task* tasks[],int n,int delta);
	void setPriorities(Task* tasks[],int taskPrior[],int n,int time);
	int getPreemptions() {return this->preemptions;}
	int getIdleTime() {return this->idleTime;}

private:
//	Task* tasks[];
	int preemptions;
	int idleTime;
	int delta;
};

#endif
