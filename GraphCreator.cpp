#include "GraphCreator.hpp"
#include <pngwriter.h>
#include <iostream>

using namespace std;

GraphCreator::GraphCreator(vector<int> points,int delta,int studInt,int tAm,int preemps,int idleT)
{
	int maxX = studInt*20+20;
	int maxY = tAm*20+20;

	cout << "maxX,maxY: " << maxX << ", " << maxY << endl;
	pngwriter png(maxX,maxY,0,"graph.png");
	for(unsigned int i=0;i<points.size();i+=5) {
		if(points[i+1] == -1) {
			png.filledsquare((points[i+2]*20)+1,1,(points[i+3]*20)+1,tAm*20+1,0.0,1.0,0.0);
		}
		else {
			png.filledsquare(points[i+2]*20+1,points[i+1]*20+1,points[i+3]*20+1,(points[i+1]+1)*20+1,0.0,0.0,1.0);
		}
	}
	png.close();
}
