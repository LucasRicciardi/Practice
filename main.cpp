#include <iostream>
#include <cstdio>
#include <cmath>

int main()
{
	int x;
	std::cin >> x;

	while (x--)
	{
		int a, b;
		std::cin >> a >> b;

		for (int i = a; i <= b; i++)
		{	
			if (i == 2) { printf("%d\n", i); }
			else if (i == 1 or !(i&1)) { continue; }
			else
			{				
				bool prime = true;
				int k = sqrt(i);
				for (int j = 3; j < i; j = j + 2)
				{
					if (j > k) { break; }

					if (i % j == 0) { prime = false; } 
				}
				if (prime) { printf("%d\n", i); }
			}
		}

		printf("\n");
	}

	return 0;
}