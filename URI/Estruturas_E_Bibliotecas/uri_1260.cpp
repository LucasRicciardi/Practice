// https://www.urionlinejudge.com.br/judge/pt/problems/view/1260

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <algorithm>

int main(int argc, char const *argv[])
{
	int n; std::cin >> n;
	std::string str; int total = 0;
	getchar();
	getline(std::cin, str);
	bool first = true;
	while (n--)
	{		
		std::unordered_map<std::string, int> tree;
		std::vector<std::string> v;

		if (not first ) { std::cout << '\n'; }
		first = false;

		while (getline(std::cin, str))
		{
			if (not str.size()) { break; }
			
			if (tree[str]) { tree[str]++; }
			else           { tree[str] = 1; v.push_back(str); }
			total++;
		}

		std::sort(v.begin(), v.end());
		std::cout.precision(4);
		for_each(v.begin(), v.end(), [total, &tree](std::string const& x)
		{
			std::cout << x << " " << std::fixed << tree[x]*100*1.0/total << '\n';
		});

		total = 0;
	}

	return 0;
}