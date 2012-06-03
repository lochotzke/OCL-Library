//----------------------------------------------------//
// This is the OCL Library Source File, feel free to  //
//    use and edit for your own use. This is free     //
//    to the public so please give credit and post    //
//    it free for others to use as well.              //
//                                                    //
// David Medina                                       //
//----------------------------------------------------//

#ifndef OCL_SETUP_C
#define OCL_SETUP_C

/* 
#define OCL_OUTPUT_BUILD_LOG 1 
   to output the build log, otherwise the build log is neglected

Note:
   Valgrind shows memory leaks due to the follwing OpenCL functions hiding memory movement:
   device::refresh() from either of the following:
         clCreateContext()
	 clCreateCommandQueue()
   kernel::setup() from:
         clCreateProgramWithSource()
	 clBuildProgram()
	 clCreateKernel()
   ocSetup::findDevices() from either of the following:
         clGetPlatformIDs()
	 clGetDeviceIDs()
 */

#ifndef OCL_OUTPUT_BUILD_LOG
#define OCL_OUTPUT_BUILD_LOG 0
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdarg.h>
#include <CL/cl.h>
#include "ocl.h"
#include "oclInfo.h"

using namespace std;

//-----------------------------//
//           OCSETUP           //
//-----------------------------//
ocSetup::ocSetup(){
  shortInfo = "";
  longInfo = "";

  findDevices();

  question1 = "Print Detailed Information?\n"\
              "\t    i j - Platform i, Device j\n"\
              "\t[Enter] - Platform \%d, Device \%d\n"\
              "\t[Other] - Shows Detailed Information\n"\
              "Input: ";
  question2 = "    i j - Platform i, Device j\n"\
              "[Other] - Platform \%d, Device \%d\n"\
              "Input: ";
}

ocSetup::~ocSetup(){
  free(pID);
  for(int i=0;i<pSize;i++)
    free(dID[i]);
  free(dID);
  free(dSize);
}

void ocSetup::findDevices(){
  int maxP = 64,maxD = 64;
  cl_platform_id pID2[maxP];
  cl_device_id dID2[maxD];

  clGetPlatformIDs(maxP, pID2, &pSize);	
  pID = (cl_platform_id*) malloc(pSize*sizeof(cl_platform_id));
  dID = (cl_device_id**)  malloc(pSize*sizeof(cl_device_id*));
  dSize = (cl_uint*)      malloc(pSize*sizeof(cl_uint));

  for(int i=0;i<pSize;i++){
    pID[i] = pID2[i];

    clGetDeviceIDs(pID[i],CL_DEVICE_TYPE_ALL, maxD, dID2, dSize+i);
    dID[i] = (cl_device_id*) malloc(dSize[i]*sizeof(cl_device_id));
    for(int j=0;j<dSize[i];j++)
      dID[i][j] = dID2[j];
  }
}

