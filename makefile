hPath = ${HOME}/.headers
paths = -I${hPath} -I/usr/local/cuda/include
links = -fopenmp -lm -L/usr/local/cuda/lib -lOpenCL
sources = main.cpp ocl.cpp 
objects = $(sources:.cpp=.o)

all: main
	rm *.o
main: $(objects)
	g++ $(objects) $(paths) $(links)
%.o:%.cpp
	g++ -c -w $(paths) $<
clean:
	rm *.o;

VPATH = src:$(hPath)