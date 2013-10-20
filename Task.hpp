#ifndef TASK_H
#define TASK_H

class Task
{
public:
	Task();
	Task(int O,int T,int D,int C);
	~Task();
	int getOffset();
	int getPeriod();
	int getDeadline();
	int getWCET();
	int getLaxity(int t);
	int getPriority();
	void setPriority(int p);
	void setOffset(int offset);
	void setDeadline(int deadline);
	bool isRunning() {return running;}
	bool isWaiting() {return waiting;}
	void preempt();
	void launch();
	void recieveCPU();
	void launchJob();
	int runJob();
	bool hasStarted() {return (elapsed > 0);}

private:
	int O,T,D,C;
	int elapsed;//Time spent computing a job of this task
	int priority;
	bool running;
	bool waiting;
};

#endif
