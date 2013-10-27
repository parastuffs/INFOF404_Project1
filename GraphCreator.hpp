#ifndef GRAPHCREATOR_H
#define GRAPHCREATOR_H
#include <vector>

class GraphCreator
{
public:
	GraphCreator(std::vector<int> points,int delta,int studInt,int tAm,int preemps,int idleT, int graphMarks[][2]);
	~GraphCreator(){}


};

#endif
