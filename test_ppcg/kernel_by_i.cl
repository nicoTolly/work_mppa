__kernel void kernel0(__global float *dst, __global float *tab)
{
    int b0 = get_group_id(0), b1 = get_group_id(1);
    int t0 = get_local_id(0), t1 = get_local_id(1);

    if (32 * b0 + t0 >= 2 && 32 * b0 + t0 <= 253)
      for (int c3 = t1; c3 <= 31; c3 += 16)
        dst[8192 * b0 + 32 * b1 + 256 * t0 + c3] = ((((tab[8192 * b0 + 32 * b1 + 256 * t0 + c3 - 512] - tab[8192 * b0 + 32 * b1 + 256 * t0 + c3 - 256]) + tab[8192 * b0 + 32 * b1 + 256 * t0 + c3]) - tab[8192 * b0 + 32 * b1 + 256 * t0 + c3 + 256]) + tab[8192 * b0 + 32 * b1 + 256 * t0 + c3 + 512]);
}
__kernel void kernel1(__global float *dst, __global float *tab)
{
    int b0 = get_group_id(0), b1 = get_group_id(1);
    int t0 = get_local_id(0), t1 = get_local_id(1);

    if (b0 == 7 && t0 >= 30)
      for (int c3 = t1; c3 <= 31; c3 += 16)
        dst[32 * b1 + 256 * t0 + c3 + 57344] = tab[32 * b1 + 256 * t0 + c3 + 57344];
}
__kernel void kernel2(__global float *dst, __global float *tab)
{
    int b0 = get_group_id(0), b1 = get_group_id(1);
    int t0 = get_local_id(0), t1 = get_local_id(1);

    for (int c3 = t1; c3 <= 31; c3 += 16)
      dst[32 * b1 + 256 * t0 + c3] = tab[32 * b1 + 256 * t0 + c3];
}
