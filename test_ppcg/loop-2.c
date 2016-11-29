
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

#define MAX(i, j) ((i) - (j) > 0 ? (i) : (j) )
#define MIN(i, j) ((i) - (j) > 0 ? (j) : (i) )
int main()
{
  float tab[N*N];
  float dst[N*N];
  float sum;
  for(int i = 0; i < N*N ; i++)
    tab[i] = 1.2 + i * 3.5;

#pragma scop
  for(int i = 0; i < N ; i++)
    for(int j = MAX(0, - N + i); j < MIN( N, i) ; j++)
    {
      dst[i * N + ( i - j)] = tab[i * N + (i - j)] * tab[ i * N + (i - j) ];
    }
    
#pragma endscop
  sum = 0;
  for(int i = 0; i < N * N ; i++)
    sum += dst[i];
  printf("%f\n", sum);
  return 0;
}
