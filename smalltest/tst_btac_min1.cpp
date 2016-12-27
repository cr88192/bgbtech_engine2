#include "btac1c_mini0.h"

#include <cstdio>
#include <cmath>

int main()
{
	btac1c_s64 v;
	
	v=69696969;
	v=40934093;
	v=254;
	printf("%d %d\n", (int)sqrt(v), (int)btac1c2_fakesqrt(v));
}
