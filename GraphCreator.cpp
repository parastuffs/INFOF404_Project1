#include "GraphCreator.hpp"
#include <pngwriter.h>
#include <iostream>

using namespace std;

GraphCreator::GraphCreator(vector<int> points,int delta,int studInt,int tAm,int preemps,int idleT,int graphMarks[][2])
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
			if(points[i] == -3) {
				png.filledtriangle(points[i+2]*20+1,points[i+1]*20+1,points[i+2]*20+1,(points[i+1]+1)*20+1,points[i+2]*20-5,points[i+1]*20+11,1.0,0.0,0.0);
			}
			if(points[i+4] == -5) {
				png.filledtriangle(points[i+3]*20+1,points[i+1]*20+1,points[i+3]*20+1,(points[i+1]+1)*20+1,points[i+3]*20+6,points[i+1]*20+11,1.0,0.0,0.0);
			}
		}
	}
	for(int i=0;i<tAm;i++) {
		for(int j=0;j<=studInt/graphMarks[i][0];j++) {
			cout << "period: " << graphMarks[i][0] << endl;
		//Periods
			png.arrow(graphMarks[i][0]*j*20+1,(i+1)*20+1,graphMarks[i][0]*j*20+1,i*20+1,7,0.3,1.0,1.0,1.0);
		}
		for(int j=0;j<=studInt/graphMarks[i][1];j++) {
			cout << "Deadline: " << graphMarks[i][1] << endl;
		//Deadlines
			png.filleddiamond(graphMarks[i][0]*j*20+1+graphMarks[i][1]*20,i*20+1,8,8,1.0,1.0,1.0);
		}
	}
	png.close();
}
