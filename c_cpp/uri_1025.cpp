// https://www.urionlinejudge.com.br/judge/pt/problems/view/1025

#include <iostream>
#include <vector>

using namespace std;

int binary_search(std::vector<int> v, int n)
{
	int low = 0, high = v.size()-1;

	while (low <= high)
	{
		int mid = (low + high) / 2;

		if (n == v.at(mid))
			return mid;

		if (n > v.at(mid))
		{
			low = mid + 1;
		}

		if (n < v.at(mid))
		{
			high = mid - 1;
		}
	}

	return -1;
}

void swap(int& a, int& b)
{
	int temp 	= a;
	a 			= b;
	b 			= temp;
} 

void quickSort(std::vector<int>& v, int low, int high)
{
	if (low >= high)
		return;

	int pivot 	= v.at(high);
	int left	= low;
	int right	= high-1;

	for (;;)
	{
		while(v.at(left) < pivot)
			left++;

		while(v.at(right) >= pivot and right > left)
			right--;

		if (left >= right)
			break;
		else
			swap(v.at(left), v.at(right));
	}

	swap(v.at(left), v.at(high));

	quickSort(v, low, left-1);
	quickSort(v, left+1, high);
}

int main(int argc, char const *argv[])
{
	int c = 1;

	while (true)
	{
		int n, q;

		cin >> n >> q;

		if (not n and not q)
			return 0;

		std::vector<int> v;
		for (int i = 0; i < n; i++)
		{
			int a; cin >> a;
			v.push_back(a);
		}
		quickSort(v, 0, v.size()-1);

		std::vector<int> w;
		for (int i = 0; i < q; i++)
		{
			int a; cin >> a;
			w.push_back(a);
		}

		cout << "CASE# " << c++ << ":" << endl;

		for (int i = 0; i < w.size(); i++)
		{
			int a = binary_search(v, w.at(i));
			cout << w.at(i);

			if (a == -1)
				cout << " not found" << endl;
			else
			{
				if (a > 0)
					while (v.at(a-1) == v.at(a))
						a = a-1;

				cout << " found at " << a+1 << endl;
			}

		}
	}
	return 0;
}