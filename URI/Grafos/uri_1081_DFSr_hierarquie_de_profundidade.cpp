#include <iostream>
#include <cstdio>

int n;
int vertex_list[20], adjacency_matrix[20][20];
int v, e;

bool dfs(int vertex, int depth)
{
    // Variável dizer se há algum vértice adjacente não visitado
    bool found = false;
    
    // Marca o vértice como visitado
    vertex_list[vertex] = 0;
    
    // Busca por vértices adjacentes
    for (int i = 0; i < v; i++)
    {
        // Se o vértice ainda não tiver sido visitado
        if (adjacency_matrix[vertex][i] == 1)
        {   
            // Imprime os espaços de acordo com a profundidade
            for (int j = 0; j < depth; j++)
                printf(" ");

            // Imprime os vértices que definem a aresta
            printf("%d-%d", vertex, i); 
            
            // Imprime o caminho se o vértice não foi visitado
            if (vertex_list[i] == 1)
                printf(" pathR(G,%d)", i);
            printf("\n");

            // Remove a entrada da matriz de adjacência
            adjacency_matrix[vertex][i] = 0;

            // Busca recursivamente no vértice seguinte
            dfs(i, depth+2);

            // Avisa que foi achado um vértice não visitado
            found = true;
        }
    }

    return found;
}

int main()
{
    // Variáveis usadas pelo sistema
    int i, j, k;

    // Zera a matriz de adjacência e o vetor que indica se o vértice foi visitado
    // Como o algoritmo garante que todos os vértices sejam visitados, essa rotina
    // só precisa ser executada uma vez para iniciar o sistema e podemos usar o
    // próprio algoritmo para zerar as entradas, economizando (20 * 20) = 400
    // passos por iteração
    for (i = 0; i < 20; i++)
        for (j = 0; j < 20; j++)
            vertex_list[i] = adjacency_matrix[i][j] = 0;        
    
    // Lê o número de entradas e inicia o sistema
    scanf("%d", &n);
    for (int k = 0; k < n; k++)
    {   
        // Lê o número de vértices e o número de arestas
        scanf("%d %d", &v, &e);
    
        // Lê as informações sobre as arestas do grafo e monta a matriz de adjacência
        while (e --> 0)
        {
            scanf("%d %d", &i, &j);
            vertex_list[i] = vertex_list[j] = adjacency_matrix[i][j] = 1;
        }

        // Chama o algoritmo de busca em profundidade (depth-first search)
        printf("Caso %d:\n", k+1);
        for (i = 0; i < v; i++)
            if (dfs(i, 2))
                printf("\n");
    }

    return 0;
}