#include <iostream>
#include <sstream>
#include "ocl.h"

using namespace std;

string getAddKernel(int);

int main(){
  // Get available devices and platforms
  ocl_setup o;

  // Find all devices
  ocl_device device = o.displayDevices();

  int N = 1024;

  // Create a kernel using the device above from vectoradd.cl
  ocl_kernel kernel(&device,getAddKernel(N));

  // We can copy without memory leaks
  ocl_kernel add = kernel;  

  // Print the add kernel with the current "dumb" parser
  ocl::printKernel(add);

  // Create host variables
  float* a = new float[N*sizeof(float)];
  float* b = new float[N*sizeof(float)]();
  float* c = new float[N*sizeof(float)];

  // Setup the values of a
  for(int i=0;i<N;i++)
    a[i] = i;

  // Allocate memory on the device
  //   I like to use _var for the GPU variable that corresponds to var
  //   but it's just a matter of taste
  ocl_mem _a = device.malloc(N*sizeof(float),CL_MEM_READ_ONLY);
  ocl_mem _b = device.malloc(N*sizeof(float),CL_MEM_READ_ONLY);
  ocl_mem _c = device.malloc(N*sizeof(float),CL_MEM_WRITE_ONLY);
  
  // When doing device.malloc, the variable gets the size and is able to copy
  //   and write without needing the size again
  _a.copyFrom(a);

  // However, partial copy is also available
  //    copyFrom(array,offset,size you want to copy)
  _b.copyFrom(b,0*sizeof(float),N*sizeof(float));

  // Set the arguments required for the kernel
  //   This was not noted on example1.cpp
  //     but the arguments are automatically
  //     scanned when creating the kernel.
  //   This saves a lot of time inputting them
  //     individually.
  add.setArgs(&N,_a.mem(),_b.mem(),_c.mem());

  // We can also set the args manually (which is useful when checking constants)
  add.setArg(0,&N);

  // When doing multiple executes, it is faster to set the dimension before 
  //   executing.
  add.setDims(N,N);

  add.run();

  // Wait until the kernel is done executing
  device.finish();

  // Copy device variable _c to host variable c
  _c.copyTo(c);
  
  // Output c 
  // Should be 0,1,...,N-1
  for(int i=0;i<N;i++)
    cout << c[i] << ',';
  cout << endl;

  // Free host variables
  delete[] a;
  delete[] b;
  delete[] c;
}

string getAddKernel(int size){
  stringstream ret;

  ret << "__kernel void vectoradd(const int N,"
      << "__global float *a,"
      << "__global float *b,"
      << "__global float *c){";
  ret << "int n = get_global_id(0);"
      << "if(n<N)"
      << "c[n] = a[n] + b[n];"
      << '}';  

  return ret.str();
}
