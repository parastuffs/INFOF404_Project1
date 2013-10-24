#include "Simulator.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
//TODO remove useless includes

//#define _DEBUG

using namespace std;

Simulator::Simulator(int n, int tasksParam[][4],int delta)
{

	this->delta = delta;

	Task* tasks[n];//Array containing the tasks objects

	createTasks(tasksParam,tasks,n);

	//TODO refactor to have n and delta as private members of the class
	simulation(tasks,n,delta);
}


void Simulator::createTasks(int tasksParam[][4], Task* tasks[],int n)
{
	for(int i=0;i<n;i++) {
		tasks[i] = new Task(tasksParam[i][0],tasksParam[i][1],
				tasksParam[i][2],tasksParam[i][3]);
	}
}

void Simulator::simulation(Task* tasks[],int n,int delta)
{
	this->preemptions = 0;
	this->idleTime = 0;
	int t=0;//time
	int studInt = computeStudInt(tasks,n);
	int taskPrior[n];

	cout << "### LAXITY ### Before sorting" << endl;
	for(int j=0;j<n;j++) {
		cout << "Task #" << j+1 << ":";
		cout << tasks[j]->getLaxity(t) << endl;
	}
	setPriorities(tasks,taskPrior,n,t);
	cout << "Hence the priorities:" << endl;
	for(int j=0;j<n;j++) {
		cout << "Task #" << j+1 << ":";
		cout << tasks[j]->getPriority() << endl;
	}

	int taskRunning = taskPrior[0];//Highest priority task uppon beginning
	int taskRunPrior = 0;
	int taskStatus;
	int p;
	bool idle;//True if the CPU has just been idle.

	for(t=0;t<studInt;t++) {
	//Shit just got real
	
#ifdef _DEBUG
		cout << "__Time: " << t << endl;
#endif	
		//
		//If on delta, set priorities.
		//
		if(t%delta == 0) {//
			setPriorities(tasks,taskPrior,n,t);
			#ifdef _DEBUG
			cout << "Setting priorities...";
			cout << "Here is what we have, in order of priority:";
			cout << endl;
			for(int k=0;k<n;k++) {
				cout << taskPrior[k] << ", laxity:";
				cout << tasks[taskPrior[k]]->getLaxity(t);
				cout << endl;
			}
			#endif	
			taskRunPrior = taskPrior[taskRunning];
		}

		//
		//Launch job uppon new period.
		//
		for(int i=0;i<n;i++) {
			if(t%tasks[taskPrior[i]]->getPeriod() == 0) {
				tasks[taskPrior[i]]->launchJob();
				#ifdef _DEBUG
				cout << "Lauching a new job for task #";
				cout << taskPrior[i] << endl;
				#endif	
				//A new job will be waiting. CPU won't go idle.
				idle = false;
				//Set the task with a new job as running. If
				//more than one have a new job, the least
				//priority will be set as running. This is
				//then corrected in the next loop.
				taskRunning = taskPrior[i];
				taskRunPrior = i;
			}
		}

		if(!idle) {

			//
			//Searching for a higher priority job waiting.
			//Note: 0 is a higher priority than 1.
			//
			p = taskRunPrior+1;//Temp priority
			for(int j=0;p > taskRunPrior && j < n;j++) {
				if(tasks[taskPrior[j]]->isWaiting()) {
					p = j;
				}
			}
			
			//
			//Preempting if necessary
			//
			if(taskRunPrior > p && tasks[taskRunning]->isRunning()) {
				#ifdef _DEBUG
				cout << "Preempting the task #";
				cout << taskRunning << endl;
				#endif	
				tasks[taskRunning]->preempt();
				if(tasks[taskRunning]->hasStarted()) {
					this->preemptions++;
				}
			}

			taskRunning = taskPrior[p];

			#ifdef _DEBUG
			cout << "Task running is #";
			cout << taskRunning << endl;
			#endif	

			//
			//Run job. If ending, search for the next waiting job.
			//In case there is no waiting job, idle.
			//
			taskStatus = tasks[taskRunning]->runJob();
			if(taskStatus == 0) {
				#ifdef _DEBUG
				cout << "Task #" << taskRunning;
				cout << " ended." << endl;
				#endif	
				int i = 0;
				for(i=0;i<n&&!(tasks[taskPrior[i]]->isWaiting());
						i++);
				if(i == n) {
					this->idleTime++;
					idle = true;
					#ifdef _DEBUG
					cout << "Now in idle time" << endl;
					#endif	
				}
				else {
					taskRunning = taskPrior[i];
					#ifdef _DEBUG
					cout << "Still not idle. FUCK" << endl;
					cout << "i = " << i << endl;
					#endif	
				}
			}
			else if(taskStatus == 1) {
				//Continue with the same job
			}
		}
		else if(idle) {
			this->idleTime++;
		}
		
	}

	//
	//Ouput some information on the simulation
	//
	cout << "### END of the simulation ###" << endl;
	cout << "Total idle time: " << idleTime << endl;
	cout << "Total number of preemptions: " << preemptions << endl;
}

int Simulator::computeStudInt(Task* tasks[],int n)
{
	//int periods[n];
	int studInt=1;
	for(int i=0;i<n;i++) {
		studInt = lcm(studInt,tasks[i]->getPeriod());
	}

	cout << "Study interval: " << studInt << endl;
	this->studInt = studInt;
	//TODO usage a private variable
	return studInt;
}

/**
 * Euclidean Algorithm
 */
int Simulator::gcd(int a,int b)
{
	int i;//remainder of the kth step
	int j;//remainder of the (k-1)th step
	int k;//remainder of the (k-2)th step
	//We want a>b
	if(a<b) {
		int c=a;
		a=b;
		b=c;
	}
	
	j=b;
	k=a;
	do {
		i = k%j;
		k = j;
		j = i;
	}while(i!=0);
	return k;
}

int Simulator::lcm(int a,int b)
{
	return (a/gcd(a,b))*b;
}

void Simulator::setPriorities(Task* tasks[],int taskPrior[],int n,int time)
{
	//1rst column: task, 2nd: laxity
	int taskLax[2][n];
	int temp[2];
	bool permutation;
	int length = n;
	for(int i=0;i<n;i++) {
		taskLax[0][i] = i;
		taskLax[1][i] = tasks[i]->getLaxity(time);
	}

	//Sort with least laxity first
	do{
		permutation = false;
		for(int i=0;i<length-1;i++) {
			if(taskLax[1][i] > taskLax[1][i+1]) {
				temp[0] = taskLax[0][i];
				temp[1] = taskLax[1][i];
				taskLax[0][i] = taskLax[0][i+1];
				taskLax[1][i] = taskLax[1][i+1];
				taskLax[0][i+1] = temp[0];
				taskLax[1][i+1] = temp[1];
				permutation = true;
			}
		}
		length--;
	} while(permutation);

	//Now, I have to got through taskLax[][] and assign
	//the right priority to each task
	for(int i=0;i<n;i++) {
		tasks[taskLax[0][i]]->setPriority(i+1);
		taskPrior[i] = taskLax[0][i];//Tasks ordered by priority
	}

}
