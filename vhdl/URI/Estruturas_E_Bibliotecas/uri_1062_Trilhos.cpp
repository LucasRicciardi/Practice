//https://www.urionlinejudge.com.br/judge/pt/problems/view/1062


#include <iostream>
#include <stack>
#include <vector>

using namespace std;

void remove(vector<int>& priority, stack<int>& out )
{
	while (true)		
	{
		if ( out.empty() or priority.empty()) 		// Se as pilhas estiverem vazias, volta.		
		{
			return;
		}
		else if (out.top() not_eq priority.front())			// Se o topo da pilha não for igual ao do vetor, volta também.
		{
			return;
		}
		else
		{
			priority.erase(priority.begin());
			out.pop();
		}	
	}
}

bool verify(stack<int>& in, stack<int>& out, vector<int>& priority)
{
	while ( not in.empty() )
	{
		remove(priority, out);			// Retira as coisas dela.

		out.push(in.top()); 		// Insere o último na nova pilha
		in.pop();					// Remove o elemento da lista anterior		
	}

	if (not out.empty())			// Se a pilha ainda não estiver vazia,
		remove(priority, out);				// remove o que ainda tiver nela.

	return out.empty();				// Retorna true se a pilha de saída estiver vazia.
}

int main(int argc, char const *argv[])
{
	int n, m;

	while (true)
	{
		START:

		cin >> n;
		if (not n)
			return 0;

		while (true)
		{
			vector<int> priority;
			stack<int> in;
			stack<int> out;

			for (int i = n; i > 0; i--)
				in.push(i);

			for (int i = 0; i < n; i++)
			{
				int a; cin >> a;

				if (not a)
				{
					cout << endl;
					goto START;
				}

				priority.push_back(a);
			}
			
			if (verify(in, out, priority))
				cout << "Yes" << endl;
			else
				cout << "No" << endl;
		}
	}
}