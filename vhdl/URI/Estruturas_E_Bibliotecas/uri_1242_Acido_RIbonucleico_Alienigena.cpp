// https://www.urionlinejudge.com.br/judge/pt/problems/view/1242

#include <iostream>
#include <stack>
#include <algorithm>

bool match(char a, char b)
{
	if (a == 'B' and b == 'S') { return true; }
	if (a == 'C' and b == 'F') { return true; }

	return false;
}

int main(int argc, char const *argv[])
{
	std::string str;
	int matches = 0;
	while (std::cin >> str)
	{
		getchar();
		std::stack<char> s;
		for_each(str.begin(), str.end(), [&](char& c)
		{
			if (s.empty()) { s.push(c); }
			else
			{
				if (match(s.top(), c) or match(c, s.top()))
				{
					matches++; s.pop();
				}
				else { s.push(c); }
			}
		});

		std::cout << matches << '\n';
		matches = 0;
	}
	
	return 0;
}