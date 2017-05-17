# -*- coding: utf-8 -*-

# Referência
# https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-01sc-introduction-to-electrical-engineering-and-computer-science-i-spring-2011/unit-1-software-engineering/state-machines/MIT6_01SCS11_chap04.pdf

class StateMachine(object):

    def __init__(self, state = None, name = 'StateMachine'):
        self.startState = state
        self.name = name

    def start(self):
        self.state = self.startState

    def step(self, inp):
        (s, o) = self.getNextValues(self.state, inp)
        self.state = s
        return o

    def getNextValues(self, state, inp):
        nextState = self.getNextState(state, inp)
        return (nextState, nextState)

    def transduce(self, inputs, verbose = False):
        self.start()
        output = []
        if verbose:
            print('\n{}'.format(self.name))
            print('Start state: {state}'.format(state = self.state))
        for inp in inputs:
            o = self.step(inp)
            if verbose:
                print('In: {input} Out: {output} Next State: {state}'.format(
                    input = inp, output = o, state = self.state))
            output.append(o)
        return output

    def run(self, n = 10):
        return self.transduce([None] * n)

# Acumulador
class Acumulator(StateMachine):

    def __init__(self, state = 0):
        super(Acumulator, self).__init__(state, 'Acumulator')

    def getNextState(self, state, inp):
        return state + inp

    # Essa acima, ou
    # def getNextValues(self, state, inp)
    #   return state + inp, state + inp

# Ganho
class Gain(StateMachine):

    def __init__(self, k):
        super(Gain, self).__init__(None, 'Gain')
        self.k = k

    def getNextState(self, state, inp):
        return inp * self.k

# 'Language Acceptor'
class ABC(StateMachine):

    def __init__(self, state = 0):
        super(ABC, self).__init__(state, 'ABC')

    def getNextValues(self, state, inp):
        if   state == 0 and inp == 'a':
            return (1, True)
        elif state == 1 and inp == 'b':
            return (2, True)
        elif state == 2 and inp == 'c':
            return (0, True)
        else:
            return (3, False)

# UpDown
class UpDown(StateMachine):

    def __init__(self, state = 0):
        super(UpDown, self).__init__(state, 'UpDown')

    def getNextState(self, state, inp):
        if inp == 'u':
            return state + 1
        else:
            return state - 1

if __name__ == '__main__':
    # Acumulador
    a = Acumulator()
    print a.transduce([100, -3, 4, -123, 10], verbose = True)

    # Ganho
    g = Gain(3)
    print g.transduce([1.1, -2, 100, 5 ], verbose = True)

    # ABC
    abc = ABC()
    print abc.transduce(['a', 'a', 'a'], verbose = True)

    # UpDown
    ud = UpDown()
    print ud.transduce(['u', 'u', 'u', 'd', 'd', 'u'], verbose = True)
    