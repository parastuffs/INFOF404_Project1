#include "Task.hpp"

Task::Task(int O,int T,int D,int C)
{
	this->O = O;
	this->T = T;
	this->D = D;
	this->C = C;

	this->elapsed = 0;
	this->running = false;
}

int Task::getLaxity(int time)
{
	//Beware, (time/T)*T can't be simplified, since time/T
	//doesn't keep the remainder
	return (time/this->T)*this->T + this->D - time - (this->C - this->elapsed);
}

void Task::preempt()
{
	this->waiting = true;
}

void Task::launch()
{
	this->waiting = false;
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

bool Task::hasStarted()
{
	return (elapsed > 0);
}

bool Task::isSchedulable(int time)
{
	if(this->running)//OK if the deadline is still ahead.
		return (((time/this->T)*this->T + this->D) > time);
	else
		return true;
}
