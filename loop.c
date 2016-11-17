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

#pragma scop
	for(int i = 0; i < N ; i++)
		for(int j = 0; j < N ; j++)
			dst[i*N+j] = tab[i*N+j] + tab[i*N+j] ;
#pragma endscop
	sum = 0;
	for(int i = 0; i < N * N ; i++)
		sum += dst[i];
	printf("%f\n", sum);
	return 0;
}
