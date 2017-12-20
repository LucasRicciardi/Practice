# -*- coding: utf-8 -*-

import pygame

import math
import time
import random

# Tamanho da tela
SCREEN_SIZE = (640*2, 480*2)

# Número de leds na fita
NUM_OF_LEDS = 5

PERIOD_RANGE = (200, 1000)

# '  #     #  ',
# '   #   #   ',
# '  #######  ',
# ' ## ### ## ',
# '###########',
# '# ####### #',
# '# #     # #',
# '   ## ##   ',

########################################################################################
# Padrões de desenho, canvas 5x5
########################################################################################

SPACE_INVADERS_PATTERN = [

    '  #     #       #     #       #     #  ',
    '   #   #         #   #         #   #   ',
    '  #######       #######       #######  ',
    ' ## ### ##     ##-###-##     ## ### ## ',
    '###########   ###########   ###########',
    '# ####### #   # ####### #   # ####### #',
    '# #     # #   # #     # #   # #     # #',
    '   ## ##         ## ##         ## ##   ',

]

########################################################################################
# Canvas
########################################################################################

class Canvas():

    def __init__(self, radius, r_partitions, w_partitions, pattern):
        # Raio
        self.radius = radius

        # Tamanho das partições
        self.r_partitions = r_partitions
        self.w_partitions = w_partitions

        # Intervalo de cada partição
        self.dr = int(self.radius / self.r_partitions)
        self.dw = int(360 / self.w_partitions)

        # Período e fase
        self.update_period(10)
        self.phase = 0

        # Padrão a ser renderizado
        self.pattern = pattern

        # Limpa a tela
        self.clean_screen = False

    def update_period(self, new_period):
        self.period = new_period / 1000 if new_period > 0 else 10 / 1000
        self.d_phase = math.degrees(-2 * math.pi * (1/self.period))
        print('Período de Oscilação = {}s'.format(self.period))

    def update(self, dt):
        elapsed_time = dt * (10**-3) # ms -> s

        # Atualiza a posição do fasor
        self.phase += self.d_phase * elapsed_time

        # Verifica se é hora de mudar o período
        if self.phase < -360:
            self.update_period(
                random.randrange(
                    PERIOD_RANGE[0],
                    PERIOD_RANGE[1]
                )
            )
            self.phase = -(-self.phase % 360)
            self.clean_screen = True

    def render(self, screen, debug=False):
        (w, h) = screen.get_size()                  # Dimensões da tela
        center = (x0, y0) = [ int(w/2), int(h/2) ]  # Centro da tela

        # Limpa a tela
        if self.clean_screen:
            self.clean_screen = False
            screen.fill(
                pygame.Color('white')
            )

        # Renderiza o círculo do 'mundo'
        self.draw_canvas(screen, center, debug)

        # Renderiza o fasor posição
        if debug:
            self.draw_fasor(screen, center)

        # Renderiza o padrão pré programado
        self.draw_pattern(screen, center)

    def draw_fasor(self, screen, center):
        (x0, y0) = center
        phase = math.radians(self.phase)
        intersection = [
            int(x0 + self.radius * math.cos(phase)),
            int(y0 + self.radius * math.sin(phase))
        ]

        # Renderiza a reta do vetor de fase
        pygame.draw.line(
            screen,
            pygame.Color('red'),
            center,
            intersection,
            1 # width
        )

    def draw_pattern(self, screen, center):
        (x0, y0) = center
        i = -int(self.phase/self.dw)
        try:
            for j in range( len(self.pattern) ):

                # Raio é constante
                radius = 13

                # position são as coordenadas em função de (i,j)
                tetha = math.radians(-(i+0.5)*self.dw)
                # tetha = math.radians(self.phase)
                position = [
                    int(x0 + ((j+7) * self.dr) * math.cos(tetha) ),
                    int(y0 + ((j+7) * self.dr) * math.sin(tetha) )
                ]

                if self.pattern[j][i] == '#':
                    pygame.draw.circle(
                        screen,
                        pygame.Color('blue'),
                        position,
                        radius,
                        0 # width
                    )
        except IndexError:
            pass

    def draw_canvas(self, screen, center, debug):
        # Centro do círculo
        (x0, y0) = center

        # Renderiza o círculo
        pygame.draw.circle(
            screen,
            pygame.Color('black'),
            center,
            self.radius,
            1 # width
        )

        # Renderiza as coordenadas polares
        if debug:
            # Renderiza todos os eixos do versor tetha
            for i in range(self.w_partitions+1):
                # Ângulo
                tetha = math.radians(self.dw * i)

                # intersecção da reta com o círculo do 'mundo'
                intersection = [
                    int(x0 + self.radius * math.cos(tetha)),
                    int(y0 + self.radius * math.sin(tetha))
                ]

                # Renderiza a reta
                pygame.draw.line(
                    screen,
                    pygame.Color('black'),
                    center,
                    intersection,
                    1 # width
                )

            # Renderiza os eixos do versor r
            for i in range(self.r_partitions):
                pygame.draw.circle(
                    screen,
                    pygame.Color('black'),
                    center,
                    self.dr * i,
                    1 if self.dr * i > 0 else 0 # width
                )

########################################################################################
# Driver
########################################################################################

def main():
    # Inicia pygame
    pygame.init()

    # Tela e clock
    screen  = pygame.display.set_mode(SCREEN_SIZE)
    screen.fill(
        pygame.Color('white')
    )
    clock = pygame.time.Clock()

    # Canvas circular
    canvas = Canvas(
        radius=500,
        r_partitions=20,
        w_partitions=50,
        pattern=SPACE_INVADERS_PATTERN
    )

    def millis():
        return time.time() * 1000

    # Variáveis de tempo
    updates_per_second = 30
    update_interval = 1000 / updates_per_second
    last_update = millis()

    # Limpeza de tela
    clean_interval = len(canvas.pattern) * 1000
    last_screen_clean = millis()

    seconds_passed = 0
    ticks = 0

    # Entra no loop principal
    done = False
    while not done:
        try:
            ########################################################################################
            # Eventos
            ########################################################################################
            for evt in pygame.event.get():
                if evt.type == pygame.QUIT:
                    done = Trues

            ########################################################################################
            # Atualização
            ########################################################################################

            dt = millis() - last_update
            canvas.update(dt)
            last_update = millis()

            ########################################################################################
            # Renderização
            ########################################################################################

            # Renderiza o canvas circular e a fita de leds
            canvas.render(screen, debug=True)

            # Renderiza os leds em função do tempo
            # Atualiza o display
            pygame.display.flip()

        except KeyboardInterrupt:
            done = True

    # Encerra tudo
    pygame.quit()

if __name__ == '__main__':
    main()
