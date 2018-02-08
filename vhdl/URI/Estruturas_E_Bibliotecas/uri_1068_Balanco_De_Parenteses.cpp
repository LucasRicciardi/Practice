// https://www.urionlinejudge.com.br/judge/pt/problems/view/1068

#include <iostream>
#include <stack>

using namespace std;

bool check(string& c)
{
	stack<int> s;

	for (int j = 0; j < c.size(); j++)
	{
		char a = c.at(j);

		if (a == '(')
			s.push(1);

		if (a == ')')
			if (s.empty())
				return false;
			else
				s.pop();
	}

	return s.empty();
}

int main(int argc, char const *argv[])
{
	string c;

	while (getline(cin, c))
	{
		if (check(c))
			cout << "correct" << endl;
		else
			cout << "incorrect" << endl;

	}

	return 0;
}
