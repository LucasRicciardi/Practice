// https://www.urionlinejudge.com.br/judge/pt/problems/view/1210

#include <iostream>
#include <cstdio>
#include <algorithm>

// Tamanho máximo dos vetores para este problema
#define MAX 2500

// N -> Peŕiodo de produção
// I -> Idade inicial do destilador
// M -> Idade máxima do destilador
// P -> Preço de um destilador novo
int N, I, M, P;

// C[i] -> Custo de manutenção do destilador
// em função da idade, para 0 <= i < M
int C[MAX]; 

// V[i] -> Valor de venda do destilador
// em função da idade, para 0 < i <= M
int V[MAX];

// Tabela em que se salvam as melhores escolhas
// para um destilador de idade j em um período i.
int S[MAX][MAX];

// Vetor para armazenar os anos em que se vendeu.
// Por padrão este vetor é zero para todo w[i]
int w[MAX];

// Recebe um inteiro i representando um período tal que
// 0 < i <= N e um inteiro j representando a idade do destilador
// tal que 0 <= j < M e retorna o custo de manutenção do destilador.
auto manut (int const& i, int const& j) -> int
{
    return C[j] + S[i+1][j+1];
}

// Recebe um inteiro i representando um período tal que
// 0 < i <= N e um inteiro j representando a idade do destilador
// tal que 0 <= j < M e retorna o custo de venda do destilador.
auto sell (int const& i, int const& j) -> int
{
    return P + C[0] - V[j] + S[i+1][1];
}

int main()
{
    while (scanf("%d %d %d %d", &N, &I, &M, &P) != EOF)
    {
        for (int i = 0; i < M; i++) { scanf("%d", &C[i]); }
        for (int i = 0; i < M; i++) { scanf("%d", &V[i+1]); }

        // Começando de i = (N+1), buscamos o MELHOR caminho para os períodos
        // até que i = 1. Para um destilador de idade j, durante um período i, temos
        // duas escolhas, um destilador de idade j+1 durante um período i-1 ou um
        // destilador de idade 1 durante um período i-1, dessa forma temos que:
        // min $(i, j) = min ( $(manut(i,j) + min$(j+1, i-1), $venda(i,j) + min$(1, i-1) )
        // Logo vemos que a melhor escolha para um destilador de idade j durante um período i
        // deve necessariamente passar pela melhor escolha de um destilador de idade j+1 durante um período i-1
        // OU de um destilador de idade 1 durante um período i-1, portanto, calculamos a partir do período 0
        // as melhores escolhas até o período N.
        
        // Seja i o período atual.
        for (int i = (N+1); i > 0; i--)
        {
            // j a idade do destilador
            for (int j = 1; j <= M; j++)
            {
                // Seja (N+1) o período 0, logo o custo aqui é zero para todo j.
                if (i == (N+1))  { S[i][j] = 0; }

                // Se a idade do destilador for a máxima, o custo nessa idade será o custo
                // de venda do destilador
                else if (j == M) { S[i][j] = sell(i, j); }
                
                // Caso contrário, o melhor custo para esse destilador será o mínimo entre seu valor
                // de venda e o custo de um destilador de idade 1 e período i-1 e o valor de manutenção
                // mais o custo de manutenção de um destilador de periodo i-1 e idade j+1
                else             { S[i][j] = std::min(manut(i, j), sell(i, j)); }
            }

            // Aproveitamos o laço de repetição para zerar o vetor que serão armazenaos os anos em que o 
            // destilador deve ser vendido. Como existem N períodos, só podem existir no máximo N caminhos
            // e como 1 <= i <= N, o valor zero será usado como default para caso o destilador não seja vendido
            w[i-1] = 0;
        }

        // Ao final, precisamos do valor armazendo na primeira linha, que representa um período N
        // e na coluna I, que representa a idade inicial do destilador.
        printf("%d\n", S[1][I]);

        for (int i = 1, j = I, k = 0; i < (N+1); i++)
        {
            // Ao percorremos a tabela de cima para baixa, buscamos qual caminho escolhemos
            // novamente e verificamos os anos em que o destilador deve ser vendido. 
            if (j == M or sell(i, j) <= manut(i, j)) { w[k++] = i; j = 1; }
            else                                     { j++; }
        }            

        // Caso w[0] seja zero, o destilador não foi vendido.
        if (not w[0]) { printf("0"); }
        else
        {
            printf("%d", w[0]);
            int i = 1;
            while (w[i]) { printf(" %d", w[i++]); }
        }

        printf("\n");
    }

    return 0;
}