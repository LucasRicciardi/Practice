// https://www.urionlinejudge.com.br/judge/pt/problems/view/1057

#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>

typedef std::pair<int, int> Coordinates;
typedef std::vector< std::string > Maze;

class Graph
{
    struct Node
    {
        int level;
        std::vector< Coordinates > coordinates_list;

        Node(std::vector< Coordinates > _coordinates_list):
            level(0),
            coordinates_list( std::move(_coordinates_list) )
        {

        }
    };

private:
    std::set< std::vector<Coordinates> > discovered_nodes;
    // std::vector< Graph::Node > discovered_nodes;
private:
    /*
        Condição final é quando os três robôs estão sobre o 'X'
    */
    bool finalCondition(Graph::Node const& node, Maze const& maze)
    {
        for (auto const& coord: node.coordinates_list)
        {
            int x = coord.first;
            int y = coord.second;

            if (x < 0 or x >= maze.size() or y < 0 or y >= maze.size())
                return false;
            if ( maze[x][y] != 'X' )
                return false;
        }
        return true;
    }

    /*
        Retorna um vetor de nós adjacentes ao nó
    */
    std::vector< Graph::Node >
        adjacentNodes(Graph::Node const& node, Maze const& maze)
    {
        std::vector< Coordinates > directions =
        {
            {  1,  0 },
            { -1,  0 },
            {  0,  1 },
            {  0, -1 }
        };

        std::vector< Graph::Node > adjacent_nodes;
        for (auto& dv: directions)
        {
            int dx = dv.first;
            int dy = dv.second;

            std::vector< Coordinates > new_position;
            for (auto& coord: node.coordinates_list)
            {
                int x = coord.first;
                int y = coord.second;

                int old_dx = dx;
                int old_dy = dy;

                // Não pode se mover para fora do tabuleiro em x
                if ( (x + dx) >= maze.size() or (x + dx) < 0 )
                    dx = 0;

                // Não pode se mover para fora do tabuleiro em y
                if ( (y + dy) >= maze.size() or (y + dy) < 0)
                    dy = 0;

                // Não pode se mover no espaço com obstáculo
                if ( maze[x + dx][y + dy] == '#' )
                    (dx = dy = 0);

                new_position.push_back({ x + dx, y + dy });

                dx = old_dx;
                dy = old_dy;
            }

            std::vector< Coordinates > v(3);
            v[0] = (new_position[0] == new_position[1] || new_position[0] == new_position[2]) ?
                node.coordinates_list[0] : new_position[0];
            v[1] = (new_position[1] == new_position[0] || new_position[1] == new_position[2]) ?
                node.coordinates_list[1] : new_position[1];
            v[2] = (new_position[2] == new_position[0] || new_position[2] == new_position[1]) ?
                node.coordinates_list[2] : new_position[2];

            std::sort(v.begin(), v.end());
            v.resize(std::unique(v.begin(), v.end()) - v.begin());
            if (v.size() < 3)
                continue;
            adjacent_nodes.push_back( Graph::Node(v) );
        }

        return adjacent_nodes;
    }

    /*
        Retorna true se o nó já foi visitado
    */
    bool visited(Graph::Node const& node)
    {
        /*
        auto& a = node.coordinates_list.at(0);
        auto& b = node.coordinates_list.at(1);
        auto& c = node.coordinates_list.at(2);

        for (auto& current_node: this->discovered_nodes)
        {
            auto& d = current_node.coordinates_list.at(0);
            auto& e = current_node.coordinates_list.at(1);
            auto& f = current_node.coordinates_list.at(2);

            bool test = (a == d) and (b == e) and (c == f);
            if (test)
                return true;
        }
        return false;
        */
        return this->discovered_nodes.find(node.coordinates_list) != this->discovered_nodes.end();
    }

    /*
        Marca o nó como visitado
    */
    void visit(Graph::Node const& node)
    {

        // this->discovered_nodes.push_back(node);

        this->discovered_nodes.insert(node.coordinates_list);
    }

    /*
        O nível do nó é 1 + o nível da sua raíz
    */
    void addParent(Graph::Node& node, Graph::Node const& parent_node)
    {
        node.level = parent_node.level + 1;
    }

public:
    /*
        Inicia o grafo para ser usado
    */
    void clear()
    {
        // Zera os nós descobertos
        this->discovered_nodes.clear();
    }

    /*
        Breadth-First Search
    */
    int bfs(std::vector< Coordinates >& first_position, Maze const& maze)
    {
        std::queue< Graph::Node > q;
        Graph::Node root = Graph::Node(first_position);
        q.push( Graph::Node(root) );
        this->visit(root);

        while ( not q.empty() )
        {
            Graph::Node current_node = q.front();
            q.pop();


            if ( this->finalCondition(current_node, maze) )
            {
                return current_node.level;
            }
            std::vector< Graph::Node > adjacent_nodes = this->adjacentNodes(current_node, maze);
            for (auto& node: adjacent_nodes)
            {
                if ( not this->visited(node) )
                {
                    this->visit(node);
                    this->addParent(node, current_node);
                    q.push(node);
                }
            }
        }

        return -1;
    }
};

// Variáveis do sistema
int t;
int n;
char line[10];
Maze maze;
Graph graph;
std::vector< Coordinates > initial_position;

int main()
{
    std::cin >> t;
    for (int k = 0; k < t; k++)
    {
        // Inicia o grafo e os containeres
        graph.clear();
        initial_position.clear();
        maze.clear();

        // Lê o input
        std::cin >> n;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                std::cin >> line[j];
                switch ( line[j] )
                {
                    case 'A':
                    case 'B':
                    case 'C':
                        initial_position.push_back({ i, j });
                        line[j] = '.';
                }
            }
            maze.push_back( std::string(line) );
        }

        // Mostra o output
        printf("Case %d: ", k+1);
        int result = graph.bfs(initial_position, maze);
        result == -1 ? printf("trapped\n") : printf("%d\n", result);
    }

    return 0;
}
