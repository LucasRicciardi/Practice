// https://www.urionlinejudge.com.br/judge/pt/problems/view/1069

#include <iostream>
#include <stack>

using namespace std;

int check(string& c)
{
	stack<int> s;
	int i = 0;

	for (int j = 0; j < c.size(); j++)
	{
		char a = c.at(j);

		if (a == '<')
			s.push(1);

		if (a == '>' and not s.empty())
		{
			s.pop();
			i++;
		}
	}

	return i;
}

int main(int argc, char const *argv[])
{
	int n; cin >> n;

	for (int i = 0; i < n; i++)
	{
		string c; cin >> c;

		cout << check(c) << endl;

	}

	return 0;
}