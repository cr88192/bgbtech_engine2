/*
Written 2007-2009 by Brendan G Bohannon
This code is re-assigned into the public domain.
*/

/*

special purpose PRNG
the main intention is to generate hopefully-unique keys for use in gensym

will be base-48 or base 62.
exp scale is ~5.585.

so, the string is 12 chars long and represents a 64 bit number.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include <bgbasm.h>

int basm_rand_pos;

/* default initial state: first 1024 digits of PI */

u32 basm_rand_state[128]=
{
0x31415926, 0x53589793, 0x23846264, 0x33832795,
0x02884197, 0x16939937, 0x51058209, 0x74944592,
0x30781640, 0x62862089, 0x98628034, 0x82534211,
0x70679821, 0x48086513, 0x28230664, 0x70938446,

0x09550582, 0x23172535, 0x94081284, 0x81117450,
0x28410270, 0x19385211, 0x05559644, 0x62294895,
0x49303819, 0x64428810, 0x97566593, 0x34461284,
0x75648233, 0x78678316, 0x52712019, 0x09145648,

0x56692346, 0x03486104, 0x54326648, 0x21339360,
0x72602491, 0x41273724, 0x58700660, 0x63155881,
0x74881520, 0x92096282, 0x92540917, 0x15364367,
0x89259036, 0x00113305, 0x30548820, 0x46652138,

0x41469519, 0x41511609, 0x43305727, 0x03657595,
0x91953092, 0x18611738, 0x19326117, 0x93105118,
0x54807446, 0x23799627, 0x49567351, 0x88575272,
0x48912279, 0x38183011, 0x94912983, 0x36733624,

0x40656643, 0x08602139, 0x49463952, 0x24737190,
0x70217986, 0x09437027, 0x70539217, 0x17629317,
0x67523846, 0x74818467, 0x66940513, 0x20005681,
0x27145263, 0x56082778, 0x57713427, 0x57789609,

0x17363717, 0x87214684, 0x40901224, 0x95343014,
0x65495853, 0x71050792, 0x27968925, 0x89235420,
0x19956112, 0x12902196, 0x08640344, 0x18159813,
0x62977477, 0x13099605, 0x18707211, 0x34999999,

0x83729780, 0x49951059, 0x73173281, 0x60963185,
0x95024459, 0x45534690, 0x83026425, 0x22308253,
0x34468503, 0x52619311, 0x88171010, 0x00313783,
0x87528865, 0x87533208, 0x38142061, 0x71776691,

0x47303598, 0x25349042, 0x87554687, 0x31159562,
0x86388235, 0x37875937, 0x51957781, 0x85778053,
0x21712268, 0x06613001, 0x92787661, 0x11959092,
0x16420198, 0x93809525, 0x72010654, 0x85863278
};

static char *basm_base48=
	"ABCDEFGHIJKLMNOPQRSTUVWX"
	"abcdefghijklmnopqrstuvwx";


int basm_genseed()
{
	int i, j, k, l;

	l=clock(); j=0; k=l;
	while(1)
	{
		i=clock();
		if(((i-l)>=10) || (i<l))break;

		k=(k>>16)^(k<<1)^i;
		j++;
	}

//	printf("seed %08X, cnt %d\n", k, j);
	return(k);
}

void BASM_InitRand()
{
	static char buf[256];
	static int init=0;
	FILE *fd;
	long long l;
	time_t t;
	int i, j, k;
	u32 i2;

	if(init)return;
	init=1;

//	fd=fopen("randbits.bin", "rb");
//	if(fd)
//	{
//		fread(basm_rand_state, 128*4, 1, fd);
//		fclose(fd);
//	}

	fd=fopen("randbits.txt", "rt");
	if(fd)
	{
		for(i=0; i<16; i++)
		{
			fgets(buf, 256, fd);
			sscanf(buf, "%X %X %X %X %X %X %X %X",
				&basm_rand_state[i*8+0],
				&basm_rand_state[i*8+1],
				&basm_rand_state[i*8+2],
				&basm_rand_state[i*8+3],
				&basm_rand_state[i*8+4],
				&basm_rand_state[i*8+5],
				&basm_rand_state[i*8+6],
				&basm_rand_state[i*8+7]);
		}
		fclose(fd);
	}


	t=time(NULL)^basm_genseed();

	l=*(u32 *)(&t);
	for(i=0; i<128; i++)
	{
		l=l*65521+basm_rand_state[i];
		basm_rand_state[i]=(l>>16)&0xFFFFFFFF;
	}
	basm_rand_state[0]^=(u32)l;

	fd=fopen("randbits.txt", "wt");
	if(fd)
	{
		for(i=0; i<16; i++)
		{
			for(j=0; j<8; j++)
				fprintf(fd, "%08X ", basm_rand_state[i*8+j]);
			fprintf(fd, "\n");
		}

//		fwrite(basm_rand_state, 128*4, 1, fd);
		fclose(fd);
	}
}

u32 basm_rand()
{
	int i, j, k;

	i=basm_rand_pos;
	j=(i+127-61)%127;
	k=(i+127-31)%127;
	basm_rand_state[i]^=basm_rand_state[j];
	basm_rand_state[i]^=basm_rand_state[k];
	basm_rand_pos=(i+1)%127;

	return(basm_rand_state[i]);
}

char *basm_rand_key12()
{
	static char buf[16];
	u32 i;

	BASM_InitRand();

	i=basm_rand();
	buf[0]=basm_base48[(i/254803968)%48];
	buf[1]=basm_base48[(i/5308416)%48];
	buf[2]=basm_base48[(i/110592)%48];
	buf[3]=basm_base48[(i/2304)%48];
	buf[4]=basm_base48[(i/48)%48];
	buf[5]=basm_base48[ i%48];

	i=basm_rand();
	buf[ 6]=basm_base48[(i/254803968)%48];
	buf[ 7]=basm_base48[(i/5308416)%48];
	buf[ 8]=basm_base48[(i/110592)%48];
	buf[ 9]=basm_base48[(i/2304)%48];
	buf[10]=basm_base48[(i/48)%48];
	buf[11]=basm_base48[ i%48];

	buf[12]=0;

	return(buf);
}

char *basm_rand_key18()
{
	static char buf[24];
	u32 i;

	BASM_InitRand();

	i=basm_rand();
	buf[0]=basm_base48[(i/254803968)%48];
	buf[1]=basm_base48[(i/5308416)%48];
	buf[2]=basm_base48[(i/110592)%48];
	buf[3]=basm_base48[(i/2304)%48];
	buf[4]=basm_base48[(i/48)%48];
	buf[5]=basm_base48[ i%48];

	i=basm_rand();
	buf[ 6]=basm_base48[(i/254803968)%48];
	buf[ 7]=basm_base48[(i/5308416)%48];
	buf[ 8]=basm_base48[(i/110592)%48];
	buf[ 9]=basm_base48[(i/2304)%48];
	buf[10]=basm_base48[(i/48)%48];
	buf[11]=basm_base48[ i%48];

	i=basm_rand();
	buf[12]=basm_base48[(i/254803968)%48];
	buf[13]=basm_base48[(i/5308416)%48];
	buf[14]=basm_base48[(i/110592)%48];
	buf[15]=basm_base48[(i/2304)%48];
	buf[16]=basm_base48[(i/48)%48];
	buf[17]=basm_base48[ i%48];

	buf[18]=0;

	return(buf);
}

