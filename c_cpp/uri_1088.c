// https://www.urionlinejudge.com.br/judge/pt/problems/view/1088

// Este aqui em especial foi bem difícil de entender...
// A solução encontra-se no problema de encontar o número 
// de inversões num vetor, ou seja, v[i] < v[j] e j > i.

#include <stdio.h>
#include <stdlib.h>

void imprime(int * v, int a, int b)
{
	if (a > b)
		return;

	int i;
	for (i = a; i < b; i++)
		printf("%d ", i[v]);
	printf("\n");
}

int * copy(int * v, int l, int r)
{
	int * w = (int *) calloc((r-l+1), sizeof(int));

	int i;
	for (i = l; i <= r; i++)
		w[i-l] = v[i];

	return w;
}

void intercala(int * v, int p, int q, int r, int * t)
{
	int * u = copy(v, p, q);
	int * w = copy(v, q+1, r);

	int a = q-p+1, b = r-q;

	int i, j, k;

	for (i = p, j = k = 0; j < a && k < b; i++)
	{
		if (u[j] < w[k])
		{
			v[i] = u[j++];
		}
		else
		{
			//        índices   p  p+1 ..  q  q+1 q+2 ..  r 	Veja que a posição do elemento [2] no vetor
 			// 	Vetor original [1] [3] .. [5] [2] [4] .. [6]	original era (q+1) e no vetor ordenado é (p+1),
			//                                                  portanto ele se deslocou (q+1)-(p+1) = (q-p) posições.		
			//  0   1     q-p        0   1    r-q-1             Ao dividirmos os vetores em dois vetores ordenados, 
			// [1] [3] .. [5]  <->  [2] [4] .. [6]              o da esquerda está na ordem correta, portanto,
			//                                                  movimentamos (desinvertemos) apenas os elementos que	
			//   p  p+1 ..  q  q+1 q+2 ..  r                    estão no vetor da direita. De forma que o número total		
			//  [1] [2] .. [3] [4] [5] .. [6]                   inversões será o quanto o elemento se deslocou em relação
			//                                                  à sua posição original.		 
			
			(*t) += ((k+q+1) - i);
			v[i] = w[k++];
		}
	}

	while (j < a) { v[i++] = u[j++]; }
	while (k < b) { v[i++] = w[k++]; }

	free(u); free(w);
}

void mergeSort(int * v, int p, int r, int * t)
{
	if (p < r)
	{   
		int q = (p + r) / 2;
		mergeSort(v, p, q, t);
		mergeSort(v, q+1, r, t);
		intercala(v, p, q, r, t);
	}
	else
		return;
}

int main(int argc, char const *argv[]) 	 
{
	while (1)
	{
		int n; scanf("%d", &n);

		if (!n)
			return 0;

		int * v = (int *) calloc(n, sizeof(int));

		int i;
		for (i = 0; i < n; i++)
			scanf("%d", &v[i]);

		int t = 0;
		mergeSort(v, 0, n-1, &t);

		if (t % 2 == 0)
			printf("Carlos\n");
		else
			printf("Marcelo\n");

	}

	return 0;
}