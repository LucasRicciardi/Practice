// https://www.urionlinejudge.com.br/judge/pt/problems/view/1112

#include <cstdio>
#include <algorithm>

using namespace std;

class BIT
{
public:
	BIT() { }
	~BIT() { }
	
	void zero(int x, int y)
	{
		this->xSize = x; this->ySize = y;

		for (int i = 0; i <= this->xSize; i++)
			for (int j = 0; j <= this->ySize; j++)
				this->v[i][j] = 0;
	}

	void update(int x, int y, int value)
	{
		x++; y++;
		for (int i = x; i <= this->xSize; i += i & (-i))
			for (int j = y; j <= this->ySize; j += j & (-j))
				this->v[i][j] += value;
	}

	int sum(int x, int y)
	{
		int sum = 0; x++; y++;
		for (int i = x; i >0 ; i -= i & (-i))
			for (int j = y; j > 0; j -= j & (-j))
				sum += this->v[i][j];		
		
		return sum;
	}

	int query(int x1, int y1, int x2, int y2)
	{
		if (x1 > x2) { swap(x1, x2); }
		if (y1 > y2) { swap(y1, y2); }

		return this->sum(x2, y2) + this->sum(x1-1, y1-1)
					- this->sum(x2, y1-1) - this->sum(x1-1, y2);
	}

private:
	int v[1001][1001];

	int xSize;
	int ySize;

};

int main(int argc, char const *argv[])
{
	BIT b;

	while (true)
	{
		int x, y, p; scanf("%d %d %d%*c", &x, &y, &p);
		if (not (x | y | p)) { return 0; }

		b.zero(x, y);

		int q; scanf("%d%*c", &q);

		while (q--)
		{
			char c = getchar();

			if ( c == 'A')
			{
				int n, m, l; scanf("%d %d %d%*c", &n, &m, &l);
				b.update(m, l, n*p);
			}
			else
			{
				int n, m, l, o; scanf("%d %d %d %d%*c", &n, &m, &l, &o);


				printf("%d\n", b.query(n, m, l, o));
			}
		}

		putchar('\n');
	}

	return 0;
}