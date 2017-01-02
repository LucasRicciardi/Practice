// https://www.urionlinejudge.com.br/judge/pt/problems/view/1251

#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>

int main(int argc, char const *argv[])
{
	std::string str;
	bool b = false;
	while (getline(std::cin, str))
	{
		if (b) { std::cout << '\n'; }
		b = true;

		std::vector< std::pair<char, int> > v;
		for_each(str.begin(), str.end(), [&] (char& c)
		{
			auto res = std::find_if(v.begin(), v.end(), [&] (std::pair<char, int> const& el)
			{
				if (c == el.first) { return true; }
				else               { return false; }
			});

			if (res == v.end()) { v.push_back(std::make_pair(c, 1)); }
			else                { (*res).second++; }
		});

		std::sort(v.begin(), v.end(), [] (std::pair<char, int> const& a, std::pair<char, int> const& b)
		{
			if (a.second == b.second) { return a.first > b.first; }
			else                      { return a.second < b.second; }
		});

		for_each(v.begin(), v.end(), [] (std::pair<char, int> const& el)
		{
			std::cout << int(el.first) << " " << el.second << '\n';
		});
	}

	return 0;
}