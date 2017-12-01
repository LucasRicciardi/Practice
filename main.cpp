// std io
#include <iostream>
#include <iomanip>

// numeric limits
#include <limits>

// assert
#include <assert.h>

// std container libs
#include <vector>
#include <map>
#include <list>

// #####################################################################
// # Funções Auxiliares
// #####################################################################

// #####################################################################
// # Estrutura Vértice e Estrutura Aresta
// #####################################################################

// Facilita a vida
using Vertex = unsigned int;
using Distance = long int;
using Connected = bool;

struct Edge
{
    // Facilita a vida
    using VertexPair = std::pair<Vertex, Vertex>;
    using Weight = long int;
    using List = std::vector< std::pair< VertexPair, Weight > >;

    // Par de vértices que compoe a aresta
    VertexPair vertex_pair;

    // Peso da aresta
    Weight weight;

    // Conexão entre os vertices
    Connected connected;

    /*
    *   Construtor
    */
    Edge(VertexPair p = { 0, 0 }, Weight w = 0, Connected c = false):
        vertex_pair(p),
        weight(w),
        connected(c)
    {}
};

// Peso infinito
constexpr Edge::Weight infinity = 999999;

// Nenhum pai
constexpr Vertex none = std::numeric_limits<Vertex>::max();

// #####################################################################
// # Interface de Grafo
// #####################################################################

class IGraph
{
private: // Atributos privados

protected: // Interface para subclasses

    /*
    *   Construtor
    */
    IGraph()
    {}

    /*
    *   Destrutor
    */
    virtual ~IGraph()
    {}

public: // Interface pública

};

// #####################################################################
// # Grafo Estático, usado quando o número de vértices é conhecido
// # e, consequentemente, resolvido em tempo de compilação.
// # Implementado usando uma matriz de adjacência.
// #####################################################################

template <Vertex num_of_vertexes>
class StaticGraph
{
private: // Atributos privados

    // Número de vértices é conhecido de antemão
    using AdjacencyMatrix = std::vector< std::vector<Edge> >;
    AdjacencyMatrix edges;

public: // Interface pública

    /*
    *   Construtor
    */
    StaticGraph():
        edges()
    {
        this->edges.resize(this->vertexes());
        for (Vertex i = 0; i < this->vertexes(); i++)
        {
            this->edges[i].resize(this->vertexes());
            for (Vertex j = 0; j < this->vertexes(); j++)
            {
                // Inicia as aresta
                Edge& e = this->edges[i][j];
                e.vertex_pair = { i, j };   // Par de vértices
                e.weight = 0;               // Peso 0 (default)
                e.connected = false;        // Desconectados (default)
            }
        }
    }

