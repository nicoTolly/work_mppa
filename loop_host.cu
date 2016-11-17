#include <assert.h>
#include <stdio.h>
#include "loop_kernel.hu"
#include <stdio.h>
#include <stdlib.h>
#define N 2000
int main()
{
	float tab[N*N];
	float dst[N*N];
	float sum;
	for(int i = 0; i < N ; i++)
		for(int j = 0; j < N ; j++)
			tab[i*N + j] = 1.2 + i * 3.5;

	{
#define cudaCheckReturn(ret) \
  do { \
    cudaError_t cudaCheckReturn_e = (ret); \
    if (cudaCheckReturn_e != cudaSuccess) { \
      fprintf(stderr, "CUDA error: %s\n", cudaGetErrorString(cudaCheckReturn_e)); \
      fflush(stderr); \
    } \
    assert(cudaCheckReturn_e == cudaSuccess); \
  } while(0)
#define cudaCheckKernel() \
  do { \
    cudaCheckReturn(cudaGetLastError()); \
  } while(0)

	  float *dev_dst;
	  float *dev_tab;
	  
	  cudaCheckReturn(cudaMalloc((void **) &dev_dst, (4000000) * sizeof(float)));
	  cudaCheckReturn(cudaMalloc((void **) &dev_tab, (4000000) * sizeof(float)));
	  
	  cudaCheckReturn(cudaMemcpy(dev_tab, tab, (4000000) * sizeof(float), cudaMemcpyHostToDevice));
	  {
	    dim3 k0_dimBlock(16, 32);
	    dim3 k0_dimGrid(63, 63);
	    kernel0 <<<k0_dimGrid, k0_dimBlock>>> (dev_dst, dev_tab);
	    cudaCheckKernel();
	  }
	  
	  cudaCheckReturn(cudaMemcpy(dst, dev_dst, (4000000) * sizeof(float), cudaMemcpyDeviceToHost));
	  cudaCheckReturn(cudaFree(dev_dst));
	  cudaCheckReturn(cudaFree(dev_tab));
	}
	sum = 0;
	for(int i = 0; i < N * N ; i++)
		sum += dst[i];
	printf("%f\n", sum);
	return 0;
}
