#include <bteifgl.h>

BGBDTC_ClassInfo *BGBDTC_AllocClass()
{
	BGBDTC_ClassInfo *tmp;
	
	tmp=dtmAlloc("bgbdtc_classinfo_t", sizeof(BGBDTC_ClassInfo));
	return(tmp);
}