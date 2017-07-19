#include <iostream>
#include <cstdio>
#include <cstdint>

// Variáveis globais
int t;
int n;
int v, e;
int used_vertices;

// Vetor de vértices e variáveis para acesso
uint64_t vertex_list;
uint64_t mask = 0x1;
uint64_t shift[2];

// Variáveis auxiliares
int i, j;

int main()
{
    // Lê o número de casos de testes e inicia o sistema
    scanf("%d", &t);
    while (t --> 0)
    {
        // Ao invés de um vetor, usarei um inteiro positivo de 64 bits
        // para armazenar os vértices, isso torna o tempo de inicialização constante
        vertex_list |= 0xffffffffffffffff;
                      
        // Lê o nó inicial e o número de vértices e arestas
        scanf("%d", &n);
        scanf("%d %d", &v, &e);

        // Se não permitirmos multiplas arestas, o número
        // mínimo de passos é igual ao dobro do número de arestas
        used_vertices = 0;        
        while (e --> 0)
        {
            // Lê o par de vértices
            scanf("%d %d", &i, &j);

            // Para não ficar escrevendo (mask << i) e (mask << j)
            shift[0] = (mask << i);
            shift[1] = (mask << j);

            // Vê se a posição ainda não foi contada
            for (int k = 0; k < 2; k++)
                if (vertex_list & shift[k])
                    (vertex_list ^= shift[k], used_vertices++);
        }

        // Número mínimo de passos é igual à: (número de vértice usados - 1) * 2
        printf("%d\n", (used_vertices-1) * 2);
    }

    return 0;
}