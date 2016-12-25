// https://www.urionlinejudge.com.br/judge/pt/problems/view/1162

#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

int * copy(int * v, int a, int b)
{
	int * w = (int*) calloc((b-a+1), sizeof(int));

	for (int i = a; i <= b; i++) { w[i-a] = v[i]; }

	return w;
}

void merge(int * v, int p, int q, int r, int * t)
{
	int * w = copy(v, p, q);
	int * u = copy(v, q+1, r);

	int a = (q-p+1);
	int b = (r-q);

	int i, j, k;
	for (i = p, j = k = 0; j < a and k < b; i++)
	{
		if (w[j] < u[k]) { v[i] = w[j++]; }
		else			 
		{ 
			(*t) += ((k+q+1) - i);
			v[i] = u[k++];
		}
	}

	while (j < a) { v[i++] = w[j++]; }
	while (k < b) { v[i++] = u[k++]; }

	free(w); free(u);
}

void mergeSort(int * v, int p, int r, int * t)
{
	if (p < r)
	{
		int q = (p+r) / 2;
		mergeSort(v, p, q, t);
		mergeSort(v, q+1, r, t);
		merge(v, p, q, r, t);
	}

	return;
}

int main(int argc, char const *argv[])
{
	int n; scanf("%d%*c", &n);
	int v[51];

	while(n--)
	{
		int l; scanf("%d%*c", &l);
		for (int i = 0; i < l; i++) { scanf("%d%*c", &v[i]); }

		int t = 0; mergeSort(v, 0, l-1, &t);

		printf("Optimal train swapping takes %d swaps.\n", t);
	}

	return 0;
}