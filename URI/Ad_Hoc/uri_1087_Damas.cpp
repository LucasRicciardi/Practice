// https://www.urionlinejudge.com.br/judge/pt/problems/view/1087

#include <iostream>
#include <cstdio>

int x1, x2, y1, y2;
int main()
{
	while (true)
	{
		scanf("%d %d %d %d", &x1, &y1, &x2, &y2);

		if (not(x1|x2|y1|y2)) { break; }

		// Se for igual, não anda nada.
		if      (!(x1^x2) and !(y1^y2)) { printf("0"); }
		else
		{
			int dx = (x1 - x2);
			int dy = (y1 - y2);

			// Se só se moveu em uma dimensão, um só.
			if      (!(dx^0) or !(dy^0)) { printf("1"); }
			// Ou se só se moveu na diagonal, ou seja, 
			// o mesmo tanto de casas para x e y.
			else if (!(abs(dx)^abs(dy))) { printf("1"); }
			// Se não, se moveu em alguma casa estranha, então 2.
			else                         { printf("2"); }
		}
		printf("\n");
	}

	return 0;
}