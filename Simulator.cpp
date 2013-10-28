#include "Simulator.hpp"
#include "GraphCreator.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
//TODO remove useless includes

//TODO remove _DEBUG statements
//#define _DEBUG

using namespace std;

Simulator::Simulator(int n, int tasksParam[][4],int delta,bool makeGraph)
{

	this->delta = delta;
	this->tAm = n;
	bool success;

	Task* tasks[tAm];//Array containing the tasks objects

	createTasks(tasksParam,tasks);

	success = simulation(tasks);
	if(success) {
		if(makeGraph) {
			cout << "Generating the graph of the scheduling under 'graph.png'" << endl;
			int graphMarks[this->tAm][2];//{Period,Deadline} to mark on the graph
			for(int i=0;i<this->tAm;i++) {
				graphMarks[i][0] = tasks[i]->getPeriod();
				graphMarks[i][1] = tasks[i]->getDeadline();
			}
			new GraphCreator(this->graph, this->delta, this->studInt,this->tAm,this->preemptions,this->idleTime,graphMarks);
		}
	}
	else {
		this->preemptions = -1;
		this->idleTime = -1;
		cout << "The system was not schedulable. Please try again with a lower delta." << endl;
	}
}


void Simulator::createTasks(int tasksParam[][4], Task* tasks[])
{
	for(int i=0;i<this->tAm;i++) {
		tasks[i] = new Task(tasksParam[i][0],tasksParam[i][1],
				tasksParam[i][2],tasksParam[i][3]);
	}
}

