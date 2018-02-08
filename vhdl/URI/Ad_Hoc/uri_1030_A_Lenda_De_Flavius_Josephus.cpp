// https://www.urionlinejudge.com.br/judge/pt/problems/view/1030

#include <cstdio>

int a, n, k, j;
int main()
{
	scanf("%d", &a);
	
	for (int l= 0; l < a; l++)
	{
		scanf("%d %d", &n, &k);

		// O problema de Josephus pode ser analisado
		// da seguinte forma:
		// Seja j(n, k, i) uma função que retorna o sobrevivente
		// de um círculo com n participantes, onde k é número de
		// saltos de um para o outro e i é a rodada atual.
		// Se j(n, k, i) = g, vemos que o g-ésimo membro será o vencedor,
		// onde 0 <= g < n. Ao aumentarmos o círculo em um, a função será
		// j(n+1, k, i+1). Contudo, sabemos que o primeiro participante,
		// j(n+1, k, 1) será sempre o de posição k % (n+1), então, removendo ele
		// temos um círculo de n posições ! Mas para n já sabemos que será g, portanto
		// o vencedor j(n+1, k, i+1) = ((k % (n+1)) + (j(n, k, i) % (n+1))) % (n+1)
		//                           = (k + j(n, k, i)) % (n+1)
		//                           = (k + g) % (n+1).
		// Porntato, temos a fórmula recursiva:
		// j(n, k, i) = (k + j(n-1, k, i-1)) % n, onde j(1, k, 1) = 0.
		// Obs.: O resultado da função j(n, k, i) é o indice que vai de 0..n-1,
		// portanto para indexar um círculo de 1..n basta somar um ao resultado g. 

		j = 0;
		for (int i = 1; i <= n; i++)
			{ j = (k + j) % i; }

		printf("Case %d: %d\n", l+1, j+1);
	}

	return 0;
}