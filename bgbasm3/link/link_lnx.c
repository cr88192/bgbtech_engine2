#ifdef linux
#define _GNU_SOURCE
#include <dlfcn.h>
#include <link.h>
#endif

// #include <bgblink.h>
#include <bgbasm.h>

#ifdef linux

void *blnk_hmodule;

char *blnk_hmodname[256];
void *blnk_hmoddll[256];
int blnk_numdll=0;

static int blnk_phdr_callback(struct dl_phdr_info *info,
	size_t size, void *data)
{
	char *s, *s1;
	s64 j, addr, sz;
	int i;

	sz=0;

	printf("(MOD) %s %p\n",
		(char *)info->dlpi_name,
		(void *)info->dlpi_addr);

	for(i=0; i<info->dlpi_phnum; i++)
	{
//		j=info->dlpi_phdr[i].p_vaddr+
//			info->dlpi_phdr[i].p_memsz;
//		if(j>sz)sz=j;

		addr=(s64)info->dlpi_addr+
			info->dlpi_phdr[i].p_vaddr;
		sz=info->dlpi_phdr[i].p_memsz;

		if(!sz)continue;

		printf("\t%p %d\n",
			(void *)addr, (int)sz);

		BLNK_AddExeSection((char *)info->dlpi_name,
			addr, sz, 0);
	}

//	printf("(MOD) %s %p %d\n",
//			(char *)info->dlpi_name,
//			(s64)info->dlpi_addr, (int)sz);

//	BLNK_AddExeSection((char *)info->dlpi_name,
//		(s64)info->dlpi_addr, sz, 0);

	BLNK_LoadDynamicOS((char *)info->dlpi_name);

	return(0);
}

int BLNK_InitDllsOS()
{
	static int init=0;

	if(init)return(0);
	init=1;

	dl_iterate_phdr(blnk_phdr_callback, NULL);
}

BASM_API int BLNK_GetModuleListOS(char **buf, int max)
{
	int i, n;

	BLNK_InitDllsOS();

	n=blnk_numdll;
	if(n>max)n=max;
	for(i=0; i<n; i++)
		buf[i]=blnk_hmodname[i];
	return(n);

//	return(0);
}

BASM_API char **BLNK_GetEnvironOS()
{
	return(NULL);
}

char *BLNK_LookupAddrNameOS(void *addr, void **addr2)
{
	Dl_info inf;
	char *s;
	int i;
	
	i=dladdr(addr, &inf);
	if(!i)return(NULL);
	if(addr2)*addr2=inf.dli_saddr;
	s=blnk_strdup((char *)inf.dli_sname);
	return(s);
	
//	return(NULL);
}

int BLNK_LoadDynamicOS(char *name)
{
	void *hdl;
	int i;

	if(blnk_lookup_blacklist(name)>0)
		return(-1);

	for(i=0; i<blnk_numdll; i++)
		if(!strcmp(blnk_hmodname[i], name))
			return(0);

//	hdl=dlopen(name, RTLD_LAZY);
	hdl=dlopen(name, RTLD_NOW|RTLD_GLOBAL);
	if(hdl)
	{
		i=blnk_numdll++;
		blnk_hmodname[i]=blnk_strdup(name);
		blnk_hmoddll[i]=hdl;
		return(0);
	}
	
	printf("BLNK_LoadDynamicOS: Fail %s\n", dlerror());
	
	return(-1);
}

void BLNK_UpdateProxyOS(char *name, void *ptr)
{
	char ntmp[256];
	void *p;
	int i;

	sprintf(ntmp, "_iproxy_%s", name);

	if(blnk_hmodule)
	{
		p=dlsym(blnk_hmodule, ntmp);
		if(p)*(void **)p=ptr;
	}

	for(i=0; i<blnk_numdll; i++)
	{
		p=dlsym(blnk_hmoddll[i], ntmp);
		if(p)*(void **)p=ptr;
	}
}