    /*
    *   Debug
    */
    void debug()
    {
        std::cout << std::endl;
        std::cout << "   ";
        for (Vertex i = 0; i < this->vertexes(); i++)
        {
            std::cout << " " << i << "   ";
        }
        std::cout << std::endl;
        for (Vertex i = 0; i < this->vertexes(); i++)
        {
            std::cout << i << " ";
            for (Vertex j = 0; j < this->vertexes(); j++)
            {
                std::cout << "[";
                auto& w = this->edges[i][j].weight;
                if (w == infinity)
                {
                    std::cout << "inf";
                }
                else
                {
                    std::cout.fill(' ');
                    std::cout << std::setw(3) << std::internal << this->edges[i][j].weight;
                }
                std::cout << "]";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    /*
    *   Retorna o número de vértices no grafo
    */
    unsigned long vertexes() const
    {
        return num_of_vertexes;
    }

    /*
    *   Garante que está dentro do range e avisa caso contrário
    */
    #define assert_is_in_range(n) \
        assert((n < 0 or n > this->vertexes(), "número de vértices excedido!"))

    /*
    *   Adiciona uma nova aresta (u,v) com peso w ao grafo
    */
    void add_edge(Vertex u, Vertex v, Edge::Weight w)
    {
        // Garante que u e v estão dentro do range
        assert_is_in_range(u);
        assert_is_in_range(v);
        this->edges[u][v].weight = w;
        this->edges[u][v].connected = true;
    }

    /*
    *   Remove uma aresta (u,v) do grafo
    */
    void remove_edge(Vertex u, Vertex v)
    {
        // Garante que u e v estão dentro do range
        assert_is_in_range(u);
        assert_is_in_range(v);
        this->edges[u][v].weight = infinity;
        this->edges[u][v].connected = false;
    }

    /*
    *   Relaxamento
    */
    void relax(Vertex& u, Vertex& v, std::vector<Distance>& dist, std::vector<Vertex>& parent)
    {
        Edge& e = this->edges[u][v];
        if (e.connected)
        {
            if (dist[v] > dist[u] + e.weight)
            {
                dist[v] = dist[u] + e.weight;
                parent[v] = u;
            }
        }
    }

    /*
    *   Algoritmo de Dijkstra para o caminho mais curto
    */
    void dijkstra(Vertex s, std::vector<Distance>& dist, std::vector<Vertex>& parent)
    {
        // Garante o tamanho do output
        dist.resize(this->vertexes());
        parent.resize(this->vertexes());

        // Inicialização
        std::vector<bool> visited_vertex(this->vertexes());
        for (Vertex u = 0; u < this->vertexes(); u++)
        {
            dist[u] = infinity;
            parent[u] = none;
            visited_vertex[u] = false;
        }
        dist[s] = 0;

        // Busca pelo caminho mais curto
        for (Vertex i = 0; i < this->vertexes(); i++)
        {
            // Busca pelo vértice de menor distância
            Vertex u = 0;
            Distance min = infinity;
            for (Vertex j = 0; j < this->vertexes(); j++)
            {
                if (dist[j] < min and not visited_vertex[j])
                {
                    min = dist[j];
                    u = j;
                }
            }
            visited_vertex[u] = true;

            // Relaxamento
            for (Vertex v = 0; v < this->vertexes(); v++)
            {
                this->relax(u, v, dist, parent);
            }
        }

        #undef dist
        #undef parent
    }

    /*
    *   Algoritmo de Bellman-Ford para o caminho mais curto
    */
    bool bellman_ford(Vertex s, std::vector<Distance>& dist, std::vector<Vertex>& parent)
    {
        // Inicialização
        dist.resize(this->vertexes());
        parent.resize(this->vertexes());
        for (Vertex u = 0; u < this->vertexes(); u++)
        {
            dist[u] = infinity;
            parent[u] = none;
        }
        dist[s] = 0;

        // Relaxamento
        for (Vertex i = 0; i < this->vertexes(); i++)
            for (Vertex u = 0; u < this->vertexes(); u++)
                for (Vertex v = 0; v < this->vertexes(); v++)
                    this->relax(u, v, dist, parent);

        // Verificação
        for (Vertex u = 0; u < this->vertexes(); u++)
            for (Vertex v = 0; v < this->vertexes(); v++)
                if (dist[v] > dist[u] + this->edges[u][v].weight)
                    return false;
        return true;
    }

    /*
    *   Algoritmo de Floyd-Warshall para o caminho mais curto
    */
    void floyd_warshall(std::vector< std::vector<Distance> >& dist)
    {
        // Garante o tamanho do output e inicia a matriz
        dist.resize(this->vertexes());
        for (Vertex i = 0; i < this->vertexes(); i++)
        {
            dist[i].resize(this->vertexes());
            for (Vertex j = 0; j < this->vertexes(); j++)
            {
                dist[i][j] = infinity;
            }
        }

        // Copia os pesos
        for (Vertex i = 0; i < this->vertexes(); i++)
            for (Vertex j = 0; j < this->vertexes(); j++)
                if (this->edges[i][j].connected)
                    dist[i][j] = this->edges[i][j].weight;

        // Busca pelos caminhos mais curtos
        for (Vertex k = 0; k < this->vertexes(); k++)
            for (Vertex i = 0; i < this->vertexes(); i++)
                for (Vertex j = 0; j < this->vertexes(); j++)
                    if (dist[i][j] > dist[i][k] + dist[k][j])
                        dist[i][j] = dist[i][k] + dist[k][j];

        // Marca como infinito a diagonal principal
        for (Vertex i = 0; i < this->vertexes(); i++)
            dist[i][i] = infinity;
    }

    // Limpa os #defines
    #undef assert_is_in_range
};

// #####################################################################
// # Grafo Dinâmico, usado quando o número de vértices é desconhecido
// # e há a necessidade de se adicionar e/ou remover vértices
// # em tempo de execução do programa.
// # Implementado usando uma lista de adjacência.
// #####################################################################

class DynamicGraph:
    public IGraph
{

private: // Atributos privados

    // Mapa de listas encadeadas para armazenar os vértices
    using AdjacencyList = std::map< Vertex, std::map< Vertex, Edge > >;
    AdjacencyList edges;

public: // Interface pública

    /*
    *   Construtor
    */
    DynamicGraph():
        edges()
    {}

    /*
    *   Debug
    */
    void debug()
    {
        for (Vertex u = 0; u < this->vertexes(); u++)
        {
            std::cout << u;
            for (auto const& edge : this->edges[u])
            {
                std::cout << " --> " << edge.second.vertex_pair.second;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    /*
    *   Retorna o número de vértices no grafo
    */
    unsigned long vertexes() const
    {
        return this->edges.size();
    }

    /*
    *   Adiciona um novo vértice u ao grafo
    */
    bool add_vertex(Vertex u)
    {
        auto it = this->edges.find(u);
        if (it == this->edges.end())
        {
            this->edges[u] = std::map< Vertex, Edge >();
            return true;
        }
        else return false;
    }

    /*
    *   Remove um vértice u do grafo
    */
    bool remove_vertex(Vertex u)
    {
        try
        {
            this->edges.erase(u);
            return true;
        }
        catch (std::out_of_range const&)
        {
            return false;
        }
    }

    /*
    *   Adiciona uma nova aresta (u,v) com peso w ao grafo
    */
    void add_edge(Vertex u, Vertex v, Edge::Weight w)
    {
        try
        {
            this->edges[u][v] = Edge({ u, v }, w, true);
        }
        catch (std::out_of_range const&)
        {
            return;
        }
    }

    /*
    *   Remove uma aresta (u,v) do grafo
    */
    void remove_edge(Vertex u, Vertex v)
    {
        try
        {
            this->edges[u].erase(v);
        }
        catch (std::out_of_range const&)
        {
            return;
        }
    }

    /*
    *   Relaxamento
    */
    void relax(Vertex& u, Vertex& v, std::vector<Distance>& dist, std::vector<Vertex>& parent)
    {
        Edge& e = this->edges[u][v];
        if (e.connected)
        {
            if (dist[v] > dist[u] + e.weight)
            {
                dist[v] = dist[u] + e.weight;
                parent[v] = u;
            }
        }
    }

    /*
    *   Algoritmo de Dijkstra para o caminho mais curto
    */
    void dijkstra(Vertex s, std::vector<Distance>& dist, std::vector<Vertex>& parent)
    {
        // Garante o tamanho do output
        dist.resize(this->vertexes());
        parent.resize(this->vertexes());

        // Inicialização
        std::vector<bool> visited_vertex(this->vertexes());
        for (Vertex u = 0; u < this->vertexes(); u++)
        {
            dist[u] = infinity;
            parent[u] = none;
            visited_vertex[u] = false;
        }
        dist[s] = 0;

        // Busca pelo caminho mais curto
        for (Vertex i = 0; i < this->vertexes(); i++)
        {
            // Busca pelo vértice de menor distância
            Vertex u = 0;
            Distance min = infinity;
            for (Vertex j = 0; j < this->vertexes(); j++)
            {
                if (dist[j] < min and not visited_vertex[j])
                {
                    min = dist[j];
                    u = j;
                }
            }
            visited_vertex[u] = true;

            // Relaxamento
            for (Vertex v = 0; v < this->vertexes(); v++)
            {
                this->relax(u, v, dist, parent);
            }
        }

        #undef dist
        #undef parent
    }

    /*
    *   Algoritmo de Bellman-Ford para o caminho mais curto
    */
    bool bellman_ford(Vertex s, std::vector<Distance>& dist, std::vector<Vertex>& parent)
    {
        // Inicialização
        dist.resize(this->vertexes());
        parent.resize(this->vertexes());
        for (Vertex u = 0; u < this->vertexes(); u++)
        {
            dist[u] = infinity;
            parent[u] = none;
        }
        dist[s] = 0;

        // Relaxamento
        for (Vertex i = 0; i < this->vertexes(); i++)
            for (Vertex u = 0; u < this->vertexes(); u++)
                for (Vertex v = 0; v < this->vertexes(); v++)
                    this->relax(u, v, dist, parent);

        // Verificação
        for (Vertex u = 0; u < this->vertexes(); u++)
            for (Vertex v = 0; v < this->vertexes(); v++)
                if (dist[v] > dist[u] + this->edges[u][v].weight)
                    return false;
        return true;
    }

    /*
    *   Algoritmo de Floyd-Warshall para o caminho mais curto
    */
    void floyd_warshall(std::vector< std::vector<Distance> >& dist)
    {
        // Garante o tamanho do output e inicia a matriz
        dist.resize(this->vertexes());
        for (Vertex i = 0; i < this->vertexes(); i++)
        {
            dist[i].resize(this->vertexes());
            for (Vertex j = 0; j < this->vertexes(); j++)
            {
                dist[i][j] = infinity;
            }
        }

        // Copia os pesos
        for (Vertex i = 0; i < this->vertexes(); i++)
            for (Vertex j = 0; j < this->vertexes(); j++)
                if (this->edges[i][j].connected)
                    dist[i][j] = this->edges[i][j].weight;

        // Busca pelos caminhos mais curtos
        for (Vertex k = 0; k < this->vertexes(); k++)
            for (Vertex i = 0; i < this->vertexes(); i++)
                for (Vertex j = 0; j < this->vertexes(); j++)
                    if (dist[i][j] > dist[i][k] + dist[k][j])
                        dist[i][j] = dist[i][k] + dist[k][j];

        // Marca como infinito a diagonal principal
        for (Vertex i = 0; i < this->vertexes(); i++)
            dist[i][i] = infinity;
    }
};

// #####################################################################
// # Driver
// #####################################################################

int main()
{
    // Lista de arestas que define o grafo
    Edge::List edge_list = {

           // Arestas com origem em 0
           { { 0, 1 }, 5 },
           { { 0, 2 }, 4 },
           { { 0, 3 }, 2 },

           // Arestas com origem em 1
           { { 1, 0 }, 5 },
           { { 1, 2 }, 1 },
           { { 1, 5 }, 6 },

           // Arestas com origem em 2
           { { 2, 0 }, 4 },
           { { 2, 1 }, 1 },
           { { 2, 4 }, 6 },
           { { 2, 5 }, 9 },
           { { 2, 6 }, 5 },

           // Arestas com origem em 3
           { { 3, 0 }, 2 },
           { { 3, 7 }, 4 },

           // Arestas com origem em 4
           { { 4, 2 }, 6 },
           { { 4, 6 }, 4 },
           { { 4, 7 }, 4 },

           // Arestas com origem em 5
           { { 5, 1 }, 6 },
           { { 5, 2 }, 9 },
           { { 5, 6 }, 6 },

           // Arestas com origem em 6
           { { 6, 2 }, 5 },
           { { 6, 4 }, 4 },
           { { 6, 5 }, 6 },
           { { 6, 7 }, 3 },

           // Arestas com origem em 7
           { { 7, 3 }, 4 },
           { { 7, 4 }, 4 },
           { { 7, 6 }, 3 },
    };

    constexpr int num_of_vertexes = 8;

    std::cout << "###########################################################" << std::endl;
    std::cout << "# GRAFO COM MATRIZ DE ADJACÊNCIA" << std::endl;
    std::cout << "###########################################################\n" << std::endl;

    // Cria um gráfico simples com 'num_of_vertexes' vértices e adiciona as arestas
    StaticGraph<num_of_vertexes> static_graph;
    for (auto& edge : edge_list)
    {
        auto& vertex = edge.first;
        auto& weight = edge.second;
        static_graph.add_edge(vertex.first, vertex.second, weight);
    }

    // Imprime o grafo para debug
    static_graph.debug();

    std::cout << "###########################################################" << std::endl;
    std::cout << "# Dijkstra" << std::endl;
    std::cout << "###########################################################\n" << std::endl;

    // Algoritmo de Dijkstra
    std::vector<Distance> dist;
    std::vector<Vertex> path;
    static_graph.dijkstra(0, dist, path);
    for (Vertex i = 0; i < num_of_vertexes; i++)
    {
        std::cout << " Vértice : " << i;
        std::cout << " Distância: " << dist[i];
        std::cout << " Caminho: " << i;
        Vertex parent = path[i];
        while (parent != none)
        {
            std::cout << " <--- " << parent;
            parent = path[parent];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "###########################################################" << std::endl;
    std::cout << "# Bellman-Ford" << std::endl;
    std::cout << "###########################################################\n" << std::endl;

    dist.clear();
    path.clear();
    static_graph.bellman_ford(0, dist, path);
    for (Vertex i = 0; i < num_of_vertexes; i++)
    {
        std::cout << " Vértice : " << i;
        std::cout << " Distância: " << dist[i];
        std::cout << " Caminho: " << i;
        Vertex parent = path[i];
        while (parent != none)
        {
            std::cout << " <--- " << parent;
            parent = path[parent];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "###########################################################" << std::endl;
    std::cout << "# Floyd-Warshall" << std::endl;
    std::cout << "###########################################################\n" << std::endl;

    std::vector< std::vector<Distance> > distance_matrix;
    static_graph.floyd_warshall(distance_matrix);
    for (Vertex i = 0; i < num_of_vertexes; i++)
    {
        for (Vertex j = 0; j < num_of_vertexes; j++)
        {
            std::cout << "[";
            if (distance_matrix[i][j] == infinity)
            {
                std::cout << "inf";
            }
            else
            {
                std::cout.fill(' ');
                std::cout << std::setw(3) << std::internal << distance_matrix[i][j];
            }
            std::cout << "]";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "###########################################################" << std::endl;
    std::cout << "# GRAFO COM LISTA DE ADJACÊNCIA" << std::endl;
    std::cout << "###########################################################\n" << std::endl;

    // Grafo dinâmico
    DynamicGraph dynamic_graph;

    // Adiciona vértices
    for (uint32_t i = 0; i < num_of_vertexes; i++)
    {
        dynamic_graph.add_vertex(i);
    }

    // Adiciona arestas
    for (auto& edge : edge_list)
    {
        auto& vertex = edge.first;
        auto& weight = edge.second;
        dynamic_graph.add_edge(vertex.first, vertex.second, weight);
    }

    // debug
    dynamic_graph.debug();

    std::cout << "###########################################################" << std::endl;
    std::cout << "# Dijkstra" << std::endl;
    std::cout << "###########################################################\n" << std::endl;

    dist.clear();
    path.clear();
    dynamic_graph.dijkstra(0, dist, path);
    for (Vertex i = 0; i < num_of_vertexes; i++)
    {
        std::cout << " Vértice : " << i;
        std::cout << " Distância: " << dist[i];
        std::cout << " Caminho: " << i;
        Vertex parent = path[i];
        while (parent != none)
        {
            std::cout << " <--- " << parent;
            parent = path[parent];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "###########################################################" << std::endl;
    std::cout << "# Bellman-Ford" << std::endl;
    std::cout << "###########################################################\n" << std::endl;

    dist.clear();
    path.clear();
    dynamic_graph.bellman_ford(0, dist, path);
    for (Vertex i = 0; i < num_of_vertexes; i++)
    {
        std::cout << " Vértice : " << i;
        std::cout << " Distância: " << dist[i];
        std::cout << " Caminho: " << i;
        Vertex parent = path[i];
        while (parent != none)
        {
            std::cout << " <--- " << parent;
            parent = path[parent];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "###########################################################" << std::endl;
    std::cout << "# Floyd-Warshall" << std::endl;
    std::cout << "###########################################################\n" << std::endl;

    distance_matrix.clear();
    dynamic_graph.floyd_warshall(distance_matrix);
    for (Vertex i = 0; i < num_of_vertexes; i++)
    {
        for (Vertex j = 0; j < num_of_vertexes; j++)
        {
            std::cout << "[";
            if (distance_matrix[i][j] == infinity)
            {
                std::cout << "inf";
            }
            else
            {
                std::cout.fill(' ');
                std::cout << std::setw(3) << std::internal << distance_matrix[i][j];
            }
            std::cout << "]";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
