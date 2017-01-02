// https://www.urionlinejudge.com.br/judge/pt/problems/view/1301

#include <iostream>
#include <cstdio>
#include <vector>

class BIT
{
public:
	BIT(int s): size_(s+1) { this->reset(); }
	~BIT() {}

	int& size() { return this->size_; }

    void reset() { this->v.assign(this->size()+1, 0); }

	int get(int x)
	{
		int sum = 0;
		for (; x; x -= x&(-x)) { sum += this->v[x]; }
		return sum;
	}

	int query(int a, int b) { return this->get(b) - this->get(a-1); }

	void update(int x, int v)
	{
		for (; x <= this->size(); x += x&(-x)) { this->v[x] += v; }
	}

private:
	int size_;
	std::vector<int> v;
};

int main(int argc, char const *argv[])
{
	int n, k; 

	while (scanf("%d %d%*c", &n, &k) != EOF)
	{	
		int v[100005];
		BIT zero(n), negt(n); 
		for (int i = 1; i <= n; i++)
		{
			int t; scanf("%d%*c", &t);
			v[i] = t;
			if (not t) { zero.update(i, 1); }
			if (t < 0) { negt.update(i, 1); }
		}

		while (k--)
		{
			int a, b; char c;
			scanf("%c %d %d%*c", &c, &a, &b);

			if (!(c ^ 'C'))
			{
				if (!(v[a]^0) && b != 0) { zero.update(a, -1); }
				if (v[a] <  0 && b >= 0) { negt.update(a, -1); }

				if (v[a] != 0 && !(b^0)) { zero.update(a, 1); }
				if (v[a] >= 0 && b <  0) { negt.update(a, 1); }

				v[a] = b; 
			}
			else
			{
				if      (zero.query(a,b)|0) { printf("0"); }
				else if (negt.query(a,b)&1) { printf("-"); }
				else                        { printf("+"); }
			}
		}

		printf("\n");
	}

	return 0;
}