#ifndef OCL_SETUP_INFO
#define OCL_SETUP_INFO

#include <iostream>
#include <CL/cl.h>

// Opencl Variable Sizes
namespace ocl{
  const int types = 62;

  const std::string type[] = {
    "char"   , "char16", "char2"   , "char3"   , "char4"  , "char8"  ,
    "cl_mem" , "double", "double16", "double2" , "double3", "double4",
    "double8", "float" , "float16" , "float2"  , "float3" , "float4" , 
    "float8" , "half"  , "int"     , "int16"   , "int2"   , "int3"   ,
    "int4"   , "int8"  , "long"    , "long16"  , "long2"  , "long3"  ,
    "long4"  , "long8" , "short"   , "short16" , "short2" , "short3" ,
    "short4" , "short8", "uchar"   , "uchar16" , "uchar2" , "uchar3" ,
    "uchar4" , "uchar8", "uint"    , "uint16"  , "uint2"  , "uint3"  ,
    "uint4"  , "uint8" , "ulong"   , "ulong16" , "ulong2" , "ulong3" , 
    "ulong4" , "ulong8", "ushort"  , "ushort16", "ushort2", "ushort3", 
    "ushort4", "ushort8"
  };

  const int typeSize[] = {
    1 ,    16,    2  ,    4  ,    4 ,    8 ,
    8 ,    8 ,    128,    16 ,    32,    32,
    64,    4 ,    64 ,    8  ,    16,    16,
    32,    2 ,    4  ,    64 ,    8 ,    16,
    16,    32,    8  ,    128,    16,    32,
    32,    64,    2  ,    32 ,    4 ,    8 ,
    8 ,    16,    1  ,    16 ,    2 ,    4 ,
    4 ,    8 ,    4  ,    64 ,    8 ,    16,
    16,    32,    8  ,    128,    16,    32,
    32,    64,    2  ,    32 ,    4 ,    8 ,
    8 ,    16
  };

  const char error[65][45] = {
    "CL_SUCCESS"                        ,"CL_DEVICE_NOT_FOUND"             ,"CL_DEVICE_NOT_AVAILABLE",
    "CL_COMPILER_NOT_AVAILABLE"         ,"CL_MEM_OBJECT_ALLOCATION_FAILURE","CL_OUT_OF_RESOURCES",
    "CL_OUT_OF_HOST_MEMORY"             ,"CL_PROFILING_INFO_NOT_AVAILABLE" ,"CL_MEM_COPY_OVERLAP",
    "CL_IMAGE_FORMAT_MISMATCH"          ,"CL_IMAGE_FORMAT_NOT_SUPPORTED"   ,"CL_BUILD_PROGRAM_FAILURE",
    "CL_MAP_FAILURE"                    ,"CL_MISALIGNED_SUB_BUFFER_OFFSET" ,"CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST",
    "N/A"                               ,"N/A"                             ,"N/A",
    "N/A"                               ,"N/A"                             ,"N/A",
    "N/A"                               ,"N/A"                             ,"N/A",
    "N/A"                               ,"N/A"                             ,"N/A",
    "N/A"                               ,"N/A"                             ,"N/A",
    "CL_INVALID_VALUE"                  ,"CL_INVALID_DEVICE_TYPE"          ,"CL_INVALID_PLATFORM",
    "CL_INVALID_DEVICE"                 ,"CL_INVALID_CONTEXT"              ,"CL_INVALID_QUEUE_PROPERTIES",
    "CL_INVALID_COMMAND_QUEUE"          ,"CL_INVALID_HOST_PTR"             ,"CL_INVALID_MEM_OBJECT",
    "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR","CL_INVALID_IMAGE_SIZE"           ,"CL_INVALID_SAMPLER",
    "CL_INVALID_BINARY"                 ,"CL_INVALID_BUILD_OPTIONS"        ,"CL_INVALID_PROGRAM",
    "CL_INVALID_PROGRAM_EXECUTABLE"     ,"CL_INVALID_KERNEL_NAME"          ,"CL_INVALID_KERNEL_DEFINITION",
    "CL_INVALID_KERNEL"                 ,"CL_INVALID_ARG_INDEX"            ,"CL_INVALID_ARG_VALUE",
    "CL_INVALID_ARG_SIZE"               ,"CL_INVALID_KERNEL_ARGS"          ,"CL_INVALID_WORK_DIMENSION",
    "CL_INVALID_WORK_GROUP_SIZE"        ,"CL_INVALID_WORK_ITEM_SIZE"       ,"CL_INVALID_GLOBAL_OFFSET",
    "CL_INVALID_EVENT_WAIT_LIST"        ,"CL_INVALID_EVENT"                ,"CL_INVALID_OPERATION",
    "CL_INVALID_GL_OBJECT"              ,"CL_INVALID_BUFFER_SIZE"          ,"CL_INVALID_MIP_LEVEL",
    "CL_INVALID_GLOBAL_WORK_SIZE"       ,"CL_INVALID_PROPERTY"
  };
};

void printError(std::string s,int error){
  if(error){
    error = error < 0  ? error : -error;
    error = error < 65 ? error : 15;
    std::cout << s << " Error: " << error << '\n'
	      << "OpenCL Error: " << ocl::error[-error];
    exit(1);
  }
}

