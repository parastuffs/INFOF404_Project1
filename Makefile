TARGET=LLF_study simLLF taskGenerator

normal: $(TARGET)

LLF_study: Simulator.cpp Task.cpp GraphCreator.cpp LLF_study.cpp Generator.cpp
	g++ -Wall Simulator.cpp Task.cpp GraphCreator.cpp LLF_study.cpp Generator.cpp -o LLF_study `freetype-config --cflags` -I/usr/local/include -L/usr/local/lib -lpng -lpngwriter -lz -lfreetype

simLLF: Simulator.cpp Task.cpp simLLF.cpp GraphCreator.cpp
	g++ -Wall Simulator.cpp Task.cpp simLLF.cpp GraphCreator.cpp -o simLLF `freetype-config --cflags` -I/usr/local/include -L/usr/local/lib -lpng -lpngwriter -lz -lfreetype

taskGenerator: taskGenerator.cpp Generator.cpp
	g++ -Wall -o taskGenerator taskGenerator.cpp Generator.cpp