void ocSetup::findDeviceInformation(){
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
    printError("PLATFORM NAME",clGetPlatformInfo(pID[i], CL_PLATFORM_NAME , bSize, buffer, NULL));
    str << "\tPLATFORM_NAME       : " << buffer << '\n';
    
    printError("PLATFORM VERSION",clGetPlatformInfo(pID[i], CL_PLATFORM_VERSION, bSize, buffer, NULL));
    str << "\tPLATFORM_VERSION    : " << buffer << '\n';
    str << starLine;

    for(int j=0;j<dSize[i];j++){
      str << "\tDEVICE: " << j << '\n';

      printError("DEVICE_NAME",clGetDeviceInfo(dID[i][j], CL_DEVICE_NAME, bSize, buffer, NULL));
      str << "\t\tDEVICE_NAME                        : " << buffer << '\n';

      printError("DEVICE_VENDOR",clGetDeviceInfo(dID[i][j], CL_DEVICE_VENDOR, bSize, buffer, NULL));
      str << "\t\tDEVICE_VENDOR                      : " << buffer << '\n';

      printError("DEVICE_VERSION",clGetDeviceInfo(dID[i][j], CL_DEVICE_VERSION, bSize, buffer, NULL));
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
    
    printError("PLATFORM PROFILE",clGetPlatformInfo(pID[i], CL_PLATFORM_PROFILE, bSize, buffer, NULL));
    str << "PLATFORM_PROFILE    : " << buffer << '\n';
    
    printError("PLATFORM VERSION",clGetPlatformInfo(pID[i], CL_PLATFORM_VERSION, bSize, buffer, NULL));
    str << "PLATFORM_VERSION    : " << buffer << '\n';
    
    printError("PLATFORM NAME",clGetPlatformInfo(pID[i], CL_PLATFORM_NAME , bSize, buffer, NULL));
    str << "PLATFORM_NAME       : " << buffer << '\n';
    
    printError("PLATFORM VENDOR",clGetPlatformInfo(pID[i], CL_PLATFORM_VENDOR , bSize, buffer, NULL));
    str << "PLATFORM_VENDOR     : " << buffer << '\n';
    
    printError("PLATFORM EXTENSIONS",clGetPlatformInfo(pID[i], CL_PLATFORM_EXTENSIONS, bSize, buffer, NULL));
    str << "PLATFORM_EXTENSIONS : " << buffer << "\n\n";

    str << starLine;
      
    for(int j=0;j<dSize[i];j++){
      str << "\tDevice                             : " << j << endl;

      printError("DEVICE_NAME",clGetDeviceInfo(dID[i][j], CL_DEVICE_NAME, bSize, buffer, NULL));
      str << "\tDEVICE_NAME                        : " <<  buffer << endl;

      printError("DEVICE_VENDOR",clGetDeviceInfo(dID[i][j], CL_DEVICE_VENDOR, bSize, buffer, NULL));
      str << "\tDEVICE_VENDOR                      : " <<  buffer << endl;

      printError("DEVICE_VERSION",clGetDeviceInfo(dID[i][j], CL_DEVICE_VERSION, bSize, buffer, NULL));
      str << "\tDEVICE_VERSION                     : " <<  buffer << endl;

      printError("DRIVER_VERSION",clGetDeviceInfo(dID[i][j], CL_DRIVER_VERSION, bSize, buffer, NULL));
      str << "\tDRIVER_VERSION                     : " <<  buffer << endl;

      printError("DEVICE_MAX_COMPUTE_UNITS",
		 clGetDeviceInfo(dID[i][j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buf_uint), &buf_uint, NULL));
      str << "\tDEVICE_MAX_COMPUTE_UNITS           : " << (unsigned int)buf_uint << endl;

      printError("DEVICE_MAX_CLOCK_FREQUENCY",
		 clGetDeviceInfo(dID[i][j], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buf_uint), &buf_uint, NULL));
      str << "\tDEVICE_MAX_CLOCK_FREQUENCY         : " <<  (unsigned int)buf_uint << endl;

      printError("DEVICE_GLOBAL_MEM_SIZE",
		 clGetDeviceInfo(dID[i][j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, NULL));
      str << "\tDEVICE_GLOBAL_MEM_SIZE             : " <<  (unsigned long long)buf_ulong << endl;

      printError("DEVICE_MAX_WORK_GROUP_SIZE",
		 clGetDeviceInfo(dID[i][j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(buf_ulong), &buf_ulong, NULL));
      str << "\tDEVICE_MAX_WORK_GROUP_SIZE         : " <<  (unsigned long long)buf_ulong << endl;

      size_t workitem_dims;
      printError("GET_MAX_ITEM_DIMENSIONS",
		 clGetDeviceInfo(dID[i][j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(workitem_dims), &workitem_dims, NULL));
      str << "\tCL_DEVICE_MAX_WORK_ITEM_DIMENSIONS : " <<  (unsigned int) workitem_dims << endl;
	    
      size_t workitem_size[3];
      printError("GET_MAX_WORK_ITEM_SIZES",
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

device ocSetup::displayDevices(){  
  if(shortInfo.empty())
    findDeviceInformation();

  char buffer[8192];
  int di = 1,dj = 3;
  int i,j;

  cout << shortInfo;  

  printf(question1.c_str(),di,dj);
  gets(buffer);

  if(buffer[0] == '\0')
    return device(pID[di],dID[di][dj]);
  else if(sscanf(buffer,"%d %d",&i,&j) == 2)
    return device(pID[i],dID[i][j]);

  cout << longInfo;
  printf(question2.c_str(),di,dj);
  gets(buffer);

  if(sscanf(buffer,"%d %d",&i,&j) == 2)
    return device(pID[i],dID[i][j]);
  else
    return device(pID[di],dID[di][dj]);
}

device ocSetup::getDevice(int p,int d){
  return device(pID[p],dID[p][d]);
}

//-----------------------------//
//           KERNEL            //
//-----------------------------//

kernel::kernel(){}

kernel::kernel(device* d,string str){
  setup(d,str);
}

kernel::kernel(device* d,string str,string fstr){
  flags = fstr;
  setup(d,str);
}

kernel::~kernel(){
  for(int i=0;i<inputs;i++)    
    free(inputType[i]); 
  free(inputType);
  free(inputSize);
  clReleaseKernel(ker);
  clReleaseProgram(program);
}

void kernel::setup(device* d,string str){
  dev = d;
  cl_int err;

  if(ifstream(str.c_str())){
    ifstream file(str.c_str());

    file.seekg(0,ios::end);
    int length = file.tellg();
    file.seekg(0,ios::beg);
  
    char* tmp = (char*) malloc(length*sizeof(char));
    file.read(tmp,length);

    function = tmp;
    free(tmp);
  }
  else
    function = str;

  getKernelInformation(function);

  const char* cFunction = function.c_str();
  const size_t cLength = function.length();

  program = clCreateProgramWithSource(*(dev->getContext()),1,&cFunction,&cLength,&err);
  printError("Kernel: Constructing Program",err);

  err = clBuildProgram(program,1,dev->getDevice(),flags.c_str(),NULL,NULL);
  printError("Kernel: Building Program",err);

  #if OCL_OUTPUT_BUILD_LOG
  char* log;
  size_t logSize;

  err = clGetProgramBuildInfo(program, *(dev->getDevice()), CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);    
  log = (char*) malloc(logSize);
  err = clGetProgramBuildInfo(program, *(dev->getDevice()), CL_PROGRAM_BUILD_LOG, logSize, log, NULL);  
  log[logSize] = '\0';

  if(logSize > 2)
    cout << "Build Log:\n\t" << log;

  free(log);
  #endif

  ker = clCreateKernel(program,name.c_str(),&err);
  printError("Kernel: Creating Kernel",err);
}

void kernel::getKernelInformation(string str){
  int start = str.find("__kernel",0) + 9;
  int end;

  if(start < 9){
    cout << "Kernel: Getting Kernel Information Error\n";
    exit(1);
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
  
  int* pointer = (int*) calloc(inputs,sizeof(int));
  inputType = (char**) malloc(inputs*sizeof(char*));
  inputSize = (int*) malloc(inputs*sizeof(int));
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
      inputType[i] = (char*) malloc(6*sizeof(char));
      (ocl::type[6]).copy(inputType[i],6,0);
      inputType[i][6] = '\0';

      inputSize[i] = sizeofType("cl_mem");
    }
    else{
      pos = inputSize[i];

      while(dm.find(str[pos],0) == string::npos)
	pos--;

      inputType[i] = (char*) malloc((inputSize[i]-pos)*sizeof(char));

      str.copy(inputType[i],inputSize[i]-pos,pos+1);
      inputType[i][inputSize[i]-pos] = '\0';

      inputSize[i] = sizeofType(inputType[i]);
    }
  }

  free(pointer);
}

int kernel::sizeofType(string type){
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
	cout << "Kernel: Type <" << type << "> not found in oclInfo\n";
	exit(1);
    }
  }
}

void kernel::setDims(size_t g,size_t i){
  groups = g;
  items = i;
}

void kernel::setArgs(void* x,...){
  va_list list;
  va_start(list,x);
  
  printError("Kernel: Setting Kernel Arguments",
  	     clSetKernelArg(ker,0,inputSize[0],x));
  for(int i=1;i<inputs;i++)
    printError("Kernel: Setting Kernel Arguments",
  	       clSetKernelArg(ker,i,inputSize[i],(void*) va_arg(list,void*)));

  va_end(list);
}

void kernel::setArg(int pos,void* arg){
  if(pos >= inputs || pos < 0)
    printError("Kernel: Incorrect Kernel Argument Position",15);
  printError("Kernel: Setting Kernel Arguments",
	     clSetKernelArg(ker,pos,inputSize[pos],arg));
}

void kernel::run(){
  const size_t a = items;
  const size_t b = groups;
  printError("Kernel: Kernel Run",
	     clEnqueueNDRangeKernel(*(dev->getCommandQueue()),ker,1,NULL,&a,&b,0,NULL,NULL));  
}

void kernel::run(size_t g,size_t i){
  groups = g;
  items = i;
  run();
}

cl_kernel kernel::getKernel(){
  return ker;
}

void kernel::setKernel(cl_kernel k){
  ker = k;
}

cl_program kernel::getProgram(){
  return program;
}

void kernel::setProgram(cl_program p){
  program = p;
}

string kernel::getName(){
  return name;
}

void kernel::setName(string n){
  name = n;
}

string kernel::getFunction(){
  return function;
}

void kernel::setFunction(string f){
  function = f;
}
 
string kernel::getFlags(){
  return flags;
}

void kernel::setFlags(string f){
  flags = f;
}  

//-----------------------------//
//           DEVICE            //
//-----------------------------//
device::device(){
  p = NULL;
  d = NULL;
  c = NULL;
  cq = NULL;
}

device::device(cl_platform_id p2,cl_device_id d2){
  p = p2; 
  d = d2;
  refresh();
}

device::~device(){
  clReleaseCommandQueue(cq);
  clReleaseContext(c);
}

void device::refresh(){
  cl_int err;
  c = clCreateContext(NULL,1,&d,NULL,NULL,&err);
  printError("Device: Creating Context",err);
  cq = clCreateCommandQueue(c,d,0,&err);
  printError("Device: Creating Command Queue",err);
}

ocl_mem device::malloc(size_t s){
  cl_int err;
  cl_mem mem = clCreateBuffer(c, CL_MEM_READ_WRITE , s, NULL, &err);
  printError("Device: Malloc",err);
  return ocl_mem(this,mem,s);
}

ocl_mem device::malloc(size_t s,cl_mem_flags f){
  cl_int err;
  cl_mem mem = clCreateBuffer(c, f, s, NULL, &err);
  printError("Device: Malloc",err);
  return ocl_mem(this,mem,s);
}

void device::finish(){
  clFinish(cq);
}

void device::flush(){
  clFlush(cq);
}

cl_platform_id* device::getPlatform(){
  return &p;
}

void device::setPlatform(cl_platform_id p2){
  p = p2;
}

cl_device_id* device::getDevice(){
  return &d;
}

void device::setDevice(cl_device_id d2){
  d = d2;
}

cl_context* device::getContext(){
  return &c;
}

void device::setContext(cl_context c2){
  c = c2;
}

cl_command_queue* device::getCommandQueue(){
  return &cq;
}

void device::setCommandQueue(cl_command_queue cq2){
  cq = cq2;
}

//-----------------------------//
//           OCL_MEM           //
//-----------------------------//

ocl_mem::ocl_mem(){
  d = NULL;
  m = NULL;
  s = -1;
}

ocl_mem::ocl_mem(device* d2,cl_mem m2,size_t s2){
  d = d2;
  m = m2;
  s = s2;
}

ocl_mem::~ocl_mem(){
  if(s >= 0)
    clReleaseMemObject(m);
}

void ocl_mem::copyTo(void* v){
  clEnqueueReadBuffer(*(*d).getCommandQueue(), m, CL_TRUE, 0, s, v,0,NULL,NULL);  
}

void ocl_mem::copyTo(void* v,size_t offset,size_t size){
  clEnqueueReadBuffer(*(*d).getCommandQueue(), m, CL_TRUE, offset, size, v,0,NULL,NULL);  
}

void ocl_mem::copyToNB(void* v){
  clEnqueueReadBuffer(*(*d).getCommandQueue(), m, CL_FALSE, 0, s, v,0,NULL,NULL);  
}

void ocl_mem::copyToNB(void* v,size_t offset,size_t size){
  clEnqueueReadBuffer(*(*d).getCommandQueue(), m, CL_FALSE, offset, size, v,0,NULL,NULL);  
}

void ocl_mem::copyFrom(void* v){
  clEnqueueWriteBuffer(*(*d).getCommandQueue(), m , CL_TRUE, 0, s, v, 0, NULL, NULL);
}

void ocl_mem::copyFrom(void* v,size_t offset,size_t size){
  clEnqueueWriteBuffer(*(*d).getCommandQueue(), m, CL_TRUE, offset, size, v,0,NULL,NULL);  
}

void ocl_mem::copyFromNB(void* v){
  clEnqueueWriteBuffer(*(*d).getCommandQueue(), m , CL_FALSE, 0, s, v, 0, NULL, NULL);
}

void ocl_mem::copyFromNB(void* v,size_t offset,size_t size){
  clEnqueueWriteBuffer(*(*d).getCommandQueue(), m, CL_FALSE, offset, size, v,0,NULL,NULL);  
}

cl_mem* ocl_mem::mem(){
  return &m;
}

size_t ocl_mem::getSize(){
  return s;
}

#endif
