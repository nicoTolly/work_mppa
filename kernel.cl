__kernel void mul(__global float * Dst, __global float * Src)
{
	int base = get_global_id(0);
	Dst[base] = 4 * Src[base];
}
