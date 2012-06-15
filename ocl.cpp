//--------------------------------------------------------------------//
//   Copyright (c) 2012, David Medina                                 //
//   All rights reserved.                                             //
//   License Located in:                                              //
//      https://github.com/dmed256/OCL-Library/blob/master/LICENSE    //
//--------------------------------------------------------------------//

#ifndef OCL_SETUP_C
#define OCL_SETUP_C

#ifndef OCL_OUTPUT_BUILD_LOG
#define OCL_OUTPUT_BUILD_LOG 0
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <stdarg.h>
#include <CL/cl.h>
#include "ocl.h"
#include "oclInfo.h"

using namespace std;

//-----------------------------//
//          OCL_SETUP          //
//-----------------------------//
ocl_setup::ocl_setup(){
  allocs = new int[1];
  allocs[0] = 1;

  shortInfo = "";
  longInfo = "";

  findDevices();

  question1 = "Print Detailed Information?\n"\
              "\t    i j - Platform i, Device j\n"\
              "\t[Other] - Shows Detailed Information\n"\
              "Input: ";
  question2 = "    i j - Platform i, Device j\n"\
              "Input: ";
}

ocl_setup::ocl_setup(const ocl_setup& s){
  *this = s;
}

ocl_setup::~ocl_setup(){
  if(allocs[0] > 1)
    allocs[0]--;
  else{
    delete[] allocs;
    destructor();
  }
}

ocl_setup& ocl_setup::operator=(const ocl_setup& s){
  copyCheck(s.allocs);
  shortInfo = s.shortInfo;
  longInfo  = s.longInfo;
  question1 = s.question1;
  question2 = s.question2;
  pID = s.pID;
  dID = s.dID;
  pSize = s.pSize;
  dSize = s.dSize;
  err = s.err;
  return *this;
}

void ocl_setup::destructor(){
  delete[] pID;
  for(int i=0;i<pSize;i++)
    delete[] dID[i];
  delete[] dID;
  delete[] dSize;

}

void ocl_setup::copyCheck(int* allocs2){
  if(allocs == NULL)
    allocs = allocs2;
  else if(allocs[0] > 1){
    allocs[0]--;
    allocs = allocs2;
  }
  else{
    delete[] allocs;
    allocs = allocs2;
    destructor();
  }
  allocs[0]++;
}

void ocl_setup::findDevices(){
  int maxP = 64,maxD = 64;
  cl_platform_id pID2[maxP];
  cl_device_id dID2[maxD];

  clGetPlatformIDs(maxP, pID2, &pSize);	
  pID = new cl_platform_id[pSize];
  dID = new cl_device_id*[pSize];
  dSize = new cl_uint[pSize];

  for(int i=0;i<pSize;i++){
    pID[i] = pID2[i];

    clGetDeviceIDs(pID[i],CL_DEVICE_TYPE_ALL, maxD, dID2, dSize+i);
    dID[i] = new cl_device_id[dSize[i]];
    for(int j=0;j<dSize[i];j++)
      dID[i][j] = dID2[j];
  }
}

