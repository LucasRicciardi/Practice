# -*- coding: utf-8 -*-

import time
import random
import math

import pygame

#############################################################################################
# Constantes
#############################################################################################

# Número de partículas que rodam na simulação
NUMBER_OF_PARTICLES = 30

# Oscilador Harmônico com Amortecimento: F = ma = -kx - cv,
# kx = força restauradora proporcional à posição e com sentido contrário
# cv = força amortecedora proporcional à velocidade e com sentido contrário

POSITION_FORCE_CONSTANT = 0.001 # => k
VELOCITY_FORCE_CONSTANT = 0.001 # => c

# dimensões da tela
SCREEN_WIDTH = 640 * 2
SCREEN_HEIGHT = 480 * 2
SCREEN_SIZE = (SCREEN_WIDTH, SCREEN_HEIGHT)

# radio de cada 'bolinha'
NODE_RADIUS = 10

# range das distâncias que as bolas podem ter entre uma e outra
DISTANCE_RANGE = (10, 1000)

#############################################################################################
# Classe Vetor
#############################################################################################

class Vector():

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def angleWith(self, v):
        return math.atan2(self.y - v.y, self.x - v.x)

    def __str__(self):
        return '({}, {})'.format(self.x, self.y)

    def __add__(self, v):
        return Vector(self.x + v.x, self.y + v.y)

    def __sub__(self, v):
        return Vector(self.x - v.x, self.y - v.y)

    def __mul__(self, k):
        return Vector(self.x * k, self.y * k)

    @staticmethod
    def distance(u, v):
        return math.hypot(u.x - v.x, u.y - v.y)

#############################################################################################
# Classe Partícula
#############################################################################################

class Particle():

    def __init__(self, x, y):
        self.position = Vector(x, y)
        self.velocity = Vector(0, 0)
        self.acceleration = Vector(0, 0)

        # para mover com o mouse
        self.is_selected = False

    def draw(self, screen):
        pygame.draw.circle(
            screen,
            pygame.Color('blue'),
            [ int(self.position.x), int(self.position.y) ],
            NODE_RADIUS,
            0   # 0 para que a bolinha fique preta, e não só o contorno
        )

#############################################################################################
# Grafo genérico
#############################################################################################

class Graph():

    class Node():

        def __init__(self, data):
            self.data = data
            self.neighbours = []

        def addNeighbour(self, node):
            self.neighbours.append(node)

    def __init__(self):
        self._nodes = []
        self._weights = {}

    def addNode(self, clientData):
        self._nodes.append( Graph.Node(clientData) )

    def addEdge(self, i, j, w):
        self._nodes[i].addNeighbour(j)
        if self._nodes[i].data not in self._weights:
            self._weights[self._nodes[i].data] = { self._nodes[j].data: w }
        else:
            self._weights[self._nodes[i].data][self._nodes[j].data] = w

    def nodes(self):
        for node in self._nodes:
            yield node.data

    def edges(self):
        for node in self._nodes:
            for neighbour in node.neighbours:
                yield (node.data, self._nodes[neighbour].data )

    def weight(self, u, v):
        return self._weights[u][v]

#############################################################################################
# Sistema de Partículas
#############################################################################################

class ParticleSystem(Graph):

    def __init__(self):
        super(ParticleSystem, self).__init__()

    def updateParticlePosition(self, particle):
        particle.velocity += particle.acceleration
        particle.position += particle.velocity
        particle.acceleration = Vector(0, 0)

    def updateParticles(self):
        for particle in self.nodes():
            self.updateParticlePosition(particle)
            if particle.is_selected:
                (x, y) = pygame.mouse.get_pos()
                mousePosition = Vector(x, y)
                particle.position = mousePosition

#############################################################################################
# Sistema de Molas
#############################################################################################

class SpringSystem():

    def __init__(self, k, c):
        self.k = k
        self.c = c

    def relativePosition(self, u, v):
        return u.position - v.position

    def relativeVelocity(self, u, v):
        return u.velocity - v.velocity

    def distanceVector(self, u, v, norm):
        angle = u.position.angleWith(v.position)
        return Vector(
            norm * math.cos(angle),
            norm * math.sin(angle)
        )

    def calculateSpringForce(self, u, v, distance):
        force = ( self.relativePosition(u, v) - self.distanceVector(u, v, distance) ) * self.k
        force += self.relativeVelocity(u, v) * self.c
        return force

    def applyForces(self, graph):
        for (u, v) in graph.edges():
            distance = min(graph.weight(u, v), graph.weight(v, u))
            v.acceleration += self.calculateSpringForce(u, v, distance)

#############################################################################################
# Driver
#############################################################################################

def main():
    # Inicia pygame
    pygame.init()

    # Variáveis da plataforma
    screen = pygame.display.set_mode(SCREEN_SIZE)
    clock = pygame.time.Clock()

    # Cria o sistema de molas
    springSystem = SpringSystem(
        k=POSITION_FORCE_CONSTANT,
        c=VELOCITY_FORCE_CONSTANT
    )

    # Monta o grafo com partículas
    particleSystem = ParticleSystem()
    for i in range(NUMBER_OF_PARTICLES):
        particleSystem.addNode(
            Particle(
                random.randrange(0, SCREEN_WIDTH),
                random.randrange(0, SCREEN_HEIGHT)
            )
        )
    for i in range(NUMBER_OF_PARTICLES):
        for j in range(NUMBER_OF_PARTICLES):
            if i != j:
                particleSystem.addEdge(
                    i, j,
                    random.randrange(DISTANCE_RANGE[0], DISTANCE_RANGE[1])
                )

    # loop principal
    done = False
    while not done:

        # Despacha os eventos
        for e in pygame.event.get():

            # evento de encerramento de janela
            if e.type == pygame.QUIT:
                done = True

            # evento de clique do mouse
            if e.type == pygame.MOUSEBUTTONDOWN:
                (x, y) = pygame.mouse.get_pos()
                mousePosition = Vector(x, y)
                for particle in particleSystem.nodes():
                    if Vector.distance(mousePosition, particle.position) < NODE_RADIUS:
                        particle.is_selected = True

            # evento de soltar o botão do mouse
            if e.type == pygame.MOUSEBUTTONUP:
                for particle in particleSystem.nodes():
                    particle.is_selected = False

        # Atualiza o sistema de forças e aplica as acelerações
        springSystem.applyForces(particleSystem)

        # Aplica as forças
        particleSystem.updateParticles()

        # limpa a tela
        screen.fill( pygame.Color('white') )

        # Renderiza as linhas das arestas
        for (u, v) in particleSystem.edges():
            pygame.draw.line(
                screen,
                pygame.Color('black'),
                [ int(u.position.x), int(u.position.y) ],
                [ int(v.position.x), int(v.position.y) ],
                1
            )

        # Renderiza as partículas
        for particle in particleSystem.nodes():
            particle.draw(screen)

        # 'flipa' o display
        pygame.display.flip()
        clock.tick(30)

    # Encerra
    pygame.quit()

if __name__ == '__main__':
    main()
