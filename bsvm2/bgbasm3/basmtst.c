#include <bgbasm.h>

int ptst()
{
	printf("Lookup Test OK\n");
}

int main(int argc, char *argv[])
{
	void *p, *q;
	int (*fcn)();
	char *s;
	int i;

	if(argc<2)
	{
		printf("usage: %s asm+\n", argv[0]);
		return(-1);
	}

	BASM_ProcessEXE(argv[0]);

	printf("stack %p\n", &i);

	for(i=1; i<argc; i++)
		BASM_LoadFile(argv[i]);

//	p=BASM_GetPtr("ptst");
//	q=&ptst;
//	if(p!=q)printf("Got %p, should be %p\n", p, q);

	for(i=0; i<4; i++)
		printf("%s\n", BASM_GenSym());

	p=BASM_GetPtr("basm_main");
	printf("Got %p\n", p);

	if(p)
	{
		fcn=(int(*)())p;
		fcn();
	}

	return(0);
}
