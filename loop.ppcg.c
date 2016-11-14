#include <stdio.h>
#include <stdlib.h>
#define N (1 << 18)
int main()
{
  float tab[N];
  float dst[N];
  float sum;
  for(int i = 0; i < N ; i++)
    tab[i] = 1.2 + i * 3.5;

  for(int i = 0; i < N ; i++)
    dst[i] = tab[i] * tab[i] ;
  sum = 0;
  for(int i = 0; i < N ; i++)
    sum += dst[i];
  printf("%f\n", sum);
  return 0;
}
