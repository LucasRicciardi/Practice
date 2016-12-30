// https://www.urionlinejudge.com.br/judge/pt/problems/view/1258

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

int main(int argc, char const *argv[])
{
	
	bool first = true;
	while (true)
	{
		int n; std::cin >> n;
		getchar();
		if (not n) { break; }

		if (not first) { std::cout << '\n'; }
		first = false;

		using key = std::tuple<std::string, std::string, std::string>;
		std::vector< key > v;
		for (int i = 0; i < n; i++)
		{
			std::string foo, bar, gnar;
			getline(std::cin, foo);
			std::cin >> bar >> gnar;
			getchar();
			v.push_back(std::make_tuple(foo, bar, gnar));
		}
	
		std::sort(v.begin(), v.end(), [] (key const& a, key const& b) -> bool
		{
			std::string foo = std::get<1>(a), bar = std::get<1>(b);
			if (foo not_eq bar) { return foo < bar; }

			foo = std::get<2>(a); bar = std::get<2>(b);
			if (foo not_eq bar) { return foo > bar; }	

			return std::get<0>(a) < std::get<0>(b);	
		});

		for_each(v.begin(), v.end(), [] (key const& e)
		{
			std::cout << std::get<1>(e) << " " << std::get<2>(e) << " " << std::get<0>(e) << '\n';
		});
	}

	return 0;
}