void printSizes(){
  std::cout << "sizeof(char) = " << sizeof(cl_char) << std::endl
	    << "sizeof(char16) = " << sizeof(cl_char16) << std::endl
	    << "sizeof(char2) = " << sizeof(cl_char2) << std::endl
	    << "sizeof(char3) = " << sizeof(cl_char3) << std::endl
	    << "sizeof(char4) = " << sizeof(cl_char4) << std::endl
	    << "sizeof(char8) = " << sizeof(cl_char8) << std::endl
	    << "sizeof(cl_mem) = " << sizeof(cl_mem) << std::endl
	    << "sizeof(double) = " << sizeof(cl_double) << std::endl
	    << "sizeof(double16) = " << sizeof(cl_double16) << std::endl
	    << "sizeof(double2) = " << sizeof(cl_double2) << std::endl
	    << "sizeof(double3) = " << sizeof(cl_double3) << std::endl
	    << "sizeof(double4) = " << sizeof(cl_double4) << std::endl
	    << "sizeof(double8) = " << sizeof(cl_double8) << std::endl
	    << "sizeof(float) = " << sizeof(cl_float) << std::endl
	    << "sizeof(float16) = " << sizeof(cl_float16) << std::endl
	    << "sizeof(float2) = " << sizeof(cl_float2) << std::endl
	    << "sizeof(float3) = " << sizeof(cl_float3) << std::endl
	    << "sizeof(float4) = " << sizeof(cl_float4) << std::endl
	    << "sizeof(float8) = " << sizeof(cl_float8) << std::endl
	    << "sizeof(half) = " << sizeof(cl_half) << std::endl
	    << "sizeof(int) = " << sizeof(cl_int) << std::endl
	    << "sizeof(int16) = " << sizeof(cl_int16) << std::endl
	    << "sizeof(int2) = " << sizeof(cl_int2) << std::endl
	    << "sizeof(int3) = " << sizeof(cl_int3) << std::endl
	    << "sizeof(int4) = " << sizeof(cl_int4) << std::endl
	    << "sizeof(int8) = " << sizeof(cl_int8) << std::endl
	    << "sizeof(long) = " << sizeof(cl_long) << std::endl
	    << "sizeof(long16) = " << sizeof(cl_long16) << std::endl
	    << "sizeof(long2) = " << sizeof(cl_long2) << std::endl
	    << "sizeof(long3) = " << sizeof(cl_long3) << std::endl
	    << "sizeof(long4) = " << sizeof(cl_long4) << std::endl
	    << "sizeof(long8) = " << sizeof(cl_long8) << std::endl
	    << "sizeof(short) = " << sizeof(cl_short) << std::endl
	    << "sizeof(short16) = " << sizeof(cl_short16) << std::endl
	    << "sizeof(short2) = " << sizeof(cl_short2) << std::endl
	    << "sizeof(short3) = " << sizeof(cl_short3) << std::endl
	    << "sizeof(short4) = " << sizeof(cl_short4) << std::endl
	    << "sizeof(short8) = " << sizeof(cl_short8) << std::endl
	    << "sizeof(uchar) = " << sizeof(cl_uchar) << std::endl
	    << "sizeof(uchar16) = " << sizeof(cl_uchar16) << std::endl
	    << "sizeof(uchar2) = " << sizeof(cl_uchar2) << std::endl
	    << "sizeof(uchar3) = " << sizeof(cl_uchar3) << std::endl
	    << "sizeof(uchar4) = " << sizeof(cl_uchar4) << std::endl
	    << "sizeof(uchar8) = " << sizeof(cl_uchar8) << std::endl
	    << "sizeof(uint) = " << sizeof(cl_uint) << std::endl
	    << "sizeof(uint16) = " << sizeof(cl_uint16) << std::endl
	    << "sizeof(uint2) = " << sizeof(cl_uint2) << std::endl
	    << "sizeof(uint3) = " << sizeof(cl_uint3) << std::endl
	    << "sizeof(uint4) = " << sizeof(cl_uint4) << std::endl
	    << "sizeof(uint8) = " << sizeof(cl_uint8) << std::endl
	    << "sizeof(ulong) = " << sizeof(cl_ulong) << std::endl
	    << "sizeof(ulong16) = " << sizeof(cl_ulong16) << std::endl
	    << "sizeof(ulong2) = " << sizeof(cl_ulong2) << std::endl
	    << "sizeof(ulong3) = " << sizeof(cl_ulong3) << std::endl
	    << "sizeof(ulong4) = " << sizeof(cl_ulong4) << std::endl
	    << "sizeof(ulong8) = " << sizeof(cl_ulong8) << std::endl
	    << "sizeof(ushort) = " << sizeof(cl_ushort) << std::endl
	    << "sizeof(ushort16) = " << sizeof(cl_ushort16) << std::endl
	    << "sizeof(ushort2) = " << sizeof(cl_ushort2) << std::endl
	    << "sizeof(ushort3) = " << sizeof(cl_ushort3) << std::endl
	    << "sizeof(ushort4) = " << sizeof(cl_ushort4) << std::endl
	    << "sizeof(ushort8) = " << sizeof(cl_ushort8) << std::endl;
}

#endif
