#include "loop_kernel.hu"
__global__ void kernel0(float *dst, float *tab)
{
    int b0 = blockIdx.y, b1 = blockIdx.x;
    int t0 = threadIdx.y, t1 = threadIdx.x;

    #define ppcg_min(x,y)    ({ __typeof__(x) _x = (x); __typeof__(y) _y = (y); _x < _y ? _x : _y; })
    if (32 * b0 + t0 <= 1999)
      for (int c3 = t1; c3 <= ppcg_min(31, -32 * b1 + 1999); c3 += 16)
        dst[64000 * b0 + 32 * b1 + 2000 * t0 + c3] = (tab[64000 * b0 + 32 * b1 + 2000 * t0 + c3] + tab[64000 * b0 + 32 * b1 + 2000 * t0 + c3]);
}
