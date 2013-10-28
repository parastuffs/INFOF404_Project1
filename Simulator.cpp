#include "Simulator.hpp"
#include "GraphCreator.hpp"

#include <iostream>

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
	computeStudInt(tasks);
	int taskPrior[this->tAm];//Array of the priority of each task, ordered by priotity.
	bool schedulable = true;//Flag that be triggered in case a job doesn't meet its deadline.
	setPriorities(tasks,taskPrior,t);

	int taskRunning = taskPrior[0];//Highest priority task uppon beginning
	int taskRunPrior = 0;//Priority of the running task
	int taskStatus;
	int p;
	bool idle=false;//True if the CPU has just been idle.

	for(t=0;t<this->studInt && schedulable;t++) {
	//Shit just got real
	
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
			taskRunPrior = tasks[taskRunning]->getPriority();
		}

		//
		//Launch job uppon new period.
		//
		for(int i=0;i<this->tAm;i++) {
			if(t%tasks[taskPrior[i]]->getPeriod() == 0) {
				tasks[taskPrior[i]]->launchJob();
				if(idle) {
					//A new job will be waiting. CPU won't go idle anymore.
					idle = false;
					this->graph.push_back(t);//Final idle time.
					this->graph.push_back(-4);
				}
			}
		}

		if(!idle) {

			//
			//Searching for a higher priority job waiting.
			//Note: 0 is a higher priority than 1.
			//
			p = taskRunPrior+1;//Temp priority
			bool b=true;
			for(int j=0;b && j < this->tAm;j++) {
				if(tasks[taskPrior[j]]->isWaiting() || tasks[taskPrior[j]]->isRunning()) {
					p = j;
					b=false;
				}
			}
			
			//
			//Preempting if necessary
			//
			if(taskRunPrior > p && tasks[taskRunning]->isRunning() && taskRunning!=taskPrior[p]) {//The task running may have changed of priority due to a delta. We need to make sure that the running task and that want to preempt are different
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
				this->graph.push_back(t+1);//Final task's running time, but it's still running during this unit of time, hence the '+1'.
				this->graph.push_back(-4);
				int i = 0;
				for(i=0;i<this->tAm&&!(tasks[taskPrior[i]]->isWaiting());i++);
				if(i == this->tAm) {
					idle = true;
					this->idleTime++;
				}
				else {
					taskRunning = taskPrior[i];
				}
			}
		}
		else if(idle) {
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

	return schedulable;

}

void Simulator::computeStudInt(Task* tasks[])
{
	//int periods[n];
	int studInt=1;
	for(int i=0;i<this->tAm;i++) {
		studInt = lcm(studInt,tasks[i]->getPeriod());
	}
	this->studInt = studInt;
	cout << "Study interval: " << studInt << endl;
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
