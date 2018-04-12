# -*- coding: utf-8 -*-

def newton_raphson(x, max_iters=100):
    result = x / 2
    for _ in range(max_iters):
        result = (result + (x / result)) * 0.5
    return result

def bisection(x, max_iters=100):
    def f(n):
        return n ** 2 - x        
    def sign(x):
        return abs(x) / x
    (a, b) = (0, x + 1)    
    for _ in range(max_iters):
        c = (a + b) / 2
        if f(c) == 0:
            return c
        if sign(f(c)) == sign(f(a)):
            a = c
        else:
            b = c
    return c

def secant(x, max_iters=100):
    def f(n):
        return n ** 2 - x
    (a, b) = (0, x+1)
    for _ in range(max_iters):
        try:
            c = b - f(b) * ((b-a) / (f(b) - f(a)))            
        except ZeroDivisionError:
            continue
        (a, b) = (b, c)
    return c

def main():
    done = False
    while not done:
        x = input('Digite um número (ou "q" para sair) --> ')
        if x == 'q':
            done = True
        else:            
            x = float(x)
            n = 1000
            for method in [ newton_raphson, bisection, secant ]:
                message = '     Raíz de {} por {} com {} iterações = {}'.format(x, method.__name__, n, method(x, n))
                print(message)

if __name__ == '__main__':
    main()