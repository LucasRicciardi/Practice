// https://www.ime.usp.br/~pf/algoritmos/aulas/mrgsrt.html

// ########################################################################
//	 
//					%%%%%%%%%	%       %		%%%%%
//					%			 %	   %	%		%
//					%			  %   %		%		%
//					%%%%%%%%%	   % %				%
//					%			  %	  %		%		%
//					%			 %	   %	%		%
//					%%%%%%%%%	%	    %		%%%%%%%%%
//				
// #######################################################################

#include <stdio.h>
#include <stdlib.h>

// 1. Escreva uma função que receba vetores disjuntos x[0..m-1] e
// y[0..n-1]. ambos em ordem crescente, e produza um vetor z[0..m+n-1]
// que contenha o resultado da intercalação dos dois vetores dados.
// Escreva duas versões da função: uma iterativa e uma recursiva.

// Recebe dois vetores x e y e dois inteiros m e n
// sendo m o tamanho de x e n o tamanho de y e retorna
// um vetor v de tamanho m+n contendo a intercalação
// dos elementos de x e y tal que v[i] <= v[i+1].
// x e y devem estar ordenados.

// Versão iterativa: Complexidade O(n)
int * intercala_it(int * x, int * y, int m, int n) 
{

	// Cria um novo vetor v com tamanho de m+n
	int * v = (int *) calloc(m+n, sizeof(int));

	// A cada iteração, compara o valor de x e y
	// e insere o menor em v, e então, o vetor cujo
	// elemento foi inserido em v (x ou y) avança para
	// a próxima posição.
	int i, j, k;
	for (i = j = k = 0; j < m && k < n; i++)
	{
		if (x[j] < y[k])
			v[i] = x[j++];
		else
			v[i] = y[k++];
	}

	// O loop sai quando ou x ou y esgotaram seus valores,
	// portanto basta copiar para v o que sobrou do outro vetor.
	while (j < m) v[i++] = x[j++];
	while (k < n) v[i++] = y[k++]; 

	return v;
}

// Versão recursiva
void intercala(int * x, int * y, int m, int n, int * v)
{
	if (m > 0 && n > 0)
	{
		if (*x < *y)
		{
			*v = *x;
			intercala(++x, y, --m, n, ++v);
		}
		else
		{
			*v = *y;
			intercala(x, ++y, m, --n, ++v);
		}
	}
	else
	{
		if (m > 0)
		{
			*v = *x;
			intercala(++x, y, --m, 0, ++v);
		}

		else if (n > 0)
		{
			*v = *y;
			intercala(x, ++y, 0, --n, ++v);
		}
	}

	return;
}

int * intercala_rec(int * x, int * y, int m, int n)
{
	int * v = (int *) calloc(m+n, sizeof(int));

	intercala(x, y, m, n, v);

	return v;	
}

// ########################################################################

int main(int argc, char const *argv[])
{
	int x[] = { 1, 3, 5 };
	int y[] = { 2, 4, 6 };

//	int * v = intercala_it(x, y, 3, 3);
	int * v = intercala_rec(x, y, 3, 3);

	for (int i = 0; i < 6; i++)
		printf("%d ", v[i]);

	return 0;
}