#include <math.h>

typedef unsigned char btac1c_byte;
typedef signed char btac1c_sbyte;
typedef unsigned short btac1c_u16;
typedef signed short btac1c_s16;
typedef unsigned int btac1c_u32;
typedef signed int btac1c_s32;
typedef unsigned long long btac1c_u64;
typedef signed long long btac1c_s64;


btac1c_u32 tst_fakesqrt32(btac1c_u32 val)
{
	btac1c_u32 v, v0, v1;
	
	v=val;
	if(v>>24)	{ v=v>>8; }
	if(v>>20)	{ v=v>>4; }
	if(v>>18)	{ v=v>>2; }
	if(v>>16)	{ v=v>>1; }

	v0=v>>8; if((v0*v0)>val) { v=v0; }
	v0=v>>4; if((v0*v0)>val) { v=v0; }
	v0=v>>2; if((v0*v0)>val) { v=v0; }
	v0=v>>1; if((v0*v0)>val) { v=v0; }
	if((v*v)>val) { v=v>>1; }

	for(v0=v>>1; v0; v0=v0>>1)
	{
		v1=v+v0;
		if((v1*v1)<=val)
		{
			v=v1; v1=v+v0;
			if((v1*v1)<=val)
				{ v=v1; }
		}
	}
	return(v);
}

btac1c_u32 tst_sqrt(btac1c_u32 val)
{
	return(sqrt(val));
}
