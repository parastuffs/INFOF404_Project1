#include <string>
#include <cstdlib>

#include "Simulator.hpp"


using namespace std;

int main(int argc, char** argv)
{
	new Simulator(argv[2],atoi(((string)argv[1]).c_str()));	
	return 0;
}
