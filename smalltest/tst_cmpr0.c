#include <stdio.h>

int main()
{
	int i0, i1, i2, i3;
	int w0, w1, w2;
	int i, j, k;
	
#if 0
	for(i=0; i<256; i++)
	{
		j=i+((256-i)>>5);
		k=i-(i>>5);
		printf("%02X %02X %02X\n", i, j, k);
	}
#endif

	for(i=0; i<256; i++)
	{
		i0=(i   )&15;
		i1=(i>>4)&15;

		w0=256*(i0+1)/(i0+i1+2);

		i2=i0+1; i3=i1;
		if(i2>=16) { i2--; i3--; if(i3<0)i3=0; }
		w1=256*(i2+1)/(i2+i3+2);
		j=i2|(i3<<4);

		i2=i0; i3=i1+1;
		if(i3>=16) { i2--; i3--; if(i2<0)i2=0; }
		w2=256*(i2+1)/(i2+i3+2);
		k=i2|(i3<<4);

	
		printf("%02X(%02X) %02X(%02X) %02X(%02X)\n", i, w0, j, w1, k, w2);
	}
}
