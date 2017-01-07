// https://www.urionlinejudge.com.br/judge/pt/problems/view/1026

#include <cstdio>

unsigned int A, B;
int main()
{
	while (scanf("%u %u", &A, &B) != EOF)
	{		
		// O exercício pede simplesmente o
		// resultado de x XOR y (x ^ y).
		// 
		//      Tabela verdade XOR.
		//       ---------------
		//       | A | B | A^B |
		//       ---------------
		//       | 0 | 0 |  0  |
		//       | 0 | 1 |  1  |
		//       | 1 | 0 |  1  |
		//       | 1 | 1 |  0  |
		//       --------------- 
		//
		// Ao escrever o circuito do exercício na forma algébrica, temos que
		// S = (!A)&B | A&(!B) | A&B, porém, o enunciado diz que o carry  é
		// sempre zero, portanto o circuito é: 
		// S = (!A)&B | A&(!B) | 0 = (!A)&B | A&(!B) = A^B.	

		printf("%u\n", A^B);
	}

	return 0;
}