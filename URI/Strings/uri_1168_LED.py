# https://www.urionlinejudge.com.br/judge/pt/problems/view/1168
# WARM UP !

import sys

size = [ 6, 2, 5, 5, 4, 5, 6, 3, 7, 6 ]
pair = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 ]

leds = zip(pair, size)	

def compare(number):

	qtd = 0

	for num, size in leds:
		if (number == str(num)):
			qtd += size

	return qtd		

if __name__ == "__main__":

	n = input()
	for i in range(n):
		
		number = raw_input()
		qtd = 0

		for n in number:
			qtd += compare(n)

		print '{} leds'.format(qtd)	



