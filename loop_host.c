#include <assert.h>
#include <stdio.h>
#include "ocl_utilities.h"
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

  /* ppcg generated CPU code */
  
  {
    int i;
    for (i = 0; i < (1 << 18); i += 1) {
      dst[i] = (tab[i] * tab[i]);
    }
  }
  sum = 0;
  for(int i = 0; i < N ; i++)
    sum += dst[i];
  printf("%f\n", sum);
  return 0;
}
