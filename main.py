# -*- coding: utf-8 -*-

import sys
import math

class StateMachine(object):

    def __init__(self, state):
        self.startState = state
    
    def name(self):
        return type(self).__name__

    def reset(self):
        self.state = self.startState

    def getNextValues(self, state, inp):
        nextState = self.nextState(state, inp)
        return (nextState, nextState)

    def done(self, state):
        return False

    def transduce(self, inp, verbose=False):
        self.reset()
        output = []
        if verbose:
            print('\n{}'.format(self.name()))
            print('Start state: {}'.format(self.state))
        for i in inp:
            if not self.done(self.state):
                (s, o) = self.getNextValues(self.state, i)
                if verbose:
                    print('In: {input} Out: {output} Next State: {state}'
                        .format(input=i, output=o, state=s))
                self.state = s
                output.append(o)
        return output

    def run(self, n=10, verbose=False):
        return self.transduce([None] * n, verbose)

class Cascade(StateMachine):

    def __init__(self, sm1, sm2):
        self.m1 = m1 = sm1
        self.m2 = m2 = sm2
        super(Cascade, self).__init__((m1.startState, m2.startState), name)

    def getNextValues(self, state, inp):
        (s1, s2) = state 
        (newS1, o1) = self.m1.getNextValues(s1, inp)
        (newS2, o2) = self.m2.getNextValues(s2, o1)
        return ((newS1, newS2), o2)

class RepeatUntil(StateMachine):

    def __init__(self, sm, condition):
        self.sm = sm
        self.condition = condition
        super(RepeatUntil, self).__init__((False, sm.startState))

    def getNextValues(self, state, inp):
        (condTrue, smState) = state
        (smState, o) = self.sm.getNextValues(smState, inp)
        condTrue = self.condition(inp)
        if self.sm.done(smState) and not condTrue:
            smState = self.sm.startState
        return((condTrue, smState), o)

    def done(self, state):
        (condTrue, smState) = state
        return self.sm.done(smState) and condTrue

class ConsumeFiveValues(StateMachine):

    def __init__(self, state=(0, 0)):
        super(ConsumeFiveValues, self).__init__(state)

    def getNextValues(self, state, inp):
        (count, total) = state
        if count == 4:
            return((count + 1, total + inp), total + inp)
        else:
            return((count + 1, total + inp), None)

    def done(self, state):
        (count, total) = state
        return count == 5

class Repeat(StateMachine):

    def __init__(self, sm, n=None):
        self.sm = sm
        self.n = n
        super(Repeat, self).__init__((0, sm.startState))

    def advanceIfDone(self, counter, smState):
        while self.sm.done(smState) and not self.done((counter, smState)):
            counter += 1
            smState = self.sm.startState
        return (counter, smState)

    def getNextValues(self, state, inp):
        (counter, smState) = state
        (smState, o) = self.sm.getNextValues(smState, inp)
        (counter, smState) = self.advanceIfDone(counter, smState)
        return ((counter, smState), o)

    def done(self, state):
        (counter, smState) = state
        return counter == self.n

class CharTSM(StateMachine):
    
    def __init__(self, c):
        self.c = c
        super(CharTSM, self).__init__(False)

    def getNextValues(self, state, inp):
        return (True, self.c)

    def done(self, state):
        return state

class Sequence(StateMachine):

    def __init__(self, smList):
        self.smList = smList
        self.n = len(smList)
        super(Sequence, self).__init__((0, smList[0].startState))

    def advanceIfDone(self, counter, smState):
        while self.smList[counter].done(smState) and counter + 1 < self.n:
            counter = counter + 1
            smState = self.smList[counter].startState
        return (counter, smState)

    def getNextValues(self, state, inp):
        (counter, smState) = state
        (smState, o) = self.smList[counter].getNextValues(smState, inp)
        (counter, smState) = self.advanceIfDone(counter, smState)
        return ((counter, smState), o)

    def done(self, state):
        (counter, smState) = state
        return self.smList[counter].done(smState)

def makeTextSequenceTSM(string):
    return Sequence([CharTSM(c) for c in string])

class RepeatUntil(StateMachine):

    def __init__(self, condition, sm):
        self.sm = sm
        self.condition = condition
        super(RepeatUntil, self).__init__((False, sm.startState))

    def getNextValues(self, state, inp):
        (condTrue, smState) = state
        (smState, o) = self.sm.getNextValues(smState, inp)
        condTrue = self.condition(inp)
        if self.sm.done(smState) and not condTrue:
            smState = self.sm.startState
        return ((condTrue, smState), o)

    def done(self, state):
        (condTrue, smState) = state
        return self.sm.done(smState) and condTrue

def main(argc, argv):
    a = CharTSM('a')
    print a.run(verbose=True)

    a4 = Repeat(a, 4)
    print a4.run(verbose=True)

    m = makeTextSequenceTSM('Hello, World !')
    print m.run(20, verbose=True)

    t = RepeatUntil(lambda x: x > 10, ConsumeFiveValues())
    print t.transduce(range(20), verbose=True)

if __name__ == '__main__':
    main(len(sys.argv), sys.argv)