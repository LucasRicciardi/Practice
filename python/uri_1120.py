# https://www.urionlinejudge.com.br/judge/pt/problems/view/1120

import sys

while True:

	values = raw_input()

	d = values[0]
	n = ""

	for v in values[2:]:
		n += v

	if (d == '0' and n == '0'):
		break

	out = ""	
	for i in n:
		if (i != d):
			out += i

	if not out:
		out = '0'		

	print int(out)		
