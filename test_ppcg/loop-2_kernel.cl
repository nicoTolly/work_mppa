__kernel void kernel0(__global float *dst, __global float *tab)
{
    int b0 = get_group_id(0), b1 = get_group_id(1);
    int t0 = get_local_id(0), t1 = get_local_id(1);

    #define ppcg_min(x,y)    min((__typeof__(x + y)) x, (__typeof__(x + y)) y)
    for (int c3 = t1; c3 <= ppcg_min(31, 32 * b0 - 32 * b1 + t0 - 1); c3 += 16)
      dst[8224 * b0 - 32 * b1 + 257 * t0 - c3] = (tab[8224 * b0 - 32 * b1 + 257 * t0 - c3] * tab[8224 * b0 - 32 * b1 + 257 * t0 - c3]);
}
