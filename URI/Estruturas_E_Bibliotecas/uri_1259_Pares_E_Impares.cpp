// https://www.urionlinejudge.com.br/judge/pt/problems/view/1259

#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char const *argv[])
{
	int n; std::cin >> n;
	
	auto foo = [] (int const& x, int const& y) { return x < y; };
	auto bar = [] (int const& x, int const& y) { return x > y; };

	std::vector<int> even;
	std::vector<int> odd;
	while (n--)
	{
		int x; std::cin >> x;
		
		if (x&1) { odd.push_back(x); }
		else     { even.push_back(x); }
	}

	std::sort(even.begin(), even.end(), foo);
	std::sort(odd.begin(), odd.end(), bar);

	for_each(even.begin(), even.end(), [] (int const& x) { std::cout << x << '\n'; });
	for_each(odd.begin(), odd.end(), [] (int const& x) { std::cout << x << '\n'; });

	return 0;
}