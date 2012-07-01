#include <iostream>
#include <sstream>
#include "ocl.h"

using namespace std;

string getReductionKernel(int,int,int,int,int);

int main(){
  // Find all devices
  ocl_device device = ocl::displayDevices();
 
  int N = 102400;

  // We'll get the group size of the device for the first dimension
  int groupSize = device.getGroupSize(0);

  // Only kernels can see the warp size (NVIDIA) or wavefront size (AMD)
  //   so we'll make a dummy kernel to find the information
  ocl_kernel dummy(&device,"__kernel void dummy(){}");
  int warpSize = dummy.getWarpSize();

  // We'll make the reduction with partitions
  int partitions = 10;

  // Each partition will multiply loops*groupSize number of entries
  int loops = (N + partitions*groupSize - 1)/(partitions*groupSize);

  // We can use OCL's printFormattedKernel to see the nice output of 
  //   a kernel without involving a kernel initialization
  //   Good for debugging
  cout << ocl::getFormattedKernel(getReductionKernel(N,groupSize,warpSize,partitions,loops)) << endl;

  // Create a kernel using the device above from getReductionKernel()
  //   We'll use the warp size (NVIDIA) or wavefront size (AMD)
  //   To make our add kernel more dynamics
  ocl_kernel reduction(&device,getReductionKernel(N,groupSize,warpSize,partitions,loops));

  // Create host variables
  float* a = new float[N];
  float* b = new float[N];
  float* c = new float[partitions];

  // Setup the values of a and b
  for(int i=1;i<=N;i++){
    a[i-1] = i;
    b[i-1] = i;
  }

  // Allocate memory on the device
  ocl_mem _a = device.malloc(N*sizeof(float),CL_MEM_READ_ONLY);
  ocl_mem _b = device.malloc(N*sizeof(float),CL_MEM_READ_ONLY);
  ocl_mem _c = device.malloc(partitions*sizeof(float),CL_MEM_WRITE_ONLY);
  
  _a.copyFrom(a);
  _b.copyFrom(b);

  reduction.setArgs(_a.mem(),_b.mem(),_c.mem());

  // timedRun returns the time ID corresponding to it
  //   which can later be used to find the execution
  //   time
  int tID = reduction.timedRun(groupSize,partitions*groupSize);

  // Print the time taken
  // Note: getRunTime() waits for the event to finish executing
  //         which might block the command queue
  cout << fixed << "Time Taken: " << reduction.getRunTime(tID) << " ms\n";

  // Copy device variable _c to host variable c
  _c.copyTo(c);
  
  for(int i=1;i<partitions;i++)
    c[0] += c[i];
  
  // Sum of squares
  float expected = (N/6.f)*(N+1)*(2*N+1.f);
  cout << scientific
       << "Reduction          : " << c[0] << endl
       << "Expected Reduction : " << expected << endl
       << "Relative Error     : " << (c[0]-expected)/expected << endl;

  // Free host variables
  delete[] a;
  delete[] b;
  delete[] c;
}

string getReductionKernel(int size,int groupSize,int warpSize,int partitions,int loops){
  stringstream ret;

  ret << "__kernel void reduction(__global float *a,"
      << "__global float *b,"
      << "__global float *c){"
      << "int gID = get_global_id(0);"
      << "int lID = get_local_id(0);"
      << "volatile __local float l_value[" << groupSize << "];"
      << "float value = 0.;";

  // We can easily unroll the for loop
  //   as well as get rid of the if-checks
  for(int i=0;i<loops-1;i++)
    ret << "value += a[gID]*b[gID];"
	<< "gID +=" << partitions*groupSize << ";";

  ret << "if(gID < " << size << ")"
      << "value += a[gID]*b[gID];";

  // We unroll the local reductions 
  //   (Assuming groupSize is a multiple of 2)
  ret << "l_value[lID] = value;"
      << "barrier(CLK_LOCAL_MEM_FENCE);";

  while(groupSize > 2*warpSize){
    ret << "if(lID < " << groupSize/2 << ")"
	<< "l_value[lID] += l_value[lID + " << groupSize/2 << "];"
	<< "barrier(CLK_LOCAL_MEM_FENCE);";
    groupSize /= 2;
  }

  // We're taking advantage that we're now working on one warp
  ret << "if(lID < " << warpSize << "){"
      << "l_value[lID] += l_value[lID+32];"
      << "l_value[lID] += l_value[lID+16];"
      << "l_value[lID] += l_value[lID+8];"
      << "l_value[lID] += l_value[lID+4];"
      << "}";
  
  // The first work item will do the last summation
  ret << "if(!lID)"
      << "c[get_group_id(0)] = l_value[0] + l_value[1] + l_value[2] + l_value[3];";

  ret << "}";  

  return ret.str();
}
