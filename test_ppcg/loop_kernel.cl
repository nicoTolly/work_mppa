__kernel void kernel0(__global float *dst, __global float *tab)
{
    int b0 = get_group_id(0), b1 = get_group_id(1);
    int t0 = get_local_id(0), t1 = get_local_id(1);

    #define ppcg_min(x,y)    min((__typeof__(x + y)) x, (__typeof__(x + y)) y)
    #define ppcg_max(x,y)    max((__typeof__(x + y)) x, (__typeof__(x + y)) y)
    for (int c3 = ppcg_max(t1, ((t1 + 14) % 16) - 32 * b1 + 2); c3 <= ppcg_min(31, -32 * b1 + 253); c3 += 16)
      dst[8192 * b0 + 32 * b1 + 256 * t0 + c3] = ((((tab[8192 * b0 + 32 * b1 + 256 * t0 + c3 - 2] - tab[8192 * b0 + 32 * b1 + 256 * t0 + c3 - 1]) + tab[8192 * b0 + 32 * b1 + 256 * t0 + c3]) - tab[8192 * b0 + 32 * b1 + 256 * t0 + c3 + 1]) + tab[8192 * b0 + 32 * b1 + 256 * t0 + c3 + 2]);
}
__kernel void kernel1(__global float *dst, __global float *tab)
{
    int b0 = get_group_id(0), b1 = get_group_id(1);
    int t0 = get_local_id(0), t1 = get_local_id(1);

    if (b1 == 7 && t1 >= 14)
      dst[8192 * b0 + 256 * t0 + t1 + 240] = tab[8192 * b0 + 256 * t0 + t1 + 240];
}
__kernel void kernel2(__global float *dst, __global float *tab)
{
    int b0 = get_group_id(0), b1 = get_group_id(1);
    int t0 = get_local_id(0), t1 = get_local_id(1);

    dst[8192 * b0 + 256 * t0 + t1] = tab[8192 * b0 + 256 * t0 + t1];
}
