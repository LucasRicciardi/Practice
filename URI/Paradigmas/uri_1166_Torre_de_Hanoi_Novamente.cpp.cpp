// https://www.urionlinejudge.com.br/judge/pt/problems/view/1166

#include <iostream>
#include <cstdio>

#include <vector>

// Seja n o número de casos de teste,
int n;

// t o número de varetas e k o número da bola
// a ser adicionada.
int t, k;

// Como temos 1 <= t <= 50, e cada vareta pode
// conter um número (ainda) indeterminado de elementos,
// cria-se uma matriz para representar as varetas 
// e as bolas nela suportada.
std::vector<int> v[50];

int main()
{
	scanf("%d", &n);
	while (n--)
	{
		scanf("%d", &t);
		
		// Retiramos as bolas de cada elemento a cada vez.
		// Esta passo pode ser retirado e as soluções de casos anteriores
		// podem ser usadas para encontrar próximas soluções (memoização).
		for (int i = 0; i < 50; i++) { v[i].clear(); }
		
		// O caso base é quando temos apenas uma vareta, nesta caso ele só pode
		// conter uma bola, a de #1. Portanto adicionamos a bola de #1 à primeira vareta
		// e começamos a procurar posições a partir da bola #2.
		v[0].push_back(1);
		k = 2;
		for (int i = 2; i <= t; i++)
		{	
			// Ao analisar o problema, percebemos que para cada vareta, 
			// o maior quadrado perfeito a ser encontrado é igual ao quadrado
			// do número de varetas. Assim para t = 2, o maior quadrado perfeito
			// a ser encontrado pelas regras é 4.
			// Portanto, para cada caso, buscamos o elemento em que k mais o elemento do topo
			// da pilha seja igual ao número de varetas ao quadrado, ou seja,
			// k + v.back() = i²		
			int square = i * i;		
			bool condition = true;
			
			// Buscamos para cada valor de i a melhor solução, até i = t.
			// Sabemos que encontramos a melhor solução para determinada pilha
			// quando ao percorrermos ela, não encontrarmos valor que satisfaça a equação acima.
			// Assim, adicionamos a próxima vareta.
			while (condition)
			{
				condition = false;
				for (int j = 0; j < i; j++)
				{
					if (v[j].empty())
					{
						v[j].push_back(k++);
						condition = true;
					}
					else if ((v[j].back() + k) == square)
					{
						v[j].push_back(k++);
						condition = true;
					}
				}
			}
		}		
		
		// Como começamos a contar a partir de 1, o # de bolas será igual a k-1.
		printf("%d\n", k-1);
	}
	
	return 0;
}
