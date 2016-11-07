#include <assert.h>
#include <stdio.h>
#include "../ocl_utilities.h"


void __ppcg_compute_merged_prog0(float *rho, float *e, float *eps, float *jx, float *qx, float *jy, float *qy, float *jz, float *qz, float *px, float *pix, float *pw, float *piw, float *pxy, float *pyz, float *pxz, float *mx, float *my, float *mz, float *t0, float *t1, float *t2, float *t3, float *f0, float *f1, float *f2, float *f3, float *f4, float *f5, float *f6, float *f7, float *f8, float *f9, float *f10, float *f11, float *f12, float *f13, float *f14, float *f15, float *f16, float *f17, float *f18, float *Lattices)
{
  #define ppcg_fdiv_q(n,d) (((n)<0) ? -((-(n)+(d)-1)/(d)) : (n)/(d))
  {
    #define openclCheckReturn(ret) \
  if (ret != CL_SUCCESS) {\
    fprintf(stderr, "OpenCL error: %s\n", opencl_error_string(ret)); \
    fflush(stderr); \
    assert(ret == CL_SUCCESS);\
  }

    cl_mem dev_Lattices;
    
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
    program = opencl_build_program_from_file(context, device, "main_kernel.cl", "");
    
    {
      dev_Lattices = clCreateBuffer(context, CL_MEM_READ_WRITE, (2) * (128) * (128) * (128) * (19) * sizeof(float), NULL, &err);
      openclCheckReturn(err);
    }
    
    openclCheckReturn(clEnqueueWriteBuffer(queue, dev_Lattices, CL_TRUE, 0, (2) * (128) * (128) * (128) * (19) * sizeof(float), Lattices, 0, NULL, NULL));
    for (int c0 = 0; c0 <= 99; c0 += 1)
      {
        size_t global_work_size[3] = {(8) * 16, (256) * 4, 4};
        size_t block_size[3] = {16, 4, 4};
        cl_kernel kernel0 = clCreateKernel(program, "kernel0", &err);
        openclCheckReturn(err);
        openclCheckReturn(clSetKernelArg(kernel0, 0, sizeof(cl_mem), (void *) &dev_Lattices));
        openclCheckReturn(clSetKernelArg(kernel0, 1, sizeof(c0), &c0));
        openclCheckReturn(clEnqueueNDRangeKernel(queue, kernel0, 3, NULL, global_work_size, block_size, 0, NULL, NULL));
        openclCheckReturn(clReleaseKernel(kernel0));
        clFinish(queue);
      }
      
    openclCheckReturn(clEnqueueReadBuffer(queue, dev_Lattices, CL_TRUE, 0, (2) * (128) * (128) * (128) * (19) * sizeof(float), Lattices, 0, NULL, NULL));
    openclCheckReturn(clReleaseMemObject(dev_Lattices));
    openclCheckReturn(clReleaseCommandQueue(queue));
    openclCheckReturn(clReleaseProgram(program));
    openclCheckReturn(clReleaseContext(context));
  }
}
