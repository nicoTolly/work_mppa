#include <stdio.h>	
#include <stdlib.h>
#include <stdlib.h>
#include <errno.h>
 
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>	
#endif

#include "ocl_utilities.h"
 
#define MAX_SOURCE_SIZE (0x100000)	
const char * get_error_string(cl_int err);

int main()
{	
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem Amobj = NULL;	
	cl_mem Bmobj = NULL;	
	cl_mem Cmobj = NULL;	
	cl_program program = NULL;
	cl_kernel kernel = NULL;	
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;
 
	int i, j;
	float *A;
	float *B;
 
	A = (float *)malloc(4*sizeof(float));
	B = (float *)malloc(4*sizeof(float));
 
	FILE *fp;
	const char fileName[] = "./kernel.cl";
	size_t source_size;
	char *source_str;
 
	/* Load kernel source file */
	fp = fopen(fileName, "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");	
		exit(1);
	}	
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
 
	/* Initialize input data */
	for (i=0; i < 4; i++) {
	  A[i] = 1.0;
	}	
	for (i=0; i < 4; i++) {
	  B[i] = 3.0;
	}	
 
	/* Get Platform/Device Information */
	
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);	
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
 
	/* Create OpenCL Context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
 
	/* Create command queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
 
	/* Create Buffer Object */
	Amobj = clCreateBuffer(context, CL_MEM_READ_WRITE, 4*sizeof(float), NULL, &ret);
	Bmobj = clCreateBuffer(context, CL_MEM_READ_WRITE, 4*sizeof(float), NULL, &ret);
	if (ret != CL_SUCCESS)
	{
	  printf("failed to create buffers\n");
	  const char  * errstr = get_error_string(ret);
	  printf("%s\n", errstr);
	  abort();
	}
 
	/* Copy input data to the memory buffer */
	ret = clEnqueueWriteBuffer(command_queue, Amobj, CL_TRUE, 0, 4*sizeof(float), A, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, Bmobj, CL_TRUE, 0, 4*sizeof(float), B, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
	{
	  printf("failed to enqueue buffers\n");
	  const char  * errstr = get_error_string(ret);
	  printf("%s\n", errstr);
	  abort();
	}
 
	/* Create kernel program from source file*/
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);	
	if (ret != CL_SUCCESS)
	{
	  printf("failed to create program\n");
	  const char  * errstr = get_error_string(ret);
	  printf("%s\n", errstr);
	}
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if (ret != CL_SUCCESS)
	{
	  printf("failed to build program\n");
	  const char  * errstr = get_error_string(ret);
	  printf("%s\n", errstr);
	  if (ret == CL_BUILD_PROGRAM_FAILURE) {
	    // Determine the size of the log
	    size_t log_size;
	    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

	    // Allocate memory for the log
	    char *log = (char *) malloc(log_size);

	    // Get the log
	    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

	    // Print the log
	    printf("%s\n", log);
	  }
	  abort();
	}
 
	/* Create data parallel OpenCL kernel */	
	kernel = clCreateKernel(program, "mul", &ret);
	if (ret != CL_SUCCESS)
	{
	  const char  * errstr = get_error_string(ret);
	  printf("%s\n", errstr);
	  abort();
	}
			
	/* Set OpenCL kernel arguments */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&Amobj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&Bmobj);
 
	size_t global_item_size = 2;
	size_t local_item_size = 1;
			
	/* Execute OpenCL kernel as data parallel */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
	&global_item_size, &local_item_size, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
	{
	  printf("error enqueing\n");
	  const char  * errstr = get_error_string(ret);
	  printf("%s\n", errstr);
	  abort();
	}
	else
	  printf("correct enqueuing\n");
 
	/* Transfer result to host */
	ret = clEnqueueReadBuffer(command_queue, Amobj, CL_TRUE, 0, 4*sizeof(float), A, 0, NULL, NULL);
 
	/* Display Results */
	for (i=0; i < 4; i++) {
		printf("%f\n", A[i]);
	}	
 
 
	/* Finalization */
	ret = clFlush(command_queue);	
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(Amobj);
	ret = clReleaseMemObject(Bmobj);
	ret = clReleaseMemObject(Cmobj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);	
 
	free(source_str);
 
	free(A);
	free(B);
 
	return 0;
}
