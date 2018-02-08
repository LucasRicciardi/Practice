// https://www.urionlinejudge.com.br/judge/pt/problems/view/1031

#include <cstdio>

int n, k, j;
int main()
{
	while (true)
	{
		scanf("%d", &n);

		if (not n) { break; }
		
		// Neste problema usamos a solução para o problema de Josephus.
		// O primeiro a ser eliminado será sempre a cidade 1, portanto,
		// eliminamos ela e consideramos um círculo com n-1 participantes.
		n--;
		k = 0;
		do 
		{
			// Com salto inicial de 1, calculamos o vencedor até que encontremos um salto tal que
			// j(n, k, i) = 11.
			// 11 pois a cidade 13, no círculo original, teria indice 12. Como reduzimos o círculo em
			// um, visto que a primeira cidade sempre será a primeira eliminada, a cidade treze terá
			// índice 11.

			k++;
			j = 0;
			for (int i = 1; i <= n; i++)
				{ j = (j + k) % i; }
		
		} while (j != 11);

		printf("%d\n", k);
	}

	return 0;
}