// https://www.urionlinejudge.com.br/judge/pt/problems/view/1261

#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <algorithm>

int main(int argc, char const *argv[])
{
	int m, n; std::cin >> m >> n;

	std::unordered_map<std::string, unsigned long> dict;
	while (m--)
	{
		std::string str; unsigned long x;
		std::cin >> str >> x;
		dict[str] = x;
	}
		
	while (n--)
	{
		unsigned long salary = 0;
		while(true)
		{
			std::string str; std::cin >> str;
			if (str == ".") { break; }

			if (dict[str]) { salary += dict[str]; }
		}

		std::printf("%lu\n", salary);
	}

	return 0;
}