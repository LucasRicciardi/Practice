# -*- coding: utf-8 -*-

# Referência
# https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-01sc-introduction-to-electrical-engineering-and-computer-science-i-spring-2011/unit-1-software-engineering/state-machines/MIT6_01SCS11_chap04.pdf

def safeAdd(a, b):
    if a == 'undefined' or b == 'undefined':
        return 'undefined'
    else:
        return a + b

def splitValue(v):
    if v == 'undefined':
        return ('undefined', 'undefined')
    else:
        return v

class StateMachine(object):
    def __init__(self, state = None, name = 'StateMachine'):
        self.startState = state
        self.name = name
        self.start()

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

    def run(self, n = 10, verbose = False):
        return self.transduce([None] * n, verbose)

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

# Delay
class Delay(StateMachine):
    def __init__(self, state = 0):
        super(Delay, self).__init__(state, 'Delay')

    def getNextValues(self, state, inp):
        return (inp, state)

# Média 2
class Average2(StateMachine):
    def __init__(self, state = 0):
        super(Average2, self).__init__(state, 'Average2')

    def getNextValues(self, state, inp):
        return (inp, (inp + state) / 2.0)

# SumLast3
class SumLast3(StateMachine):
    def __init__(self, state = (0, 0)):
        super(SumLast3, self).__init__(state, 'SumLast3')

    def getNextValues(self, state, inp):
        (previousPreviousInput, previousInput) = state
        return ((previousInput, inp),
                previousPreviousInput + previousInput + inp)

# SimpleParkingGate
class SimpleParkingGate(StateMachine):
    def __init__(self, state = 'waiting'):
        super(SimpleParkingGate, self).__init__(state, 'SimpleParkingGate')

    def generateOutput(self, state):
        if state == 'raising':
            return 'raise'
        elif state == 'lowering':
            return 'lower'
        else:
            return 'nop'

    def getNextValues(self, state, inp):
        (gatePosition, carAtGate, carJustExited) = inp
        if state == 'waiting' and carAtGate:
            nextState = 'raising'
        elif state == 'raising' and gatePosition == 'top':
            nextState = 'raised'
        elif state == 'raised' and carJustExited:
            nextState = 'lowering'
        elif state == 'lowering' and gatePosition == 'bottom':
            nextState = 'waiting'
        else:
            nextState = state
        return (nextState, self.generateOutput(nextState))

# Increment
class Increment(StateMachine):
    def __init__(self, incr, state = None):
        self.incr = incr
        super(Increment, self).__init__(state, 'Increment')

    def getNextState(self, state, inp):        
        return safeAdd(inp, self.incr)

# Cascade
class Cascade(StateMachine):
    def __init__(self, sm1, sm2):
        self.m1 = m1 = sm1
        self.m2 = m2 = sm2
        super(Cascade, self).__init__((m1.state, m2.state), 'Cascade')

    def getNextValues(self, state, inp):
        (s1, s2) = state
        (newS1, o1) = self.m1.getNextValues(s1, inp)
        (newS2, o2) = self.m2.getNextValues(s2, o1)
        return ((newS1, newS2), o2)

# Parallel
class Parallel(StateMachine):
    def __init__(self, sm1, sm2):
        self.m1 = m1 = sm1
        self.m2 = m2 = sm2
        super(Parallel, self).__init__((m1.state, m2.state), 'Parallel')

    def getNextValues(self, state, inp):
        (s1, s2) = state
        (newS1, o1) = self.m1.getNextValues(s1, inp)
        (newS2, o2) = self.m2.getNextValues(s2, inp)
        return ((newS1, newS2), (o1, o2))

# Parallel2
class Parallel2(Parallel):
    def getNextValues(self, state, inp):
        (s1, s2) = state
        (i1, i2) = splitValue(inp)
        (newS1, o1) = self.m1.getNextValues(s1, i1)
        (newS2, o2) = self.m2.getNextValues(s2, i2)
        return ((newS1, newS2), (o1, o2))

# Parallel Add
class ParallelAdd(Parallel):
    def getNextValues(self, state, inp):
        (s1, s2) = state
        (newS1, o1) = self.m1.getNextValues(s1, inp)
        (newS2, o2) = self.m2.getNextValues(s2, inp)
        return ((newS1, newS2), o1 + o2)

# Feedback
class Feedback(StateMachine):
    def __init__(self, sm):
        self.m = m = sm
        super(Feedback, self).__init__(sm.state, 'Feedback')        

    def getNextValues(self, state, inp):
        (ignore, o) = self.m.getNextValues(state, 'undefined')
        (newS, ignore) = self.m.getNextValues(state, o)
        return (newS, o)

# Feedback 2
class Feedback2(Feedback):
    def getNextValues(self, state, inp):
        (ignore, o) = self.m.getNextValues(state, (inp, 'undefined'))
        (newS, ignore) = self.m.getNextValues(state, (inp, o))
        return (newS, o)

# Counter
def makeCounter(init, step):
    return Feedback(
                Cascade(
                    Increment(step), Delay(init)))

# Adder
class Adder(StateMachine):
    def getNextState(self, state, inp):      
        (i1, i2) = splitValue(inp)
        return safeAdd(i1, i2)

# Switch
class Switch(StateMachine):
    def __init__(self, condition, sm1, sm2):
        self.m1 = m1 = sm1
        self.m2 = m2 = sm2
        self.condition = condition
        super(Switch, self).__init__((m1.state, m2.state))

    def getNextValues(self, state, inp):
    (s1, s2) = state
    if self.condition(inp):
        (ns1, o) = self.m1.getNextValues(s1, inp)
        return ((ns1, s2), o)
    else:
        (ns2, o) = self.m2.getNextValues(s2, inp)
        return ((s1, ns2), o)

# Multiplex
class Multiplex(Switch):
    def getNextValues(self, state, inp):
        (s1, s2) = state
        (ns1, o1) = self.m1.getNextValues(s1, inp)
        (ns2, o2) = self.m2.getNextValues(s2, inp)
        if self.condition(inp):
            return ((ns1, ns2), o1)
        else:
            return ((ns1, ns2), o2)

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

    # Delay
    d = Delay(7)
    print d.transduce([3, 1, 2, 5, 9], verbose = True)

    # Média 2
    a2 = Average2()
    print a2.transduce([10, 5, 2, 10], verbose = True)

    # Sum Last 3
    sl3 = SumLast3()
    print sl3.transduce([2, 1, 3, 4, 10, 1, 2, 1, 5], verbose = True)

    # Simple Parking Gate
    spg = SimpleParkingGate()
    print spg.transduce(
    [
        ('bottom', False, False),
        ('bottom', True, False),
        ('bottom', True, False),
        ('middle', True, False),
        ('middle', True, False),
        ('middle', True, False),
        ('top', True, False),
        ('top', True, False),
        ('top', True, False),
        ('top', True, True),
        ('top', True, True),
        ('top', True, False),
        ('middle', True, False),
        ('middle', True, False),
        ('middle', True, False),
        ('bottom', True, False),
        ('bottom', True, False)
    ], verbose = True)

    # Counter -> Feedback(Cascade(Increment, Delay))
    c = makeCounter(3, 2)
    print c.run(verbose = True) 

    # Fibonacci
    fib = Feedback(
            Cascade(
                Parallel(
                    Delay(1),
                    Cascade(
                        Delay(1), Delay(0)
                        )),
                Adder()
                ))
    print fib.run(verbose = True)
