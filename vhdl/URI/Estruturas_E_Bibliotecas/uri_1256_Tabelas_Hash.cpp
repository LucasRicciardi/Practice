// https://www.urionlinejudge.com.br/judge/pt/problems/view/1256

#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char const *argv[])
{
	bool first = true;
	int n; std::cin >> n;
	while (n--)
	{
		if (not first) { std::cout << '\n'; }
		first = false;

		int a, b; std::cin >> a >> b;

		auto bar = [a] (int const x) -> int { return x % a; };
		std::vector< std::vector<int> > v(a);
		for (int i = 0; i < b; i++)
		{
			int x; std::cin >> x;
			v.at(bar(x)).push_back(x);
		}

		for (int i = 0; i < v.size(); i++)
		{
			std::cout << i;
			for (int j = 0; j < v[i].size(); j++)
			{
				std::cout << " -> " << v[i][j];
			}

			std::cout << " -> \\" << std::endl;
		}
	}

	return 0;
}