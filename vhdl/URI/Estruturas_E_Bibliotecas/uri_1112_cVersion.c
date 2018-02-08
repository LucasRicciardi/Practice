#include <stdio.h>
#include <stdlib.h>

#define max 1000
#define max_ max + 1 

// Solução para em C para o problema Schweisen
// https://www.urionlinejudge.com.br/judge/pt/problems/view/1112
// Eu já havia solucionado o problema em C++, mas originalmente tinha
// implementado em C. O interessante é que a solução em C++ foi 
// quase 0.3s mais rápida do que esta.


void zero(int v[max_][max_], int x, int y)
{
	int i, j;
	for (i = 1; i <= x; i++)
		for (j = 1; j <= y; j++)
			v[i][j] = 0;
}

void update(int v[max_][max_], int x, int y, int xSize, int ySize, int value)
{
	x++; y++;
	int i, j;
	for (i = x; i <= xSize; i += (i&(-i)))
		for (j = y; j <= ySize; j += (j&(-j)))
			v[i][j] += value;
}

int sum(int v[max_][max_], int x, int y)
{
	x++; y++;
	int i, j, sum = 0;
	for (i = x; i > 0; i -= (i&(-i)))
		for (j = y; j > 0; j -= (j&(-j)))
			sum += v[i][j];
		
	return sum;	
}

void swap(int * a, int * b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int query(int v[max_][max_], int x1, int y1, int x2, int y2)
{
	if (x1 > x2) { swap(&x1, &x2); }
	if (y1 > y2) { swap(&y1, &y2); }

	return (sum(v, x2, y2) + sum(v, x1-1, y1-1)
				- sum(v, x2, y1-1) - sum(v, x1-1, y2));
}

char c;

int main(int argc, char const *argv[])
{
	int b[max_][max_];

	while (1)
	{
		int x, y, p; scanf("%d %d %d%*c", &x, &y, &p);
		if (!(x+y+p)) { return 0; }

		zero(b, x, y);

		int q; scanf("%d%*c", &q);

		while (q--)
		{
			char c = getchar();

			if ( c == 'A')
			{
				int n, m, l; scanf("%d %d %d%*c", &n, &m, &l);
				update(b, m, l, x, y, n*p);
			}
			else
			{
				int n, m, l, o; scanf("%d %d %d %d%*c", &n, &m, &l, &o);


				printf("%d\n", query(b, n, m, l, o));
			}
		}
		putchar('\n');
	}

	return 0;
}