# -*- coding: utf-8 -*-

import math

#######################################################################
# Função que imprime o grafo com valores das distâncias  os caminhos
#######################################################################

def show_results(G, u):
    print('\nResultados da busca pelos caminhos mais curtos ...')
    print('Source (raíz) = {}'.format(u.value))
    for v in sorted(G.vertices, key=lambda v: v.value):
        print('Nó: {}, Distância mínima: {}, Caminho: '.format(v.value, v.distance), end='')
        p = v
        if p.parent == None:
            if p.value == u.value:
                print('Este é o nó raíz', end='')
            else:
                print('Não existe caminho !', end='')
        else:
            q = []
            while p != None:
                q.append(p.value)
                p = p.parent
            while len(q) > 0:
                print(q.pop(), end='')
                if len(q) > 0:
                    print(' --> ', end='')
        print('')
    print('')

#######################################################################
# Grafo
#######################################################################

class Graph():

    class Node():

        def __init__(self, value):
            self.value = value
            self.distance = 0
            self.parent = None

    def __init__(self, representation, weighted):
        self.representation = representation()
        self.weighted = weighted
        self.vertices = []

    @property
    def E(self):
        return self.representation.edges_list()

    def debug(self):
        print('Nós: ', end='')
        for u in self.vertices:
            print('{} '.format(u.value), end='')
        print('\n')
        self.representation.debug()

    def add_nodes(self, node_list):
        for node in node_list:
            self.vertices.append(
                Graph.Node(node)
            )

    def add_edges(self, edge_list):
        self.representation.add_edges(self, edge_list)

    def edges(self):
        for edge in self.representation.edges_set(self):
            yield edge

    def neighbours(self, u):
        return self.representation.neighbours(self, u)

    def weight(self, u, v):
        return self.representation.weight(u, v)

    #############################################################################
    # Algoritmos de Dijkstra, Bellman-Ford e Floyd-Warshall
    #############################################################################

    def dijkstra(self, s):
        for u in self.vertices:
            u.distance = math.inf
            u.parent =  None
        s.distance = 0
        S = []
        Q = [ u for u in self.vertices ]
        while len(Q) > 0:
            u = min(Q, key=lambda v: v.distance)
            S.append(u)
            for v in self.neighbours(u):
                if v.distance > u.distance + self.weight(u, v):
                    v.distance = u.distance + self.weight(u, v)
                    v.parent = u
            Q.remove(u)

    def bellman_ford(self, s):
        for u in self.vertices:
            u.distance = math.inf
            u.parent = None
        s.distance = 0
        for i in range(0, len(self.vertices)):
            for (u, v) in self.edges():
                if v.distance > u.distance + self.weight(u, v):
                    v.distance = u.distance + self.weight(u, v)
                    v.parent = u
        for (u, v) in self.edges():
            if v.distance > u.distance + self.weight(u, v):
                return False
        return True

    def floyd_warshall(self):
        dist = [
            [ math.inf for _ in range(0, len(self.vertices)) ]
                for _ in range(0, len(self.vertices))
        ]
        for (u, v) in self.edges():
            dist[u.value][v.value] = self.weight(u, v)
        for k in range(0, len(self.vertices)):
            for i in range(0, len(self.vertices)):
                for j in range(0, len(self.vertices)):
                    if dist[i][j] > dist[i][k] + dist[k][j]:
                        dist[i][j] = dist[i][k] + dist[k][j]
        for i in range(0, len(self.vertices)):
            dist[i][i] = math.inf
        return dist

#######################################################################
# Grafo usando Matriz de Adjacência
#######################################################################

class AdjacencyMatrix():

    def __init__(self):
        self.edges = []
        self.pt_name = 'Matriz de Adjacência'

    def add_edges(self, G, edge_list):
        self.edges = []
        for i in range(0, len(G.vertices)):
            self.edges.append([])
            for j in range(0, len(G.vertices)):
                self.edges[i].append(0)
        for (i, j), w in edge_list:
            self.edges[i][j] = w if G.weighted else 1

    def edges_set(self, G):
        r = []
        for i in range(0, len(self.edges)):
            for j in range(0, len(self.edges)):
                if self.edges[i][j] > 0:
                    r.append(
                        [ G.vertices[i], G.vertices[j] ]
                    )
        return r

    def neighbours(self, G, u):
        r = []
        for n, w in enumerate(self.edges[u.value]):
            if n == u.value:
                pass
            else:
                if w > 0:
                    r.append(G.vertices[n])
        return r

    def weight(self, u, v):
        return self.edges[u.value][v.value]

    def debug(self):
        print('Arestas: ')
        for i in range(len(self.edges)):
            for j in range(len(self.edges[i])):
                print('[{}]'.format(
                    self.edges[i][j]
                ), end='')
            print('')
        print('')

