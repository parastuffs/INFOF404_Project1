#include "Simulator.hpp"
#include "Generator.hpp"
#include<iostream>

using namespace std;

int main(int argc, char* argv[])
{
	//What I could do: do a matrix of result by changing the delta
	//and the utilization. Well, atcually I have 3 free parameters,
	//so I'll probably need three of those matrix.
	
	//
	//First test: analysis of the system for a fixed number of tasks
	//and delta, and different utilization.
	//
	int taskQuantity = 5;
	int delta = 10;
	int tasks[taskQuantity][4];
	int utilization[] = {50,60,70,80,90};
	Simulator* sim[5];
	for(int i=0;i<5;i++) {
		Generator* gen = new Generator(utilization[i],taskQuantity);
		gen->generateTasks(tasks);
		sim[i] = new Simulator(taskQuantity,tasks,delta);
	}

	//Note: computing the ratio CPU idle time/study interval

	for(int i=0;i<5;i++) {
		cout << "### Simulation " << i << endl;
		cout << "_Utilization: " << utilization[i] << endl;
		cout << "_Study interval: " << sim[i]->getStudInt() << endl;
		cout << "_Preemptions: " << sim[i]->getPreemptions() << endl;
		cout << "_CPU idle time: " << sim[i]->getIdleTime() << endl;
	}	

	return 0;
}
