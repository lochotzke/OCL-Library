#include <iostream>
#include <fstream>
#include <streambuf>
#include "ocl.h"

using namespace std;

int main(){
  ocSetup o;
  device dev = o.getDevice(1,3);
  kernel k(&dev,"vectoradd.cl");

  float* a = (float*) malloc(N*sizeof(float));
  float* b = (float*) calloc(N*sizeof(float));
  float* c = (float*) malloc(N*sizeof(float));

  for(int i=0;i<N;i++)
    a = i;

  ocl_mem cl_a = dev.malloc(N*sizeof(float),CL_MEM_READ_ONLY);
  ocl_mem cl_b = dev.malloc(N*sizeof(float),CL_MEM_READ_ONLY);
  ocl_mem cl_c = dev.malloc(N*sizeof(float),CL_MEM_WRITE_ONLY);
  
  cl_a.copyFrom(a);
  cl_b.copyFrom(b);

  k.setArgs(&N,cl_a.mem(),cl_b.mem(),cl_c.mem());
  k.run(N,N);
    
  dev.finish();

  cl_c.copyTo(c);
  
  for(int i=0;i<N;i++)
    cout << c[i] << ',';
  cout << endl;

  free(a);
  free(b);
  free(c);
}
