#include "Task.hpp"

Task::Task(){}

Task::Task(int O,int T,int D,int C)
{
	this->O = O;
	this->T = T;
	this->D = D;
	this->C = C;

	this->elapsed = 0;
	this->running = false;
}

Task::~Task(){}

int Task::getOffset()
{
	return this->O;
}

void Task::setOffset(int O)
{
	this->O=O;
}

int Task::getPeriod()
{
	return this->T;
}

int Task::getDeadline()
{
	return this->D;
}

void Task::setDeadline(int D)
{
	this->D=D;
}

int Task::getWCET()
{
	return this->C;
}

int Task::getLaxity(int time)
{
	//Beware, (time/T)*T can't be simplified, since time/T
	//doesn't keep the remainder
	return (time/this->T)*this->T + this->D - time - (this->C - this->elapsed);
}

void Task::setPriority(int p)
{
	this->priority = p;
}

int Task::getPriority()
{
	return this->priority;
}

void Task::preempt()
{
	this->waiting = true;
}

void Task::launch()
{
	this->waiting = false;
}

void Task::recieveCPU()
{
	launch();
}

void Task::launchJob()
{
	this->elapsed = 0;
	this->waiting = true;
	this->running = true;
}

int Task::runJob()
{
	this->elapsed++;
	if(this->elapsed == C) {//Job is over
		this->running = false;
		this->waiting = false;
		return 0;
	}
	else
		return 1;
}
