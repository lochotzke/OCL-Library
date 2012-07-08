//--------------------------------------------------------------------//
//   Copyright (c) 2012, Tim Warburton, David Medina                  //
//   All rights reserved.                                             //
//   License Located in:                                              //
//      https://github.com/dmed256/OCL-Library/blob/master/LICENSE    //
//--------------------------------------------------------------------//

#ifndef OCL_LIBRARY_H
#define OCL_LIBRARY_H

#include <iostream>
#include <vector>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

class ocl_kernel;
class ocl_device;
class ocl_mem;
class ocl_context;
class ocl_commandQueue;

class ocl_setup{
private:
  int* allocs;
  std::string shortInfo,longInfo,question1,question2;

  cl_platform_id* pID;
  cl_device_id**  dID;
  cl_uint  pSize;
  cl_uint* dSize;

  cl_int err;

public:
  ocl_setup();
  ocl_setup(const ocl_setup&);
  ~ocl_setup();
  void destructor();
  void copyCheck(int*);
  ocl_setup& operator=(const ocl_setup&);
  void findDevices();
  void findDeviceInformation();
  ocl_device displayDevices();
  ocl_device getDevice(int,int);
  cl_platform_id getPlatformID(int);
  cl_device_id getDeviceID(int,int);
};

class ocl_kernel{
private:
  int* allocs;

  ocl_device* device;

  std::string flags;

  cl_kernel kernel;
  cl_program program;
  
  std::string name;
  std::string function;

  int inputs;
  int* inputSize;
  std::vector<std::string> inputType;

  cl_uint dims;
  size_t* lDims;
  size_t* gDims;

  int format;

  std::vector<cl_event>* events;

public:
  ocl_kernel();
  ocl_kernel(const ocl_kernel&);
  ocl_kernel(ocl_device*,std::string);
  ocl_kernel(ocl_device*,std::string,std::string);
  ocl_kernel(ocl_device*,std::string,int);
  ocl_kernel(ocl_device*,std::string,std::string,int);
  ~ocl_kernel();
  void destructor();
  void copyCheck(int*);
  ocl_kernel& operator=(const ocl_kernel&);
  void setup(ocl_device*,std::string);
  void getKernelInformation(std::string);
  int sizeofType(std::string);
  void setArgs(void*,...);
  void setArg(int,void*);
  std::string getArgType(int);
  void setDims(size_t,size_t);
  void setDims(int,size_t,size_t);
  void setDims(int,size_t*,size_t*);
  void run();
  void run(size_t,size_t);
  void run(int,size_t*,size_t*);
  int timedRun();
  int timedRun(size_t,size_t);
  int timedRun(int,size_t*,size_t*);
  float getRunTime(int);
  void printKernel();
  void printFormattedKernel();
  cl_kernel getKernel();
  cl_program getProgram();
  std::string getName();
  void setName(std::string);
  std::string getFunction();
  void setFunction(std::string);
  std::string getFlags();
  void setFlags(std::string);  
  int getWarpSize();
  int getWavefrontSize();
  int getGroupSize(int);
};

class ocl_context{
private:
  int* allocs;
  cl_context* context;
public:
  ocl_context();
  ocl_context(const ocl_context&);
  ~ocl_context();
  void destructor();
  void copyCheck(int*);
  ocl_context& operator=(const ocl_context&);
  void create(cl_device_id*);
  cl_context getContext();
};

class ocl_commandQueue{
private:
  int* allocs;
  cl_command_queue* commandQueue;
public:
  ocl_commandQueue();
  ocl_commandQueue(const ocl_commandQueue&);
  ~ocl_commandQueue();
  void destructor();
  void copyCheck(int*);
  ocl_commandQueue& operator=(const ocl_commandQueue&);
  void create(cl_context context,cl_device_id dID);
  cl_command_queue getCommandQueue();
  void finish();
  void flush();
};

class ocl_device{
private:
  int* allocs;

  cl_platform_id pID;
  cl_device_id   dID;  

  ocl_context context;
  ocl_commandQueue commandQueue;

  int* groupSize;

public:
  ocl_device();
  ocl_device(const ocl_device&);
  ocl_device(cl_platform_id,cl_device_id);
  ~ocl_device();
  void destructor();
  void copyCheck(int*);
  ocl_device& operator=(const ocl_device&);
  void refresh();
  ocl_mem malloc(size_t);
  ocl_mem malloc(size_t,cl_mem_flags);
  void barrier();
  void finish();
  void flush();
  cl_platform_id getPlatformID();
  cl_device_id getDeviceID();
  cl_context getContext();
  cl_command_queue getCommandQueue();
  int getGroupSize(int);
};

class ocl_mem{
private:
  int* allocs;
  ocl_device* device;
  cl_mem memory;
  size_t size;

public:
  ocl_mem();
  ocl_mem(const ocl_mem&);
  ocl_mem(ocl_device*,cl_mem);
  ocl_mem(ocl_device*,cl_mem,size_t);
  ~ocl_mem();
  void destructor();
  void copyCheck(int*);
  ocl_mem& operator=(const ocl_mem&);
  void copyTo(void*);
  void copyTo(void*,size_t,size_t);
  void copyToNB(void*);
  void copyToNB(void*,size_t,size_t);
  void copyFrom(void*);
  void copyFrom(void*,size_t,size_t);
  void copyFromNB(void*);
  void copyFromNB(void*,size_t,size_t);
  void free();
  cl_mem* mem();
  size_t getSize();
};

// Opencl Variable Sizes
namespace ocl{
  extern ocl_setup setup;

  extern ocl_device displayDevices();
  extern ocl_device getDevice(int,int);
  extern cl_platform_id getPlatformID(int);
  extern cl_device_id getDeviceID(int,int);
  
  extern const int types;
  extern const std::string type[];
  extern const int typeSize[];
  extern const char error[65][45];

  extern std::string getFormattedKernel(ocl_kernel&);
  extern std::string getFormattedKernel(std::string);
  extern int parseKernelOperatorCheck(int&,int&,std::stringstream&,std::string,std::vector<std::string>&,std::vector<int>&);
  extern void checkParsedKernel(int&,std::string,std::stringstream&,std::vector<std::string>&,std::vector<int>&);
  extern void parseKernelSpaceCheck(int&,std::stringstream&,std::string,std::string&);
  extern void parseKernel(std::string,std::vector<std::string>&,std::vector<int>&);

  extern void printError(std::string,int);
  extern void printSizes();
};

#endif
