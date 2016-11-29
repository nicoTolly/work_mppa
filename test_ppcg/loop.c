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

#pragma scop
  while(1)
  {
  for(int i = 0; i < N ; i++)
    for(int j = 0; j < N ; j++)
    {
      if (j <= 1)
	dst[i * N + j]=tab[i * N + j];
      else if(j >= N - 2)
	dst[i*N + j] = tab[i*N + j];
      else
	dst[i*N + j] = tab[i*N + j -2] -tab[i*N + j-1] + tab[i*N + j] -tab[i*N + j+1] + tab[i*N + j+2];
    }

  }
    
#pragma endscop
  sum = 0;
  for(int i = 0; i < N * N ; i++)
    sum += dst[i];
  printf("%f\n", sum);
  return 0;
}
