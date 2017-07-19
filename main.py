# -*- coding: utf-8 -*-

import pygame
import time

white = (0xff, 0xff, 0xff)
blue = (0x00, 0x00, 0xff)


def main():
    pygame.init()

    pygame.display.set_caption('App')
    screen = pygame.display.set_mode([640, 480])

    sqr = pygame.Surface([30, 50])
    sqr_rect = sqr.get_rect()

    dx = 1
    dy = 2

    done = False
    while not done:

        for evt in pygame.event.get():

            if evt.type == pygame.QUIT:
                done = True
        
        sqr_rect.x += dx
        if sqr_rect.x < 0 or sqr_rect.x > 640:
            dx *= -1

        sqr_rect.y += dy
        if sqr_rect.y < 0 or sqr_rect.y > 480:
            dy *= -1

        screen.fill(white)

        screen.fill(blue, sqr_rect)

        pygame.display.flip()
    pygame.quit()

if __name__ == '__main__':
    main()