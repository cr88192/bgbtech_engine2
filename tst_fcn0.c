#include <stdio.h>

int t_abs(int x) { int s=x>>31; return (x^s)+(s&1); }

int main()
{
	printf("%d %d\n", t_abs(63), t_abs(-63));
	printf("%d %d\n", t_abs(131071), t_abs(-131071));
	printf("%d %d\n", t_abs(1777216), t_abs(-1777216));
}