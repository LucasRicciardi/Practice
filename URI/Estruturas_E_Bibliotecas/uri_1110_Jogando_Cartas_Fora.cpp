#include <iostream>
#include <queue>

using namespace std;

int main(int argc, char const *argv[])
{
	while (true)
	{
		int n; cin >> n;
		
		if (not n) { return 0; }

		queue<int> q;

		for (int i = 1; i <= n; i++)
			q.push(i);

		cout << "Discarded cards: ";
		while (q.size() > 1)
		{
			cout << q.front();
			if (q.size() > 2)
				cout << ", ";
			else
				cout << endl;

			q.pop();
			int n = q.front();
			q.pop(); q.push(n);
		}
		cout << "Remaining card: " << q.front() << endl;

	}

	return 0;
}