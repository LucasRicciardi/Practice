// https://www.urionlinejudge.com.br/judge/pt/problems/view/1251

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int main(int argc, char const *argv[])
{
	int n, m;	
	while (true)
	{	
		std::cin >> n >> m;
		if (!(n|m)) { break; }

		std::vector<int> v(n);
		for_each(v.begin(), v.end(), [] (int& x)
		{
			std::cin >> x;
		});

		auto foo = [m] (int const& x, int const& y) -> bool
		{
			int m_x = x % m, m_y = y % m;

			if (!(m_x ^ m_y))  // NXOR -> igual
			{
				int A = (x&1), B = (y&1);

				if      (A&B)    { return x > y; }        // Ímpares
				else if (!(A|B)) { return x < y; }        // Pares
				else             { return A^0; }          // Um par e um ímpar

			}
			else { return m_x < m_y; }                   // Se não o menor módulo
		};

		// true -> x && false -> y
		std::sort(v.begin(), v.end(), foo);
		std::cout << n << " " << m << std::endl;
		for_each(v.begin(), v.end(), [] (int& x)
		{
			std::cout << x << std::endl;
		});
	}

	std::cout << "0 0" << std::endl;
	return 0;
}