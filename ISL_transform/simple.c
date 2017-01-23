#define N 100
int a[200][200] = {0};
int t[200][200] ;
int b[200][200];
int main()
{
#pragma scop
  for (int i =0; i < N; i++)
  {
	  /*
    for (int j =0; j < N; j++)
      b[i][j] = t[i][N - j - 1];
      */
    for (int j =0; j < N; j++)
    {
      t	[i][j] = a[i][j];
      t	[i][j] += 2;
    }
  }
#pragma endscop
  for (int i =0; i < N; i++)
    for (int j =0; j < N; j++)
      b[i][j] = t[i][N - j - 1];
  int v;
} 
