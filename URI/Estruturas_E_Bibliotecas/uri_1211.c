// https://www.urionlinejudge.com.br/judge/pt/problems/view/1211

#include <stdio.h>
#include <stdlib.h>

int compare(char const * s, char const * r)
{
	while (*s != '\0' && *r != '\0')
	{
		if (*s < *r) { return 1; }
		if (*s > *r) { return 0; }
	
		s++; r++;
	}

	return 0;
}

char ** copy(char ** begin, char ** const end)
{
	char ** w = (char **) malloc((end - begin) * sizeof(char *));
	
	char ** u = &w[0];
	while (begin != end) { *u++ = *begin++; }

	return w;
}

void merge(char ** begin, char ** mid, char ** end)
{
	char ** w  = copy(begin, end);
	
	char ** left  = &w[0];
	char ** right = &w[0] + (mid-begin);

	char ** const mid_end = right;
	char ** const end_end = right + (end-mid);

	while (left != mid_end && right != end_end)
	{
		if (compare(*left, *right)) { *begin++ = *left++;  }
		else                        { *begin++ = *right++; }
	}

	while (left  != mid_end)  { *begin++ = *left++;  }
	while (right != end_end)  { *begin++ = *right++; }

	free(w);
}

void mergeSort(char ** begin, char ** end)
{
	if ( begin != end )
	{
		char ** const mid = begin + (end - begin) / 2;
		if (end - mid > 1)
		{	
			mergeSort(begin, mid);
			mergeSort(mid, end);
		}
		merge(begin, mid, end);
	}
}

char * foo(char * a, char * b, int * ocur)
{
	int i;
	for (i = 0; a[i] != '\0'; i++)
	{
		if (a[i] != b[i]) { return b; }
		else              { (*ocur)++; }
	}

	return b;
}

int ocur(char ** s, int n)
{
	int i, ocur = 0;
	for (i = 1; i < n; i++) { foo(s[i-1], s[i], &ocur); }

	return ocur;
}

int main()
{
	int n; 
	while (scanf("%d", &n) != EOF)
	{
		int i;
		char ** c = (char **) malloc(n * sizeof(char *));
		for (i = 0; i < n; i++)
		{
		 	c[i] = (char *) malloc(200 * sizeof(char));
		 	scanf("%s%*c", c[i]);
		}	

		mergeSort(&c[0], &c[n]);
		printf("%d\n", ocur(c, n));

		for (i = 0; i < n; i++) { free(c[i]); }
		free(c);	
	}
}