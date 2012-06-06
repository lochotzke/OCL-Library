#include <iostream>
#include <fstream>
#include <streambuf>
#include "ocl.h"

using namespace std;
int main(){
  // Get available devices and platforms
  oclSetup o;

  // Get platform and device information and return the chosen one
  device dev = o.displayDevices();

  // Create a kernel using the device above from vectoradd.cl
  kernel k(&dev,"vectoradd.cl");

  int N = 1024;

  // Create host variables
  float* a = new float[N*sizeof(float)];
  float* b = new float[N*sizeof(float)]();
  float* c = new float[N*sizeof(float)];

  // Setup the values of a
  for(int i=0;i<N;i++)
    a[i] = i;

  // Allocate memory on the device
  ocl_mem cl_a = dev.malloc(N*sizeof(float),CL_MEM_READ_ONLY);
  ocl_mem cl_b = dev.malloc(N*sizeof(float),CL_MEM_READ_ONLY);
  ocl_mem cl_c = dev.malloc(N*sizeof(float),CL_MEM_WRITE_ONLY);
  
  // Copy host variables a and b to the device variables cl_a and cl_b
  cl_a.copyFrom(a);
  cl_b.copyFrom(b);

  // Set the arguments required for the kernel
  k.setArgs(&N,cl_a.mem(),cl_b.mem(),cl_c.mem());
  // Execute using N-sized work-groups with a total of N work-items
  k.run(N,N);
    
  // Wait until the kernel is done executing
  dev.finish();

  // Copy device variable cl_c to host variable c
  cl_c.copyTo(c);
  
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
