// https://www.urionlinejudge.com.br/judge/pt/problems/view/1120

#include <stdio.h>

char n, str[500], out[500], * c, * d;
int main(int argc, char const *argv[])
{
	while (1)
	{
		// Lê primeiro o número a ser retirado
		// e depois o vetor de char com os caracteres
		// a serem processados.
		scanf("%c %s", &n, str);
		getchar();

		// Caso n seja '0' e o primero elemento do vetor
		// de caracteres seja '0', pare a execução do programa.
		if (!(n^str[0]) && !(n^'0')) { break; }

		// Percorre o vetor de caracteres lido copiando os valores
		// que são diferentes de n para um novo vetor .
		int k = 1;
		for (c = str, d = out; *c != '\0'; c++)
		{	
			// Caso o caracter atual seja igual ao à ser retirado,
			// pule para a próxima iteração.
			if (*c == n)  { continue; }

			// Caso contrário, adiciona c ao elemento atual e
			// ande para o próximo.
			else          { *d++ = *c; }

			// Se se for igual a '0' e nenhum outro caracter diferente de
			// '0 tiver sido lido', então não ande com d.		
			if (*c > '0') { k = 0; }
			if (k) { d--; }
		}
		// Copia o caracter nulo ('\0') para marcar o fim de d
		*d = *c;

		// Se o endereço de d for IGUAL ao primeiro endereço do vetor out,
		// nenhum caracter foi copiado, portanto, imprima '0'.s
		if (!(d-out)) { printf("0"); }

		// Caso contrário, imprima o vetor out.
		else          { printf("%s", out); }
		printf("\n");
	}


	return 0;
}