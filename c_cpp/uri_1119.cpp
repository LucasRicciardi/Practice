// https://www.urionlinejudge.com.br/judge/pt/problems/view/1119

#include <cstdio>
#include <iostream>

using namespace std;

struct node
{
	node(int k): 
	data(k), next_(this), prev_(this) {}
 
	node(int k, node * n, node * m):
	data(k), next_(n), prev_(m) {}

	~node() {}

	void print()
	{
		printf("%3d", this->data);
	}

	int data;

	node * next_;
	node * prev_;

	node * next() { return next_; }
	node * prev() { return prev_; }

	void setNext(node * n) { this->next_ = n; };
	void setPrev(node * n) { this->prev_ = n; }
};

typedef node * NODE;

struct list
{
	list(): size_(0) {}
	~list() {}

	int size_;

	NODE first_;
	NODE last_;

	NODE first() { return this->first_; }
	NODE last() { return this->last_; }

	void setFirst(NODE n) { this->first_ = n; }
	void setLast(NODE n) { this->last_ = n; }

	int size() { return this->size_; }
	bool empty() { return this->size() == 0; }

	#define grow() this->size_++
	#define shrink() this->size_--

	void add(int k)
	{
		if (this->empty())
		{
			this->setFirst(new node(k));
			this->setLast(this->first());
		}
		else
		{
			NODE m = this->first();
			NODE n = this->last();			
			
			n->setNext(new node(k, m, n));
			
			this->setLast(n->next());
			m->setPrev(this->last());
		}

		grow();
	}

	NODE next(NODE n, int k) 
	{
		k--;
		while(k--) { n = n->next(); }

		return n;
	}

	NODE prev(NODE n, int k)
	{
		k--;
		while(k--) { n = n->prev(); }
		return n;
	}

	NODE del(NODE n, int k)
	{
		NODE prev = n->prev();
		NODE next = n->next();

		prev->setNext(next);
		next->setPrev(prev);

		shrink();

		delete n;

		if (k == 1)
			return next;
		else
			return prev;
		
	}
};

int main(int argc, char const *argv[])
{
	while (true)
	{
		int n, k, m; scanf("%d %d %d%*c", &n, &k, &m);
		if (not (n | k | m)) { return 0; }

		list l;

		for (int i = 0; i < n; i++)
			l.add(i+1);

		NODE victim1 = l.first();
		NODE victim2 = l.last(); 

		while(not l.empty())
		{
			victim1 = l.next(victim1, k);
			victim2 = l.prev(victim2, m);

			if ( victim1 == victim2 )
			{
				victim1->print();
				if (l.size() > 1) { printf(","); }
				else			  { printf("\n"); }					

				victim1 = l.del(victim1, 1);
				victim2 = victim1->prev();
			}
			else
			{
				victim1->print();
				victim2->print();
				if (l.size() > 2) { printf(","); }
				else			  { printf("\n"); }					

				victim1 = l.del(victim1, 1);
				if (victim1 == victim2) {victim1 = victim1->next(); }
				victim2 = l.del(victim2, 2);
			}
		}
	}


	return 0;
}