#ifndef TASK_H
#define TASK_H

class Task
{
public:
	Task();//TODO not necessary anymore, I think
	Task(int O,int T,int D,int C);
	~Task();
	int getOffset() {return this->O;}
	int getPeriod() {return this->T;}
	int getDeadline() {return this->D;}
	int getWCET() {return this->C;}
	int getLaxity(int t);
	int getPriority() {return this->priority;}
	void setPriority(int p) {this->priority=p;}
	bool isRunning() {return running;}
	bool isWaiting() {return waiting;}
	void preempt();
	void launch();
	void recieveCPU();
	void launchJob();
	int runJob();
	bool hasStarted();

private:
	int O,T,D,C;
	int elapsed;//Time spent computing a job of this task
	int priority;
	bool running;
	bool waiting;
};

#endif
