#include <stdio.h>
#include <complex.h>

int main()
{
//	complex double cpi0;
	double pi0, pi1, pi2, pi3;
	
	pi0=3.1415926535897932384626433832795;
	pi1=3.141592653589793;
	pi2=3.14159265358979;
//	pi2=3.1415926535898;
	pi3=3.14159265359;
	printf("%016llX\n", *(unsigned long long *)(&pi0));
	printf("%016llX\n", *(unsigned long long *)(&pi1));
	printf("%016llX\n", *(unsigned long long *)(&pi2));
	printf("%016llX\n", *(unsigned long long *)(&pi3));
}
