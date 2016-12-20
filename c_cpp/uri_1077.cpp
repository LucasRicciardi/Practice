// https://www.urionlinejudge.com.br/judge/pt/problems/view/1077

#include <iostream>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

// Precedência de operadores
//
// Subtração e soma: 1
// Multiplicação e divisão: 2
// Exponenciação: 3

bool is_op(char c)
{
	string op = "+-*/^()";

	for (int i = 0; i < op.size(); i++)
		if (c == op[i])
			return true;

	return false;	
}

// Retorna false caso o operador
// a tenha precedência sobre b.
bool compare(char a, char b)
{
	// a é o topo da pilha

	if ( a == '(')
	{
		return true;
	}

	if ( a == '+' or a == '-')
	{
		if (b == '+' or b == '-')
			return false;
		else
			return true;
	}
	
	if ( a == '*' or a == '/')
	{
		if (b == '^' )
			return true;
		else
			return false;
	}

	if ( a == '^')
	{
		return false;
	}
}

void eval (stack<char>& v, char c)
{
	if (v.empty())	// Se não tem nada na stack, adiciona
	{
		v.push(c); return;
	}
	else
	{
		if (c == '(')			// Se for um parêntese, retorna
		{	
			v.push(c); return;
		}

		if (c == ')')			// Se for um paretênse fechado, elimina todos até o '('.
		{
			while (v.top() not_eq '(')
			{
				cout << v.top(); v.pop();
			}
			v.pop(); return;
		}

		while (true)
		{
			if (v.empty())
			{
				v.push(c); return;
			}

			if (compare(v.top(), c))
			{
				v.push(c); return;
			}

			cout << v.top(); v.pop();
		}
	}
}

void rpn(string s)
{
	stack<char> v;

	for (int i = 0; i < s.size(); i++)
	{
		char c =  s[i];

		if (is_op(c))
		{
			eval(v, c);
			continue;
		}
		else
			cout << c;
	}

	while (not v.empty())
	{
		if (v.top() == '(')
		{
			v.pop();
		}
		else
		{
			cout << v.top();
			v.pop();
		}
	}
}

int main(int argc, char const *argv[])
{
	int n; cin >> n;

	for (int i = 0; i < n; i++)
	{
		string s; cin >> s;

		rpn(s); cout << endl;
	}

	return 0;
}