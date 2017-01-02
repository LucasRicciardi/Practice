// https://www.urionlinejudge.com.br/judge/pt/problems/view/1281

#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <algorithm>

int main(int argc, char const *argv[])
{
	int n; scanf("%d", &n);
	while (n--)
	{
		int m; scanf("%d", &m);
		std::unordered_map<std::string, double> f;
		std::string fruit;
		double price;
		while (m--)
		{
			std::cin >> fruit >> price;
			f[fruit] = price;
		}

		scanf("%d", &m);
		double total = 0.f;
		while (m--)
		{
			std::cin >> fruit >> price;
			total += f[fruit] * price;
		}

		printf("R$ %.2f\n", total);
	}

	return 0;
}
