#include <bgbasm.h>

int main(int argc, char *argv[])
{
	char *s;
	int i;

	if(argc<3)
	{
		printf("usage: %s in-asm out-coff\n", argv[0]);
		return(-1);
	}

	BASM_ProcessEXE(argv[0]);

	BASM_AssembleFile(argv[1], argv[2]);

	return(0);
}