void *BLNK_LookupLabelOS(char *name)
{
	char nbuf[64];
	void *p;
	int i;

	if(blnk_lookup_blacklist(name)>0)
		return(-1);

	if(!blnk_hmodule)blnk_hmodule=dlopen(NULL, RTLD_LAZY);

	if(blnk_hmodule)
	{
		p=dlsym(blnk_hmodule, name);
		if(p)return(p);
	}

	for(i=0; i<blnk_numdll; i++)
	{
		p=dlsym(blnk_hmoddll[i], name);
		if(p)return(p);
	}

	return(NULL);
}


int BLNK_ProcessEXE(char *name)
{
	return(0);
}


void *BLNK_LoadFileRawOS(char *name, int *rsz)
{
	void *buf;
	FILE *fd;
	int sz;

//	blnk_initvfs();
	fd=fopen(name, "rb");
	if(!fd)return(NULL);

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);

	buf=malloc(sz+16);
	memset(buf, 0, sz+16);
	fread(buf, 1, sz, fd);

	fclose(fd);

	if(rsz)*rsz=sz;
	return(buf);
}

BASM_API void *BLNK_LoadModuleFileDataOS(char *name, int *rsz)
{
	char nbuf[1024];
//	HMODULE hdl;
//	HANDLE hFile;
//	LARGE_INTEGER szFile;
//	DWORD dwBytesRead;
	void *buf;
	char *s;
	int i, sz;

	BLNK_InitDllsOS();

	if(blnk_lookup_blacklist(name)>0)
		return(-1);

	//try just loading the file...
//	buf=blnk_loadfile(name, &sz);
	buf=BLNK_LoadFileRawOS(name, &sz);
	if(buf)
	{
		printf("BLNK_LoadModuleFileDataOS: Got %s Ptr=%p Sz=%d\n",
			name, buf, sz);
		if(rsz)*rsz=sz;
		return(buf);
	}

//	*(int *)-1=-1;
	printf("BLNK_LoadModuleFileDataOS: Fail %s\n", name);

	//FIXME: find way to port to Linux...
	return(NULL);

#if 0
	hdl=GetModuleHandle(name);
	i=GetModuleFileName(hdl, nbuf, 1024);
	if(!i)return(NULL);

	hFile=CreateFile(nbuf,		// module name
		GENERIC_READ,			// open for reading
		FILE_SHARE_READ,		// share for reading
		NULL,					// default security
		OPEN_EXISTING,			// existing file only
		FILE_ATTRIBUTE_NORMAL,	// normal file
		NULL);					// no attr. template

	if (hFile==INVALID_HANDLE_VALUE) 
		return(NULL);

	if(!GetFileSizeEx(hFile, &szFile))
	{
		CloseHandle(hFile);
		return(NULL);
	}

	sz=szFile.QuadPart;

	//fail file is overly large
	if(sz>(1<<26))
	{
		CloseHandle(hFile);
		return(NULL);
	}

	buf=malloc(sz);

	if(!ReadFile(hFile, buf, sz, &dwBytesRead, NULL))
	{
//	printf("Could not read from file (error %d)\n", GetLastError());
		CloseHandle(hFile);
		return(NULL);
	}

	CloseHandle(hFile);
	if(rsz)*rsz=sz;
	return(buf);
#endif
}

/** load file data, if it is an ExWAD image */
BASM_API void *BLNK_LoadModuleWadDataOS(char *name, int *rsz)
{
	void *p;
	byte *pb;
	int i, sz;

	p=BLNK_LoadModuleFileDataOS(name, &sz);
	if(!p)return(NULL);
	
	pb=(byte *)p;
	for(i=0; i<(sz/16); i++)
	{
//		if(!memcmp(pb+i*16, "ExWAD100", 8))
		if(!memcmp(pb+i*16, "ExWAD101", 6) &&
			!memcmp(pb+(i+1)*16, "DAWxE\r\n\xFF", 8))
		{
			if(rsz)*rsz=sz;
			return(p);
		}
	}

	//no ExWAD sig found
	free(p);
	return(NULL);
}

BASM_API void BLNK_FreeModuleDataOS(void *buf)
{
	free(buf);
}

#endif
