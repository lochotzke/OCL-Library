#ifndef OCL_ERRORS_SETUP
#define OCL_ERRORS_SETUP

// Opencl Variable Sizes
#ifndef OCL_VARIABLES
#define OCL_VARIABLES
#define K_INT sizeof(int)
#define K_CHAR sizeof(char)
#define K_LONG sizeof(long)
#define K_LONG_LONG sizeof(long long)
#define K_FLOAT sizeof(float)
#define K_DOUBLE sizeof(double)
#define K_CL_MEM sizeof(cl_mem)
#define K_INT8_T sizeof(int8_t)
#define K_INT16_T sizeof(int16_t)
#define K_INT32_T sizeof(int32_t)
#define K_INT64_T sizeof(int64_t)
#define K_UINT8_T sizeof(uint8_t)
#define K_UINT16_T sizeof(uint16_t)
#define K_UINT32_T sizeof(uint32_t)
#define K_UINT64_T sizeof(uint64_t)
#endif

// OpenCL Errors
const char oc_error[65][45] = {
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

#endif