void ocl_setup::findDeviceInformation(){
  int bSize = 8192;
  char buffer[bSize];
  cl_ulong buf_ulong;
  cl_uint buf_uint;
  stringstream str;
  cl_int err;
  
  string line = "";
  line.append(118,'-');
  line.append(1,'\n');
  string starLine = "\t";
  starLine.append(110,'*');
  starLine.append(1,'\n');

  str << line;
  for (int i=0;i<pSize;i++){
    str << "PLATFORM: " << i << '\n';
    ocl::printError("PLATFORM NAME",clGetPlatformInfo(pID[i], CL_PLATFORM_NAME , bSize, buffer, NULL));
    str << "\tPLATFORM_NAME       : " << buffer << '\n';
    
    ocl::printError("PLATFORM VERSION",clGetPlatformInfo(pID[i], CL_PLATFORM_VERSION, bSize, buffer, NULL));
    str << "\tPLATFORM_VERSION    : " << buffer << '\n';
    str << starLine;

    for(int j=0;j<dSize[i];j++){
      str << "\tDEVICE: " << j << '\n';

      ocl::printError("DEVICE_NAME",clGetDeviceInfo(dID[i][j], CL_DEVICE_NAME, bSize, buffer, NULL));
      str << "\t\tDEVICE_NAME                        : " << buffer << '\n';

      ocl::printError("DEVICE_VENDOR",clGetDeviceInfo(dID[i][j], CL_DEVICE_VENDOR, bSize, buffer, NULL));
      str << "\t\tDEVICE_VENDOR                      : " << buffer << '\n';

      ocl::printError("DEVICE_VERSION",clGetDeviceInfo(dID[i][j], CL_DEVICE_VERSION, bSize, buffer, NULL));
      str << "\t\tDEVICE_VERSION                     : " << buffer << '\n';
      str << starLine;
    }
    str << line;
  }

  shortInfo = str.str();

  str.str("");

  for(int i=0;i<pSize;i++){    
    str << line;
    str << "PLATFORM            : " << i << '\n';
    
    ocl::printError("PLATFORM PROFILE",clGetPlatformInfo(pID[i], CL_PLATFORM_PROFILE, bSize, buffer, NULL));
    str << "PLATFORM_PROFILE    : " << buffer << '\n';
    
    ocl::printError("PLATFORM VERSION",clGetPlatformInfo(pID[i], CL_PLATFORM_VERSION, bSize, buffer, NULL));
    str << "PLATFORM_VERSION    : " << buffer << '\n';
    
    ocl::printError("PLATFORM NAME",clGetPlatformInfo(pID[i], CL_PLATFORM_NAME , bSize, buffer, NULL));
    str << "PLATFORM_NAME       : " << buffer << '\n';
    
    ocl::printError("PLATFORM VENDOR",clGetPlatformInfo(pID[i], CL_PLATFORM_VENDOR , bSize, buffer, NULL));
    str << "PLATFORM_VENDOR     : " << buffer << '\n';
    
    ocl::printError("PLATFORM EXTENSIONS",clGetPlatformInfo(pID[i], CL_PLATFORM_EXTENSIONS, bSize, buffer, NULL));
    str << "PLATFORM_EXTENSIONS : " << buffer << "\n\n";

    str << starLine;
      
    for(int j=0;j<dSize[i];j++){
      str << "\tDevice                             : " << j << endl;

      ocl::printError("DEVICE_NAME",clGetDeviceInfo(dID[i][j], CL_DEVICE_NAME, bSize, buffer, NULL));
      str << "\tDEVICE_NAME                        : " <<  buffer << endl;

      ocl::printError("DEVICE_VENDOR",clGetDeviceInfo(dID[i][j], CL_DEVICE_VENDOR, bSize, buffer, NULL));
      str << "\tDEVICE_VENDOR                      : " <<  buffer << endl;

      ocl::printError("DEVICE_VERSION",clGetDeviceInfo(dID[i][j], CL_DEVICE_VERSION, bSize, buffer, NULL));
      str << "\tDEVICE_VERSION                     : " <<  buffer << endl;

      ocl::printError("DRIVER_VERSION",clGetDeviceInfo(dID[i][j], CL_DRIVER_VERSION, bSize, buffer, NULL));
      str << "\tDRIVER_VERSION                     : " <<  buffer << endl;

      ocl::printError("DEVICE_MAX_COMPUTE_UNITS",
		 clGetDeviceInfo(dID[i][j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buf_uint), &buf_uint, NULL));
      str << "\tDEVICE_MAX_COMPUTE_UNITS           : " << (unsigned int)buf_uint << endl;

      ocl::printError("DEVICE_MAX_CLOCK_FREQUENCY",
		 clGetDeviceInfo(dID[i][j], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buf_uint), &buf_uint, NULL));
      str << "\tDEVICE_MAX_CLOCK_FREQUENCY         : " <<  (unsigned int)buf_uint << endl;

      ocl::printError("DEVICE_GLOBAL_MEM_SIZE",
		 clGetDeviceInfo(dID[i][j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, NULL));
      str << "\tDEVICE_GLOBAL_MEM_SIZE             : " <<  (unsigned long long)buf_ulong << endl;

      ocl::printError("DEVICE_MAX_WORK_GROUP_SIZE",
		 clGetDeviceInfo(dID[i][j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(buf_ulong), &buf_ulong, NULL));
      str << "\tDEVICE_MAX_WORK_GROUP_SIZE         : " <<  (unsigned long long)buf_ulong << endl;

      size_t workitem_dims;
      ocl::printError("GET_MAX_ITEM_DIMENSIONS",
		 clGetDeviceInfo(dID[i][j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(workitem_dims), &workitem_dims, NULL));
      str << "\tCL_DEVICE_MAX_WORK_ITEM_DIMENSIONS : " <<  (unsigned int) workitem_dims << endl;
	    
      size_t workitem_size[3];
      ocl::printError("GET_MAX_WORK_ITEM_SIZES",
		 clGetDeviceInfo(dID[i][j], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(workitem_size), &workitem_size, NULL));
      str << "\tCL_DEVICE_MAX_WORK_ITEM_SIZES      : " 
	  << (int) workitem_size[0] << " / " 
	  << (int) workitem_size[1] << " / " 
	  << (int) workitem_size[2] << endl;
      str << starLine;
    }
  }
  str << line;

  longInfo = str.str();
}

ocl_device ocl_setup::displayDevices(){  
  if(shortInfo.empty())
    findDeviceInformation();

  char buffer[8192];
  int di = 1,dj = 3;
  int i,j;

  cout << shortInfo;  

  printf(question1.c_str(),di,dj);
  gets(buffer);

  if(sscanf(buffer,"%d %d",&i,&j) == 2)
    return ocl_device(pID[i],dID[i][j]);

  cout << longInfo;
  printf(question2.c_str(),di,dj);
  gets(buffer);

  while(sscanf(buffer,"%d %d",&i,&j) != 2){
    cout << "Wrong Input, please try again\n";
    printf(question2.c_str(),di,dj);
    gets(buffer);
  }

  return ocl_device(pID[i],dID[i][j]);
}

ocl_device ocl_setup::getDevice(int p,int d){
  return ocl_device(pID[p],dID[p][d]);
}

cl_platform_id ocl_setup::getPlatformID(int p){
  return pID[p];
}

cl_device_id ocl_setup::getDeviceID(int p,int d){
  return dID[p][d];
} 

//-----------------------------//
//          OCL_KERNEL          //
//-----------------------------//

ocl_kernel::ocl_kernel(){
  allocs = NULL;
  inputSize = NULL;
}

ocl_kernel::ocl_kernel(const ocl_kernel& k){
  *this = k;
}

ocl_kernel::ocl_kernel(ocl_device* d,string str){
  allocs = new int[1];
  allocs[0] = 1;

  setup(d,str);
}

ocl_kernel::ocl_kernel(ocl_device* d,string str,string fstr){
  allocs = new int[1];
  allocs[0] = 1;
  
  flags = fstr;
  setup(d,str);
}

ocl_kernel::~ocl_kernel(){
  if(allocs[0] > 1)
    allocs[0]--;
  else{
    delete[] allocs;
    destructor();
  }
}

ocl_kernel& ocl_kernel::operator=(const ocl_kernel& k){
  copyCheck(k.allocs);
  device = k.device;
  flags = k.flags;
  kernel = k.kernel;
  program = k.program;
  name = k.name;
  function = k.function;
  inputs = k.inputs;

  if(inputSize == NULL)
    inputSize = new int[inputs];
  for(int i=0;i<inputs;i++)
    inputSize[i] = k.inputSize[i];

  inputType = k.inputType;
  groups = k.groups;
  items = k.items;
  return *this;
}

void ocl_kernel::destructor(){
  if(function.compare("")){
    delete[] inputSize;
    clReleaseKernel(kernel);
    clReleaseProgram(program);
  }
}

void ocl_kernel::copyCheck(int* allocs2){
  if(allocs == NULL)
    allocs = allocs2;
  else if(allocs[0] > 1){
    allocs[0]--;
    allocs = allocs2;
  }
  else{
    delete[] allocs;
    allocs = allocs2;
    destructor();
  }
  allocs[0]++;
}

void ocl_kernel::setup(ocl_device* d,string str){
  device = d;
  cl_int err;

  if(ifstream(str.c_str())){
    ifstream file(str.c_str());

    file.seekg(0,ios::end);
    int length = file.tellg();
    file.seekg(0,ios::beg);
  
    char* tmp = new char[length];
    file.read(tmp,length);

    function = tmp;
    delete[] tmp;
  }
  else
    function = str;

  getKernelInformation(function);

  const char* cFunction = function.c_str();
  const size_t cLength = function.length();
  cl_device_id dID = device->getDeviceID();
  
  program = clCreateProgramWithSource(device->getContext(),1,&cFunction,&cLength,&err);
  ocl::printError("OCL_Kernel ("+name+") : Constructing Program",err);

  err = clBuildProgram(program,1,&dID,flags.c_str(),NULL,NULL);
  ocl::printError("OCL_Kernel ("+name+") : Building Program",err);

#if OCL_OUTPUT_BUILD_LOG
  char* log;
  size_t logSize;

  err = clGetProgramBuildInfo(program, device->getDeviceID(), CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);    
  log = new char[logSize];
  err = clGetProgramBuildInfo(program, device->getDeviceID(), CL_PROGRAM_BUILD_LOG, logSize, log, NULL);  
  log[logSize] = '\0';

  cout << "Build Log:\n\t" << log;

  delete[] log;
#endif

  kernel = clCreateKernel(program,name.c_str(),&err);
  ocl::printError("OCL_Kernel : Creating Kernel",err);
}

void ocl_kernel::getKernelInformation(string str){
  int start = str.find("__kernel",0) + 9;
  int end;

  if(start < 9){
    try{
      throw 1;
    }
    catch(int i){
      cout << "OCL_Kernel : Getting Kernel Information Error\n";
    }
  }
  
  string dm = " \t\n(*";

  while(dm.find(str[start],0) == string::npos)
    start++;
  while(dm.find(str[start],0) != string::npos)
    start++;
  end = start;
  while(dm.find(str[end],0) == string::npos)
    end++;

  name = str.substr(start,end-start);

  start = end;
  inputs = 0;

  while(str[end] - ')'){
    if(str[end] == ',')
      inputs++;
    end++;
  }
  inputs++;
  
  int* pointer = new int[inputs]();
  inputType.resize(inputs);
  inputSize = new int[inputs];
  int pos = start+1;

  for(int i=0;i<inputs;i++){
    while(str[pos] != ',' && str[pos] != ')'){
      if(str[pos] == '*')
	pointer[i]++;
      pos++;
    }
    inputSize[i] = pos++;    

    while(dm.find(str[inputSize[i]],0) != string::npos)
      inputSize[i]--;
    while(dm.find(str[inputSize[i]],0) == string::npos)
      inputSize[i]--;
    while(dm.find(str[inputSize[i]],0) != string::npos)
      inputSize[i]--;    
  }

  for(int i=0;i<inputs;i++){
    if(pointer[i]){
      inputType[i] = ocl::type[6];
      inputSize[i] = sizeofType("cl_mem");
    }
    else{
      pos = inputSize[i];

      while(dm.find(str[pos],0) == string::npos){
	pos--;
      }      
      
      inputType[i] = str.substr(pos+1,inputSize[i]-pos);
      inputSize[i] = sizeofType(inputType[i]);
    }
  }

  delete[] pointer;
}

int ocl_kernel::sizeofType(string type){
  int left = 0;
  int right = ocl::types-1;
  int current,check;

  while(0 <= left && right < ocl::types){
    current = (left + right)/2;
    check = type.compare(ocl::type[current]);
    
    if(!check)
      return ocl::typeSize[current];
    else if(0 < check)
      left = current;
    else
      right = current;

    if(current == (left+right)/2){
      try{
	throw 1;
      }
      catch(int i){
	cout << "OCL_Kernel ("+name+") : Type <" << type << "> not found in oclInfo\n";
      }
    }
  }
}

void ocl_kernel::setDims(size_t g,size_t i){
  groups = g;
  items = i;
}

void ocl_kernel::setArgs(void* x,...){
  va_list list;
  va_start(list,x);
  ocl::printError("OCL_Kernel ("+name+") : Setting Kernel Arguments",
  	     clSetKernelArg(kernel,0,inputSize[0],x));
  for(int i=1;i<inputs;i++)
    ocl::printError("OCL_Kernel ("+name+") : Setting Kernel Arguments",
  	       clSetKernelArg(kernel,i,inputSize[i],(void*) va_arg(list,void*)));

  va_end(list);
}

void ocl_kernel::setArg(int pos,void* arg){
  if(pos >= inputs || pos < 0)
    ocl::printError("OCL_Kernel ("+name+") : Incorrect Kernel Argument Position",15);
  ocl::printError("OCL_Kernel ("+name+") : Setting Kernel Arguments",
	     clSetKernelArg(kernel,pos,inputSize[pos],arg));
}

string ocl_kernel::getArgType(int pos){
  if(pos >= inputs || pos < 0)
    ocl::printError("OCL_Kernel ("+name+") : Incorrect Kernel Argument Position",15);    
  return inputType[pos];
}

void ocl_kernel::run(){
  const size_t a = items;
  const size_t b = groups;
  ocl::printError("OCL_Kernel ("+name+") : Kernel Run",
	     clEnqueueNDRangeKernel(device->getCommandQueue(),kernel,1,NULL,&a,&b,0,NULL,NULL));  
}

void ocl_kernel::run(size_t g,size_t i){
  groups = g;
  items = i;
  run();
}

cl_kernel ocl_kernel::getKernel(){
  return kernel;
}

cl_program ocl_kernel::getProgram(){
  return program;
}

string ocl_kernel::getName(){
  return name;
}

void ocl_kernel::setName(string n){
  name = n;
}

string ocl_kernel::getFunction(){
  return function;
}

void ocl_kernel::setFunction(string f){
  function = f;
}
 
string ocl_kernel::getFlags(){
  return flags;
}

void ocl_kernel::setFlags(string f){
  flags = f;
}  

int ocl_kernel::getWarpSize(){
  size_t ret;
  ocl::printError("OCL_DEVICE: Getting Warp Size",
	     clGetKernelWorkGroupInfo(kernel,device->getDeviceID(),CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
				      sizeof(size_t),&ret,NULL));
  return ret;
}

int ocl_kernel::getWavefrontSize(){
  return getWarpSize();
}

int ocl_kernel::getGroupSize(int p){
  return device->getGroupSize(p);
}

//-----------------------------//
//         OCL_DEVICE          //
//-----------------------------//
ocl_device::ocl_device(){
  allocs = NULL;
  pID = NULL;
  dID = NULL;
  groupSize = NULL;
}

ocl_device::ocl_device(const ocl_device& d){
  *this = d;
}

ocl_device::ocl_device(cl_platform_id p,cl_device_id d){
  allocs = new int[1];
  allocs[0] = 1;

  pID = p;
  dID = d;
  groupSize = new int[3];
  refresh();
}

ocl_device::~ocl_device(){
  if(allocs[0] > 1)
    allocs[0]--;
  else{
    delete[] allocs;
    destructor();
  }
}

ocl_device& ocl_device::operator=(const ocl_device& d){
  copyCheck(d.allocs);
  pID = d.pID; 
  dID = d.dID; 
  if(groupSize == NULL)
    groupSize = new int[3];

  context = d.context;
  commandQueue = d.commandQueue;
  groupSize[0] = d.groupSize[0];
  groupSize[1] = d.groupSize[1];
  groupSize[2] = d.groupSize[2];
  return *this;
}

void ocl_device::destructor(){
  if(groupSize != NULL)
    delete[] groupSize;
}

void ocl_device::copyCheck(int* allocs2){
  if(allocs == NULL)
    allocs = allocs2;
  else if(allocs[0] > 1){
    allocs[0]--;
    allocs = allocs2;
  }
  else{
    delete[] allocs;
    allocs = allocs2;
    destructor();
  }
  allocs[0]++;
}

void ocl_device::refresh(){
  context.create(&dID);
  commandQueue.create(context.getContext(),dID);
  size_t* tmp = new size_t[3];
  ocl::printError("GET_MAX_WORK_ITEM_SIZES",
	     clGetDeviceInfo(dID, CL_DEVICE_MAX_WORK_ITEM_SIZES, 3*sizeof(size_t), tmp, NULL));
  groupSize[0] = tmp[0];
  groupSize[1] = tmp[1];
  groupSize[2] = tmp[2];
  delete[] tmp;
}

ocl_mem ocl_device::malloc(size_t s){
  cl_int err;
  cl_mem mem = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE , s, NULL, &err);
  ocl::printError("OCL_Device: Malloc",err);
  return ocl_mem(this,mem,s);
}

ocl_mem ocl_device::malloc(size_t s,cl_mem_flags f){
  cl_int err;
  cl_mem mem = clCreateBuffer(context.getContext(), f, s, NULL, &err);
  ocl::printError("OCL_Device: Malloc",err);
  return ocl_mem(this,mem,s);
}

void ocl_device::barrier(){
  ///////////////////////////////////
  //ocl::printError("OCL_Device: Malloc",clEnqueueBarrier(*commandQueue));
}

void ocl_device::finish(){
  commandQueue.finish();
}

void ocl_device::flush(){
  commandQueue.flush();
}

cl_platform_id ocl_device::getPlatformID(){
  return pID;
}

cl_device_id ocl_device::getDeviceID(){
  return dID;
}

cl_context ocl_device::getContext(){
  return context.getContext();
}

cl_command_queue ocl_device::getCommandQueue(){
  return commandQueue.getCommandQueue();
}

int ocl_device::getGroupSize(int p){
  if(p < 0 || 2 < p || groupSize == NULL)
    ocl::printError("OCL_DEVICE: Getting group size",15);
  return groupSize[p];
}

//-----------------------------//
//         OCL_CONTEXT         //
//-----------------------------//
  
ocl_context::ocl_context(){
  allocs = NULL;
}

ocl_context::ocl_context(const ocl_context& c){
  *this = c;
}

ocl_context::~ocl_context(){
  if(allocs[0] > 1)
    allocs[0]--;
  else{
    delete[] allocs;
    destructor();
  }
}

ocl_context& ocl_context::operator=(const ocl_context& c){
  copyCheck(c.allocs);
  context = c.context;  
  return *this;
}

void ocl_context::destructor(){
  clReleaseContext(*context);
  delete[] context;
}

void ocl_context::copyCheck(int* allocs2){
  if(allocs == NULL)
    allocs = allocs2;
  else if(allocs[0] > 1){
    allocs[0]--;
    allocs = allocs2;
  }
  else{
    delete[] allocs;
    allocs = allocs2;
    destructor();
  }
  allocs[0]++;
}

void ocl_context::create(cl_device_id* dID){
  if(allocs == NULL){
    allocs = new int[1];
    allocs[0] = 1;
  }
  cl_int err;
  context = new cl_context[1];
  *context = clCreateContext(NULL,1,dID,NULL,NULL,&err);
  ocl::printError("OCL_Context: Creating Context",err);
}

cl_context ocl_context::getContext(){
  return *context;
}

//-----------------------------//
//      OCL_COMMANDQUEUE       //
//-----------------------------//
 
ocl_commandQueue::ocl_commandQueue(){
  allocs = NULL;
}

ocl_commandQueue::ocl_commandQueue(const ocl_commandQueue& cq){
  *this = cq;
}

ocl_commandQueue::~ocl_commandQueue(){
  if(allocs[0] > 1)
    allocs[0]--;
  else{
    delete[] allocs;
    destructor();
  }
}

ocl_commandQueue& ocl_commandQueue::operator=(const ocl_commandQueue& cq){
  copyCheck(cq.allocs);
  commandQueue = cq.commandQueue;  
  return *this;
}

void ocl_commandQueue::destructor(){
  clReleaseCommandQueue(*commandQueue);
  delete[] commandQueue;
}

void ocl_commandQueue::copyCheck(int* allocs2){
  if(allocs == NULL)
    allocs = allocs2;
  else if(allocs[0] > 1){
    allocs[0]--;
    allocs = allocs2;
  }
  else{
    delete[] allocs;
    allocs = allocs2;
    destructor();
  }
  allocs[0]++;
}

void ocl_commandQueue::create(cl_context context,cl_device_id dID){
  if(allocs == NULL){
    allocs = new int[1];
    allocs[0] = 1;
  }
  cl_int err;
  commandQueue = new cl_command_queue[1];
  *commandQueue = clCreateCommandQueue(context,dID,0,&err);
  ocl::printError("OCL_CommandQueue: Creating Command Queue",err);
}

void ocl_commandQueue::finish(){
  clFinish(*commandQueue);
}

void ocl_commandQueue::flush(){
  clFlush(*commandQueue);
}

cl_command_queue ocl_commandQueue::getCommandQueue(){
  return *commandQueue;
}

//-----------------------------//
//           OCL_MEM           //
//-----------------------------//

ocl_mem::ocl_mem(){
  allocs = NULL;
  device = NULL;
  memory = NULL;
  size   = -1;
}

ocl_mem::ocl_mem(const ocl_mem& m){
  *this = m;
}

ocl_mem::ocl_mem(ocl_device* d,cl_mem m,size_t s){
  allocs = new int[1];
  allocs[0] = 1;

  device = d;
  memory = m;
  size   = s;
}

ocl_mem::~ocl_mem(){
  if(allocs[0] > 1)
    allocs[0]--;
  else{
    delete[] allocs;
    destructor();
  }
}

ocl_mem& ocl_mem::operator=(const ocl_mem& m){
  copyCheck(m.allocs);
  device = m.device;
  memory = m.memory;
  size   = m.size;
  return *this;
}

void ocl_mem::destructor(){
  clReleaseMemObject(memory);
}

void ocl_mem::copyCheck(int* allocs2){
  if(allocs == NULL)
    allocs = allocs2;
  else if(allocs[0] > 1){
    allocs[0]--;
    allocs = allocs2;
  }
  else{
    delete[] allocs;
    allocs = allocs2;
    destructor();
  }
  allocs[0]++;
}

void ocl_mem::free(){
  if(size >= 0){
    delete[] allocs;
    clReleaseMemObject(memory);
    size = -1;
  }
}

void ocl_mem::copyTo(void* v){
  clEnqueueReadBuffer(device->getCommandQueue(), memory, CL_TRUE, 0, size, v,0,NULL,NULL);  
}

void ocl_mem::copyTo(void* v,size_t offset,size_t s){
  clEnqueueReadBuffer(device->getCommandQueue(), memory, CL_TRUE, offset, s, v,0,NULL,NULL);  
}

void ocl_mem::copyToNB(void* v){
  clEnqueueReadBuffer(device->getCommandQueue(), memory, CL_FALSE, 0, size, v,0,NULL,NULL);  
}

void ocl_mem::copyToNB(void* v,size_t offset,size_t s){
  clEnqueueReadBuffer(device->getCommandQueue(), memory, CL_FALSE, offset, s, v,0,NULL,NULL);  
}

void ocl_mem::copyFrom(void* v){
  clEnqueueWriteBuffer(device->getCommandQueue(), memory, CL_TRUE, 0, size, v, 0, NULL, NULL);
}

void ocl_mem::copyFrom(void* v,size_t offset,size_t s){
  clEnqueueWriteBuffer(device->getCommandQueue(), memory, CL_TRUE, offset, s, v,0,NULL,NULL);  
}

void ocl_mem::copyFromNB(void* v){
  clEnqueueWriteBuffer(device->getCommandQueue(), memory, CL_FALSE, 0, size, v, 0, NULL, NULL);
}

void ocl_mem::copyFromNB(void* v,size_t offset,size_t s){
  clEnqueueWriteBuffer(device->getCommandQueue(), memory, CL_FALSE, offset, s, v,0,NULL,NULL);  
}

cl_mem* ocl_mem::mem(){
  return &memory;
}

size_t ocl_mem::getSize(){
  return size;
}

#endif
