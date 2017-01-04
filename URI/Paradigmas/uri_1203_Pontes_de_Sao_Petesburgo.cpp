// https://www.urionlinejudge.com.br/judge/pt/problems/view/1203

#include <iostream>
#include <cstdio>
#include <algorithm>

// Solução 'tola' para o problema da soma do subconjunto.
bool findSum(int const * const& v, int n, int k)
{
    // Se a soma for zero, está na subsequência.
    if (not k) { return true; }

    // Se chegou ao final e nada, retorna false.
    if (not n) { return false; }

    // Se o valor do elemento for maior do que o da soma, retira ele.
    if (v[n-1] > k) { return findSum(v, n-1, k); }

    // Ou o elemento está presente no conjunto COMPLETO,
    // ou ele está presente em algum subconjunto, neste caso,
    // basta chamar a função para um conjunto sem o elemento atual.
    return findSum(v, n-1, k) or findSum(v, n-1, k - v[n-1]); 
}

// Solução usando programação dinâmica para o problema da soma do subconjunto.
bool dfs(int const * const& v, int const& n, int const& k) // (dynamic find sum)
{
    // Cria-se uma matriz com n+1 linhas e k+1 colunas, 
    // sendo n o número de elementos no conjunto e k
    // o número desejado a saber se sua soma se encontra no conjunto.
    // Estes valores fixos são por conta do URI (online judge).    
    bool s[101][4951];

    // A primeira linha recebe valor false.
    for (int i = 1; i <= k; i++) { s[0][i] = false; }

    // A primeira coluna recebe valor true.
    for (int i = 0; i <= n; i++) { s[i][0] = true; }    

    // Começando a partir da posição s[1][1], verifica-se se aquele subconjunto (v[0] .. v[i])
    // pode somar até o valor j. Caso sim, recebe o valor true, e false caso contrário.
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= k; j++)
        {                                                   
            s[i][j] = s[i-1][j];                            // O valor básico é o valor da posição de cima.           
            if (j >= v[i-1])                                // Se o valor de v[i-1] for menor do que o de j, então
            {                                               // o valor dessa posição será true caso a posição acima
                s[i][j] = s[i][j] or s[i-1][j - v[i-1]];    // seja true ou caso o valor da linha de cima, recuando-se
            }                                               // v[i-1] posições, seja true.
        }                                                   // A observação é que, como os vetores são indexados de 0..n,
    }                                                       // então temos que voltar uma posição. Assim a linha i da matriz
                                                            // corresponde à posição i-1 do vetor original.
    #ifndef ONLINE_JUDGE
    printf("\n");
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= k; j++)
        {
            printf("%d ", s[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    #endif

    return s[n][k];  // Ao final, se a posição s[n][k] for true, então k pertence ao conjunto.
}

int main()
{
    int v[100];
    int r, k, x, y;
    while (scanf("%d %d", &r, &k) != EOF)
    {   
        for (int i = 0; i < r; i++) { v[i] = 0; }   

        for (int i = 0; i < k; i++)
        {
            scanf("%d %d", &x, &y);
            v[x-1]++;
            v[y-1]++;
        }

        if (dfs(v, r, k)) { printf("S\n"); }
        else              { printf("N\n"); }
    }   

    return 0;
}