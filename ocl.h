//----------------------------------------------------//
// This is the OCL Library header, feel free to       //
//    use and edit for your own use. This is free     //
//    to the public so please give credit and post    //
//    it free for others to use as well.              //
//                                                    //
// David Medina                                       //
//----------------------------------------------------//

#ifndef OCL_SETUP_H
#define OCL_SETUP_H

#include <iostream>
#include <CL/cl.h>

class kernel;
class device;
class ocl_mem;

class oclSetup{
 private:
  std::string shortInfo,longInfo,question1,question2;

  cl_platform_id* pID;
  cl_device_id**  dID;
  cl_uint  pSize;
  cl_uint* dSize;

  cl_int err;

 public:
  oclSetup();
  ~oclSetup();
  void findDevices();
  void findDeviceInformation();
  device displayDevices();
  device getDevice(int,int);
};

class kernel{
 private:
  device* dev;

  std::string flags;

  cl_kernel ker;
  cl_program program;
  
  std::string name;
  std::string function;

  int inputs;
  int* inputSize;
  char** inputType;

  int groups,items;

 public:
  kernel();
  kernel(device*,std::string);
  kernel(device*,std::string,std::string);
  ~kernel();
  void setup(device*,std::string);
  void getKernelInformation(std::string);
  int sizeofType(std::string);
  void setArgs(void*,...);
  void setArg(int,void*);
  void setDims(size_t,size_t);
  void run();
  void run(size_t,size_t);
  cl_kernel getKernel();
  void setKernel(cl_kernel);
  cl_program getProgram();
  void setProgram(cl_program);
  std::string getName();
  void setName(std::string);
  std::string getFunction();
  void setFunction(std::string);
  std::string getFlags();
  void setFlags(std::string);  
};

class device{
 private:
  cl_platform_id p;
  cl_device_id   d;  

  cl_context c;
  cl_command_queue cq;

 public:
  device();
  device(cl_platform_id,cl_device_id);
  ~device();
  void refresh();
  ocl_mem malloc(size_t);
  ocl_mem malloc(size_t,cl_mem_flags);
  void finish();
  void flush();
  cl_platform_id* getPlatform();
  void setPlatform(cl_platform_id);
  cl_device_id* getDevice();
  void setDevice(cl_device_id);
  cl_context* getContext();
  void setContext(cl_context);
  cl_command_queue* getCommandQueue();
  void setCommandQueue(cl_command_queue);
};

class ocl_mem{
 private:
  device* d;
  cl_mem m;
  size_t s;

 public:
  ocl_mem();
  ocl_mem(device*,cl_mem);
  ocl_mem(device*,cl_mem,size_t);
  ~ocl_mem();
  void copyTo(void*);
  void copyTo(void*,size_t,size_t);
  void copyToNB(void*);
  void copyToNB(void*,size_t,size_t);
  void copyFrom(void*);
  void copyFrom(void*,size_t,size_t);
  void copyFromNB(void*);
  void copyFromNB(void*,size_t,size_t);
  cl_mem* mem();
  size_t getSize();
};

#endif
