#include <stdio.h>
#include <stdlib.h>
#define N 100
int a[200][200] = {0};
int t[200][200] ;
int b[200][200];

int foo(char* str)
{
	return atoi(str) + 1;
}


int main(int argc, char ** argv)
{
	int nb = foo(argv[1]);
	for (int i =0; i < N; i++)
	{
		   for (int j =0; j < N; j++)
			   b[i][j] = (i % 2 == 0)?t[i][N - j - 1]:0;
		for (int j =0; j < N; j++)
		{
			t[i][j ] = a[i][j];
			t[i][j] = t[i][j] | 2;
		}
	}
#pragma scop
	for (int i =0; i < N; i++)
		for (int j =0; j < N; j++)
			b[i][j] = t[i][N - j - 1];
#pragma endscop
	int v;
} 
