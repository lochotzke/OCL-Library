# Let hPath equal to the directory the ocl files are stored in
#   If they are in the current directory, leave blank
hPath = .

# Change -I/usr/local/cuda/include to the location where your
#   cl.h file is in
paths = -I${hPath} -I/usr/local/cuda/include

# Change -L/usr/local/cuda/lib to the location where your
#   OpenCL libraries are stored in
links = -lm -L/usr/local/cuda/lib -lOpenCL
sources = example3.cpp ocl.cpp 
objects = $(sources:.cpp=.o)

all: example
	rm *.o
example: $(objects)
	g++ -g $(objects) $(paths) $(links)
%.o:%.cpp
	g++ -g -c -w $(paths) $<
clean:
	rm *.o;

VPATH = src:$(hPath)