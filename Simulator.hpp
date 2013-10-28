#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "Task.hpp"
#include <vector>

class Simulator
{
public:
	Simulator(int n,int tasksParam[][4],int delta,bool makeGraph=false);
	~Simulator(){}
	void createTasks(int tasksParam[][4],Task* tasks[]);
	int gcd(int a,int b);
	int lcm(int a,int b);
	int computeStudInt(Task* tasks[]);
	bool simulation(Task* tasks[]);
	void setPriorities(Task* tasks[],int taskPrior[],int time);
	int getPreemptions() {return this->preemptions;}
	int getIdleTime() {return this->idleTime;}
	int getStudInt() {return this->studInt;}
//TODO encapsulation of the methods
private:
//	Task* tasks[];
	int preemptions;
	int idleTime;
	int delta;
	int studInt;
	int tAm;//tasks amount
	//graph is a vector working with quintuplets of integers:
	//begining type, task ID, begining time, ending time, ending type.
	//An idle period has the ID '-1'.
	//Begining type can be '-2' (regular begining) or '-3' (continuing after
	//having been preempted.
	//Ending type can be '-4' (regular finish) or '-5' (preempted).
	std::vector<int> graph;
};

#endif
