# -*- coding: utf-8 -*-

import collections
import logging

import pygame

#########################################################################################
# Sistemas
#########################################################################################

class System():

    def init(self, world):
        pass

    def update(self, world, debug):
        pass

    def close(self, world):
        pass


#########################################################################################
# Mundo
#########################################################################################

class World():

    CONTROL_SYSTEM = 0
    UPDATE_SYSTEM = 1
    RENDER_SYSTEM = 2

    def __init__(self, size):
        # Dimensões do 'mundo'
        self.size = (self.w, self.h) = size

        # Sistemas
        self.control_systems = collections.OrderedDict()
        self.update_systems = collections.OrderedDict()
        self.render_systems = collections.OrderedDict()
        self.systems = collections.ChainMap(
            self.control_systems,
            self.update_systems,
            self.render_systems
        )

        # Objetos
        self.objects = []

        # logger
        logging.basicConfig(
            level=logging.INFO,
            format='%(asctime)s %(name)s => %(message)s',
            datefmt='[%H:%M:%S]'
        )
        self.logger = logging.getLogger(self.__class__.__name__)

    def add_system(self, name, system, system_type):
        # Sistemas de controle (input)
        if system_type is World.CONTROL_SYSTEM:
            self.control_systems[name] = system

        # Sistemas de atualização
        elif system_type is World.UPDATE_SYSTEM:
            self.update_systems[name] = system

        # Sistemas de amostragem (output)
        elif system_type is World.RENDER_SYSTEM:
            self.render_systems[name] = system

    def start_simulation(self, debug=False):
        # Inicia todos os sistemas
        self.logger.info('Iniciando sistemas ...')
        for system in self.systems.values():
            self.logger.info('Iniciando sistema: {}'.format(system.__class__.__name__))
            system.init(self)
        self.logger.info('Sistemas inicializados ...')

        # Atualiza os sistemas
        self.logger.info('Iniciando simulação ...')
        self.done = False
        while not self.done:
            try:
                # Primeiro os sistemas de controle
                for system in self.control_systems.values():
                    system.update(self, debug)

                # Depois os sistemas de atualização
                for system in self.update_systems.values():
                    system.update(self, debug)

                # Por último os sistemas de renderização
                for system in self.render_systems.values():
                    system.update(self, debug)

            except KeyboardInterrupt:
                self.logger.info('Simulação interrompida por "Ctrl+C"')
                self.done = True

        # Encerra os sistemas
        self.logger.info('Encerrando os sistemas ...')
        for system in self.systems.values():
            system.close(self)
        self.logger.info('Encerrando a simulação ...')

def main():
    # Cria um objeto 'mundo'
    world = World(size=(640, 480))

    # Adiciona sistemas ao mundo

    # Sistemas de input
    world.add_system('event', EventSystem(), world.CONTROL_SYSTEM)

    # Sistemas de output
    world.add_system('render', RenderSystem(), world.RENDER_SYSTEM)

    world.start_simulation(debug=True)

if __name__ == '__main__':
    main()
