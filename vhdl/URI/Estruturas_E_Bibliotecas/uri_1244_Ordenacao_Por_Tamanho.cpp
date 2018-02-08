// https://www.urionlinejudge.com.br/judge/pt/problems/view/1244

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>

int main(int argc, char const *argv[])
{
	int n; std::cin >> n;
	getchar();
	while (n--)
	{
		std::vector< std::string > v;
		std::string str;
		getline(std::cin, str);

		std::istringstream iss(str);
		std::string token;

		while (std::getline(iss, token, ' '))
		{
			v.push_back(token);
		}

		std::stable_sort(v.begin(), v.end(), [] (std::string const& a, std::string const& b)
		{ 
			return a.size() > b.size();
		});

		for_each(v.begin(), v.end() - 1, [] (std::string& s) { std::cout << s << " "; });
		std::cout << v.back() << '\n';
	}

	return 0;
}