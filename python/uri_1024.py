# https://www.urionlinejudge.com.br/judge/pt/problems/view/1024

import sys

def primeira(text):

	first = ""

	for t in text:
		if t.isalpha():
			first += chr(ord(t) + 3)
		else:
			first += t	
		
	segunda(first)

	pass	

def segunda(text):
	
	second = text[::-1]
	terceira(second)

	pass

def terceira(text):
	
	tird = ""

	half = len(text) / 2
	i = 0
	for t in text:
		if (i < half):
			tird += t
			i += 1
		else:	
			tird += chr(ord(t) - 1)
	
	print tird	
	pass


n = input()

for x in xrange(0,n):

	text = raw_input()
	primeira(text)
pass

