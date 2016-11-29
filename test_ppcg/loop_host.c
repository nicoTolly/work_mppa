#include <assert.h>
#include <stdio.h>
#include "ocl_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#define N 256
void foo( float src)
{
  float sum=0;
  for (int i = 0; i < 8192; i++)
  {
    sum += src * 12.3;
    sum *= 2;
  }

}

int main()
{
  float tab[N*N];
  float dst[N*N];
  float sum;
  for(int i = 0; i < N*N ; i++)
    tab[i] = 1.2 + i * 3.5;

  {
    #define openclCheckReturn(ret) \
  if (ret != CL_SUCCESS) {\
    fprintf(stderr, "OpenCL error: %s\n", opencl_error_string(ret)); \
    fflush(stderr); \
    assert(ret == CL_SUCCESS);\
  }

    cl_mem dev_dst;
    cl_mem dev_tab;
    
    cl_device_id device;
    cl_context context;
    cl_program program;
    cl_command_queue queue;
    cl_int err;
    device = opencl_create_device(1);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    openclCheckReturn(err);
    queue = clCreateCommandQueue(context, device, 0, &err);
    openclCheckReturn(err);
    program = opencl_build_program_from_file(context, device, "loop_kernel.cl", "");
    
    {
      dev_dst = clCreateBuffer(context, CL_MEM_READ_WRITE, (65536) * sizeof(float), NULL, &err);
      openclCheckReturn(err);
    }
    {
      dev_tab = clCreateBuffer(context, CL_MEM_READ_WRITE, (65536) * sizeof(float), NULL, &err);
      openclCheckReturn(err);
    }
    
    openclCheckReturn(clEnqueueWriteBuffer(queue, dev_tab, CL_TRUE, 0, (65536) * sizeof(float), tab, 0, NULL, NULL));
    {
      size_t global_work_size[2] = {(8) * 32, (8) * 16};
      size_t block_size[2] = {32, 16};
      cl_kernel kernel0 = clCreateKernel(program, "kernel0", &err);
      openclCheckReturn(err);
      openclCheckReturn(clSetKernelArg(kernel0, 0, sizeof(cl_mem), (void *) &dev_dst));
      openclCheckReturn(clSetKernelArg(kernel0, 1, sizeof(cl_mem), (void *) &dev_tab));
      openclCheckReturn(clEnqueueNDRangeKernel(queue, kernel0, 2, NULL, global_work_size, block_size, 0, NULL, NULL));
      openclCheckReturn(clReleaseKernel(kernel0));
      clFinish(queue);
    }
    
    {
      size_t global_work_size[2] = {(8) * 32, (8) * 16};
      size_t block_size[2] = {32, 16};
      cl_kernel kernel1 = clCreateKernel(program, "kernel1", &err);
      openclCheckReturn(err);
      openclCheckReturn(clSetKernelArg(kernel1, 0, sizeof(cl_mem), (void *) &dev_dst));
      openclCheckReturn(clSetKernelArg(kernel1, 1, sizeof(cl_mem), (void *) &dev_tab));
      openclCheckReturn(clEnqueueNDRangeKernel(queue, kernel1, 2, NULL, global_work_size, block_size, 0, NULL, NULL));
      openclCheckReturn(clReleaseKernel(kernel1));
      clFinish(queue);
    }
    
    {
      size_t global_work_size[2] = {(8) * 32, (1) * 2};
      size_t block_size[2] = {32, 2};
      cl_kernel kernel2 = clCreateKernel(program, "kernel2", &err);
      openclCheckReturn(err);
      openclCheckReturn(clSetKernelArg(kernel2, 0, sizeof(cl_mem), (void *) &dev_dst));
      openclCheckReturn(clSetKernelArg(kernel2, 1, sizeof(cl_mem), (void *) &dev_tab));
      openclCheckReturn(clEnqueueNDRangeKernel(queue, kernel2, 2, NULL, global_work_size, block_size, 0, NULL, NULL));
      openclCheckReturn(clReleaseKernel(kernel2));
      clFinish(queue);
    }
    
    openclCheckReturn(clEnqueueReadBuffer(queue, dev_dst, CL_TRUE, 0, (65536) * sizeof(float), dst, 0, NULL, NULL));
    openclCheckReturn(clReleaseMemObject(dev_dst));
    openclCheckReturn(clReleaseMemObject(dev_tab));
    openclCheckReturn(clReleaseCommandQueue(queue));
    openclCheckReturn(clReleaseProgram(program));
    openclCheckReturn(clReleaseContext(context));
  }
  sum = 0;
  for(int i = 0; i < N * N ; i++)
    sum += dst[i];
  printf("%f\n", sum);
  return 0;
}