bool Simulator::simulation(Task* tasks[])
{
	this->preemptions = 0;
	this->idleTime = 0;
	int t=0;//time
	int studInt = computeStudInt(tasks);
	int taskPrior[this->tAm];//Array of the priority of each task, ordered by priotity.
	bool schedulable = true;//Flag that be triggered in case a job doesn't meet its deadline.
//TODO make the time a private member
	cout << "### LAXITY ### Before sorting" << endl;
	for(int j=0;j<this->tAm;j++) {
		cout << "Task #" << j+1 << ":";
		cout << tasks[j]->getLaxity(t) << endl;
	}
	setPriorities(tasks,taskPrior,t);
	cout << "Hence the priorities:" << endl;
	for(int j=0;j<this->tAm;j++) {
		cout << "Task #" << j+1 << ":";
		cout << tasks[j]->getPriority() << endl;
	}

	int taskRunning = taskPrior[0];//Highest priority task uppon beginning
	int taskRunPrior = 0;//Priority of the running task
	int taskStatus;
	int p;
	bool idle=false;//True if the CPU has just been idle.

	for(t=0;t<studInt && schedulable;t++) {
	//Shit just got real
	
#ifdef _DEBUG
		cout << "__Time: " << t << endl;
#endif	
		//
		//Verifying the schedulability of the system
		//
		for(int i=0; i<this->tAm; i++) {
			if(!(tasks[i]->isSchedulable(t)))
				schedulable = false;
		}


		//
		//If on delta, set priorities.
		//
		if(t%this->delta == 0) {//
			setPriorities(tasks,taskPrior,t);
			#ifdef _DEBUG
			cout << "Setting priorities...";
			cout << "Here is what we have, in order of priority:";
			cout << endl;
			for(int k=0;k<this->tAm;k++) {
				cout << "task " << taskPrior[k] << ", laxity:";
				cout << tasks[taskPrior[k]]->getLaxity(t);
				cout << endl;
			}
			#endif	
			taskRunPrior = tasks[taskRunning]->getPriority();
//			cout << "After delta, taskRunPrior=" << taskRunPrior << endl;
		}

		//
		//Launch job uppon new period.
		//
		for(int i=0;i<this->tAm;i++) {
			if(t%tasks[taskPrior[i]]->getPeriod() == 0) {
				tasks[taskPrior[i]]->launchJob();
				#ifdef _DEBUG
				cout << "Lauching a new job for task #";
				cout << taskPrior[i] << endl;
				#endif	
				if(idle) {
					//A new job will be waiting. CPU won't go idle anymore.
					idle = false;
					this->graph.push_back(t);//Final idle time.
					this->graph.push_back(-4);
				}
				/*
				//Set the task with a new job as running. If
				//more than one have a new job, the least
				//priority will be set as running. This is
				//then corrected in the next loop.
				taskRunning = taskPrior[i];
				taskRunPrior = i;
				*/
			}
		}

		if(!idle) {

			//
			//Searching for a higher priority job waiting.
			//Note: 0 is a higher priority than 1.
			//
			p = taskRunPrior+1;//Temp priority
//			cout << "Searching higher priority: p="<<p<<", taskRunPrior="<<taskRunPrior<<endl;
			for(int i=0; i<this->tAm; i++) {
//				cout << "i="<<i<< "->"<<taskPrior[i]<<" ";
			}
//			cout << endl;
			bool b=true;
			for(int j=0;b && j < this->tAm;j++) {
//				cout << "p="<<p<<"; "<<tasks[taskPrior[j]]->getPriority()<<"; waiting: "<<tasks[taskPrior[j]]->isWaiting()<<" ";
				if(tasks[taskPrior[j]]->isWaiting() || tasks[taskPrior[j]]->isRunning()) {
					p = j;
					b=false;
				}
			}
#ifdef _DEBUG
			cout << "After the search for higher priority: p="<<p<<", taskRunPrior="<<taskRunPrior<<endl;
			cout << "tasks(p)=" << taskPrior[p] << "; tasks(TRP)=" << taskPrior[taskRunPrior] << endl;
#endif
			
			//
			//Preempting if necessary
			//
			if(taskRunPrior > p && tasks[taskRunning]->isRunning() && taskRunning!=taskPrior[p]) {//The task running may have changed of priority due to a delta. We need to make sure that the running task and that want to preempt are different
				#ifdef _DEBUG
				cout << "Preempting the task #";
				cout << taskRunning << endl;
				#endif	
				tasks[taskRunning]->preempt();
				if(tasks[taskRunning]->hasStarted()) {
					//Only if the task being interrupted has
					//laready a job running.
					this->graph.push_back(t);
					this->graph.push_back(-5);
				}
				taskRunning = taskPrior[p];
				taskRunPrior = p;
				tasks[taskRunning]->launch();
				if(tasks[taskRunning]->hasStarted()) {
					//If it has started, this is a continue
					this->graph.push_back(-3);
				}
				else {
					this->graph.push_back(-2);
				}
				this->graph.push_back(taskPrior[p]);//Task preempting
				this->graph.push_back(t);
				if(tasks[taskRunning]->hasStarted()) {
					this->preemptions++;
				}
			}
			else {
				taskRunning = taskPrior[p];
				if(!(tasks[taskRunning]->hasStarted())) {
					//New job running, but not resulting
					//of a preemption.
					this->graph.push_back(-2);
					this->graph.push_back(taskRunning);
					this->graph.push_back(t);
				}
			}

			#ifdef _DEBUG
			cout << "Task running is #";
			cout << taskRunning << endl;
			#endif	

			//If in the previous unit of time, a job reached its end,
			//and if an other job was already waiting and is now
			//executed, this job did not have a chance to make
			//its come back in the graph vector. Now is the time.
			if(this->graph.back() == -4 && tasks[taskRunning]->hasStarted()) {
				this->graph.push_back(-3);
				this->graph.push_back(taskRunning);
				this->graph.push_back(t);
			}

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
				this->graph.push_back(t+1);//Final task's running time, but it's still running during this unit of time, hence the '+1'.
				this->graph.push_back(-4);
				int i = 0;
				for(i=0;i<this->tAm&&!(tasks[taskPrior[i]]->isWaiting());i++);
				if(i == this->tAm) {
					idle = true;
					this->idleTime++;
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
#ifdef _DEBUG
			cout << "Idling" << endl;
#endif
			if(this->graph.back() == -4) {
			//If the previous unit of time was suitable for an idle
			//time, that is the en of a job and no other job waiting,
			//the actual idle time has began during this very unit
			//of time.
				this->graph.push_back(-2);
				this->graph.push_back(-1);
				this->graph.push_back(t);
			}
			this->idleTime++;
		}
		
	}
	//Closing the graph
	this->graph.push_back(t);
	this->graph.push_back(-4);

	//
	//Ouput some information on the simulation
	//
	cout << "### END of the simulation ###" << endl;
	cout << "Total idle time: " << idleTime << endl;
	cout << "Total number of preemptions: " << preemptions << endl;
#ifdef _DEBUG
	cout << "Graph vector:" << endl;
	for(unsigned int i=0;i<this->graph.size();i++) {
		cout << this->graph[i] << " ";
		if(this->graph[i] == -4 || this->graph[i] == -5) {
			cout << endl;
		}
	}
#endif

	return schedulable;

}

int Simulator::computeStudInt(Task* tasks[])
{
	//int periods[n];
	int studInt=1;
	for(int i=0;i<this->tAm;i++) {
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

void Simulator::setPriorities(Task* tasks[],int taskPrior[],int time)
{
	//1rst column: task, 2nd: laxity
	int taskLax[2][this->tAm];
	int temp[2];
	bool permutation;
	int length = this->tAm;
	for(int i=0;i<this->tAm;i++) {
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
	for(int i=0;i<this->tAm;i++) {
		//tasks[taskLax[0][i]]->setPriority(i+1);//Why the hell is there a '+1'?
		tasks[taskLax[0][i]]->setPriority(i);
		taskPrior[i] = taskLax[0][i];//Tasks ordered by priority
	}

}