#######################################################################
# Grafo usando Lista de Adjacência
#######################################################################

class AdjacencyList():

    class Node():

        def __init__(self, node, weight):
            self.node = node
            self.weight = weight
            self.next = None

    def __init__(self):
        self.edges = []
        self.pt_name = 'Lista de Adjacência'

    def add_edges(self, G, edge_list):
        for (i, j), w in edge_list:
            w = w if G.weighted else 1
            try:
                node = self.edges[i]
                while node.next != None:
                    node = node.next
                node.next = AdjacencyList.Node(
                    G.vertices[j], w
                )
            except IndexError:
                self.edges.append(
                    AdjacencyList.Node(G.vertices[j], w)
                )

    def edges_set(self, G):
        r = []
        for n, edge in enumerate(self.edges):
            node = edge
            while node != None:
                r.append( [ G.vertices[n], node.node ])
                node = node.next
        return r

    def neighbours(self, G, u):
        r = []
        node = self.edges[u.value]
        while node != None:
            r.append(node.node)
            node = node.next
        return r

    def weight(self, u, v):
        node = self.edges[u.value]
        while node != None:
            if node.node == v:
                return node.weight
            node = node.next

    def debug(self):
        print('Arestas: ')
        for n, edge in enumerate(self.edges):
            print('({}, {}) --> '.format(n, 0), end='')
            node = edge
            while node != None:
                print('({}, {})'.format(node.node.value, node.weight), end='')
                if node.next != None:
                    print(' --> ', end='')
                node = node.next
            print('')
        print('')

###################################################################################
# Driver
###################################################################################

def main():
    # Referência para a implementação
    # https://www.cs.usfca.edu/~galles/visualization/Dijkstra.html
    # - Undirected Graph - Small Graph

    for representation in [ AdjacencyMatrix, AdjacencyList ]:

        # Cria um grafo com uma representação r
        G = Graph(representation, weighted=True)
        print('###############################################################')
        print('# Grafo usando uma {} '.format(
            G.representation.pt_name)
        )
        print('###############################################################\n')

        # Adiciona nós e arestas com peso ao grafo
        G.add_nodes([ i for i in range(8) ])
        G.add_edges([

            # Arestas com origem em 0
            [ (0, 1), 5 ],
            [ (0, 2), 4 ],
            [ (0, 3), 2 ],

            # Arestas com origem em 1
            [ (1, 0), 5 ],
            [ (1, 2), 1 ],
            [ (1, 5), 6 ],

            # Arestas com origem em 2
            [ (2, 0), 4 ],
            [ (2, 1), 1 ],
            [ (2, 4), 6 ],
            [ (2, 5), 9 ],
            [ (2, 6), 5 ],

            # Arestas com origem em 3
            [ (3, 0), 2 ],
            [ (3, 7), 4 ],

            # Arestas com origem em 4
            [ (4, 2), 6 ],
            [ (4, 6), 4 ],
            [ (4, 7), 4 ],

            # Arestas com origem em 5
            [ (5, 1), 6 ],
            [ (5, 2), 9 ],
            [ (5, 6), 6 ],

            # Arestas com origem em 6
            [ (6, 2), 5 ],
            [ (6, 4), 4 ],
            [ (6, 5), 6 ],
            [ (6, 7), 3 ],

            # Arestas com origem em 7
            [ (7, 3), 4 ],
            [ (7, 4), 4 ],
            [ (7, 6), 3 ],
        ])

        # Debug
        print('Debug da representação deste grafo')
        G.debug()

        # Roda o Algoritmo de Dijkstra em todos os vértices
        print('###############################################################')
        print('# Rodando o Algoritmo de Dijkstra em todos os vértices ...')
        print('###############################################################\n')
        for u in G.vertices:
            G.dijkstra(u)
            show_results(G, u)

        # Roda o Algoritmo de Bellman-Ford
        print('###############################################################')
        print('# Rodando o Algoritmo de Bellman-Ford em todos os vértices ...')
        print('###############################################################\n')
        for u in G.vertices:
            if G.bellman_ford(u) == True:
                print('G não contém ciclos negativos')
            else:
                print('G contém ciclos negativos')
            show_results(G, u)

        # Roda o Algoritmo de Floyd-Warshall
        print('###############################################################')
        print('# Rodando o Algoritmo de Floyd-Warshall ...')
        print('###############################################################\n')
        distance_matrix = G.floyd_warshall()
        print('  ', end='')
        for i in range(0, len(G.vertices)):
            print("  {}  ".format(i), end='')
        print('')
        for n, row in enumerate(distance_matrix):
            print('{} '.format(n), end='')
            for line in row:
                if isinstance(line, float):
                    msg = '[{}]'
                else:
                    msg = '[{0:3d}]'
                print(msg.format(line), end='')
            print('')
        print('')

if __name__ == '__main__':
    main()
