// https://www.urionlinejudge.com.br/judge/pt/problems/view/1166

#include <iostream>
#include <cstdio>

#include <vector>

// Seja n o n�mero de casos de teste,
int n;

// t o n�mero de varetas e k o n�mero da bola
// a ser adicionada.
int t, k;

// Como temos 1 <= t <= 50, e cada vareta pode
// conter um n�mero (ainda) indeterminado de elementos,
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
		// Esta passo pode ser retirado e as solu��es de casos anteriores
		// podem ser usadas para encontrar pr�ximas solu��es (memoiza��o).
		for (int i = 0; i < 50; i++) { v[i].clear(); }
		
		// O caso base � quando temos apenas uma vareta, nesta caso ele s� pode
		// conter uma bola, a de #1. Portanto adicionamos a bola de #1 � primeira vareta
		// e come�amos a procurar posi��es a partir da bola #2.
		v[0].push_back(1);
		k = 2;
		for (int i = 2; i <= t; i++)
		{	
			// Ao analisar o problema, percebemos que para cada vareta, 
			// o maior quadrado perfeito a ser encontrado � igual ao quadrado
			// do n�mero de varetas. Assim para t = 2, o maior quadrado perfeito
			// a ser encontrado pelas regras � 4.
			// Portanto, para cada caso, buscamos o elemento em que k mais o elemento do topo
			// da pilha seja igual ao n�mero de varetas ao quadrado, ou seja,
			// k + v.back() = i�		
			int square = i * i;		
			bool condition = true;
			
			// Buscamos para cada valor de i a melhor solu��o, at� i = t.
			// Sabemos que encontramos a melhor solu��o para determinada pilha
			// quando ao percorrermos ela, n�o encontrarmos valor que satisfa�a a equa��o acima.
			// Assim, adicionamos a pr�xima vareta.
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
		
		// Como come�amos a contar a partir de 1, o # de bolas ser� igual a k-1.
		printf("%d\n", k-1);
	}
	
	return 0;
}
