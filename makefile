paths = -I/usr/local/cuda/include
links = -fopenmp -lm -L/usr/local/cuda/lib -lOpenCL
sources = main.cpp ocl.cpp
objects = $(sources:.cpp=.o)

all: main
	rm *.o
main: $(objects)
	g++ $(objects) $(paths) $(links)
main.o: main.cpp
	g++ -c -w  main.cpp
ocl.o: ocl.cpp
	g++ -c -w ocl.cpp
clean:
	rm *.o;
