// https://www.urionlinejudge.com.br/judge/pt/problems/view/1063

#include <iostream>
#include <stack>
#include <queue>

using namespace std;

void read(queue<string>& s, int n)
{
	for (int i = 0; i < n; i++)
	{
		string a; cin >> a;
		s.push(a);
	}	
}

void remove(stack<string>& s, queue<string>& v)
{
	while (true)
	{
		if (s.empty() or v.empty())				// Se algum dos dois estiverem vazios, volte;
		{
			return;
		}
		else if ( s.top() not_eq v.front()) 
		{
			return;
		}
		else
		{
			v.pop();
			s.pop();
			cout << "R";
		}
	}
}

void insere(stack<string>& s, queue<string>& r)
{
	s.push(r.front());
	r.pop();
	cout << "I";
}

bool verify(queue<string>& s, queue<string>& v)
{
	stack<string> out;

	while (not s.empty())		// Enquanto a entrada não estiver vazia
	{
		remove(out, v);			// Remova os elementos v da saída 
		insere(out, s);
	}

	if (not out.empty())		// Se a saída ainda não estiver vazia
		remove(out, v);

	return out.empty();
}

int main(int argc, char const *argv[])
{
	int n;

	while (true)
	{
		cin >> n;

		if (not n)
			return 0;

		queue<string> 	in;
		queue<string> out;

		read(in, n); read(out, n);

		if ( not verify(in, out))
			cout << " Impossible";

		cout << endl;	 
	}

	return 0;
}