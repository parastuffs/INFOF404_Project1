#include "Simulator.hpp"
#include "Generator.hpp"
#include<iostream>

using namespace std;

int main(int argc, char* argv[])
{
	int results[5][5][3];//line - column - {study interval, preemptions, idle times}
	
	//
	//First test: analysis of the system for a fixed number of tasks
	//and different utilization and delta.
	//
	cout << "Computing the first test: analysis of the system for a fixed number of tasks and different utilization and delta." << endl;
	int taskQuantity[] = {2,3,5,7,9};
	int delta[] = {1,2,5,10,20};
	int utilization[] = {50,60,70,80,90};
	int tasks[taskQuantity[1]][4];//3 tasks, will hold the parameters
	for(int i=0;i<5;i++) {//Delta
		Simulator* sim[5];
		for(int j=0;j<5;j++) {//Util
			Generator* gen = new Generator(utilization[j],taskQuantity[1]);
			gen->generateTasks(tasks);
			sim[j] = new Simulator(taskQuantity[1],tasks,delta[i]);
			results[i][j][0] = sim[j]->getStudInt();
			results[i][j][1] = sim[j]->getPreemptions();
			results[i][j][2] = sim[j]->getIdleTime();
		}
	}
	cout << "### Horizontal: Delta, Vertical: Utilization. 3 tasks in the system. ###" << endl;
	for(int i=0;i<5;i++) {
		cout << "\t" << utilization[i] << "\t";
	}
	cout << endl;
	for(int i=0;i<5;i++) {
		cout << delta[i] << "\t";
		for(int j=0;j<5;j++) {
			for(int k=0;k<3;k++) {
				cout << results[i][j][k] << ";";
			}
			cout << "\t";
		}
		cout << endl;
	}



	//
	//Second: constant delta=2.
	//
	cout << "Computing the second test: constant delta=2 and various number of task and utilization." << endl;
	for(int i=0;i<5;i++) {//task
		Simulator* sim[5];
		int tasks[taskQuantity[i]][4];
		for(int j=0;j<5;j++) {//Util
			Generator* gen = new Generator(utilization[j],taskQuantity[i]);
			gen->generateTasks(tasks);
			sim[j] = new Simulator(taskQuantity[i],tasks,delta[1]);
			results[i][j][0] = sim[j]->getStudInt();
			results[i][j][1] = sim[j]->getPreemptions();
			results[i][j][2] = sim[j]->getIdleTime();
		}
	}
	cout << "### Horizontal: Tasks, Vertical: Utilization. Delta = 2. ###" << endl;
	for(int i=0;i<5;i++) {
		cout << "\t" << utilization[i] << "\t";
	}
	cout << endl;
	for(int i=0;i<5;i++) {
		cout << taskQuantity[i] << "\t";
		for(int j=0;j<5;j++) {
			for(int k=0;k<3;k++) {
				cout << results[i][j][k] << ";";
			}
			cout << "\t";
		}
		cout << endl;
	}

	//
	//Third: constant Utilization=70.
	//
	cout << "Computing third and final test: constant utilization of 70 and various task and delta." << endl;
	for(int i=0;i<5;i++) {//task
		Simulator* sim[5];
		int tasks[taskQuantity[i]][4];
		for(int j=0;j<5;j++) {//Delta
			Generator* gen = new Generator(utilization[1],taskQuantity[i]);
			gen->generateTasks(tasks);
			sim[j] = new Simulator(taskQuantity[i],tasks,delta[j]);
			results[i][j][0] = sim[j]->getStudInt();
			results[i][j][1] = sim[j]->getPreemptions();
			results[i][j][2] = sim[j]->getIdleTime();
		}
	}
	cout << "### Horizontal: Tasks, Vertical: Delta. Utilization of 70. ###" << endl;
	for(int i=0;i<5;i++) {
		cout << "\t" << delta[i] << "\t";
	}
	cout << endl;
	for(int i=0;i<5;i++) {
		cout << taskQuantity[i] << "\t";
		for(int j=0;j<5;j++) {
			for(int k=0;k<3;k++) {
				cout << results[i][j][k] << ";";
			}
			cout << "\t";
		}
		cout << endl;
	}

	return 0;
}
