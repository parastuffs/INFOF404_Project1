#include <pngwriter.h>
#include <vector>


int main()
{
	std::vector<int> graph;
	int i;
	int y;
	pngwriter png(300,300,0,"test.png");
	for(i = 1; i < 300;i++) {
	y = 150+100*sin((double)i*9/300.0);
	png.plot(i,y, 0.0, 0.0, 1.0);
	png.filledsquare(50,50,50,50,1.0,0.0,0.0);
	}
	png.close();
	return 0;
}
