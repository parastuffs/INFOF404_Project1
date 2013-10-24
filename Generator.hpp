#ifndef GENERATOR_H
#define GENERATOR_H

class Generator
{
public:
	Generator(int u, int tN);
	~Generator(){}
	void generateTasks(int tasks[][4]);
	void tasksToFile(char* outFile, int tasks[][4]);
private:
	int util;
	int tasksNumber;
};

#endif
