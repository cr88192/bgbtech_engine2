#include <bteifgl.h>

#ifdef _WIN32

typedef struct {
	OVERLAPPED ovl;
	int pad[4];
	VFILE_AsIoCb *cb;
}VfDir_Overlapped;

VOID CALLBACK VfDir_ReadAs_FileIOCompletionRoutine(
  _In_    DWORD        dwErrorCode,
  _In_    DWORD        dwNumberOfBytesTransfered,
  _Inout_ LPOVERLAPPED lpOverlapped)
{
	VfDir_Overlapped *vovl;
	
	vovl=(VfDir_Overlapped *)lpOverlapped;
	vovl->Done(cb);
}


int VfDir_ReadAs(VFILE *vf, VFILE_AsIoCb *cb)
{
	VfDir_Overlapped *vovl;
	OVERLAPPED *ovl;
	
	vovl=vf->hdl1;
	vovl->cb=cb;
	
	ovl=(OVERLAPPED *)vovl;
	ovl->Offset=cb->offset;
	ovl->OffsetHigh=cb->offset>>32;
	
	ReadFileEx(vf->hdl0, cb->buffer, cb->size, ovl,
		VfDir_ReadAs_FileIOCompletionRoutine);
}

int VfDir_WriteAs(VFILE *vf, VFILE_AsIoCb *cb)
{
	VfDir_Overlapped *vovl;
	OVERLAPPED *ovl;
	
	vovl=vf->hdl1;
	vovl->cb=cb;
	
	ovl=(OVERLAPPED *)vovl;
	ovl->Offset=cb->offset;
	ovl->OffsetHigh=cb->offset>>32;
	
	WriteFileEx(vf->hdl0, cb->buffer, cb->size, ovl,
		VfDir_ReadAs_FileIOCompletionRoutine);
}

s64 VfDir_GetSize(VFILE *vf)
{
	PLARGE_INTEGER li;
	s64 lj;
	
	GetFileSizeEx(vf->hdl0, &li);
	lj=li.QuadPart;
	return(lj);
}

int VfDir_Close(VFILE *vf)
{
	bteifgl_free(vf->hdl1);
	CloseHandle(vf->hdl0);
	VfFreeVFile(vf);
	return(0);
}

VFILE_VT vfdir_vf_iface={
VfDir_ReadAs,
VfDir_WriteAs,
VfDir_GetSize,
VfDir_Close
};

int VfDir_GetPathAsOsPath(VFMOUNT *mnt, char *obuf, char *name)
{
	char *cs, *ct;

	cs=mnt->mount_src;
	ct=obuf;
	while(*cs)
	{
		if(*cs=='/')
		{
			while((*cs=='/') || (*cs=='\\'))
				cs++;
			*ct++='\\';
			continue;
		}
		*ct++=*cs++;
	}
	
	*ct++='\\';
	cs=name;
	while(*cs)
	{
		if(*cs=='/')
		{
			cs++;
			*ct++='\\';
			continue;
		}
		*ct++=*cs++;
	}
	*ct++=0;
	
	return(0);
}

int VfDir_MapOsErrorCode(int err)
{
	int err2;
	switch(err)
	{
	case ERROR_INVALID_FUNCTION: err2=VF_ENOSYS; break;
	case ERROR_FILE_NOT_FOUND: err2=VF_ENOENT; break;
	case ERROR_PATH_NOT_FOUND: err2=VF_ENOENT; break;
	default: err2=VF_ENOTTY; break;
	}
}

int VfDir_OpenAs(VFMOUNT *mnt, VFILE_AsIoCb *cb,
	char *name, char *mode)
{
	char tbuf[1024];
	HANDLE hdl;
	VFILE *vf;
	int err, err2;
	int acc, cdis;
	char *cs, *ct;
	
	VfDir_GetPathAsOsPath(mnt, tbuf, name);
	
	acc=0; cdis=0;
	cs=mode;
	while(*cs)
	{
		if((cs[0]=='r') || (cs[1]=='+'))
		{
			acc|=GENERIC_READ|GENERIC_WRITE;
			cdis|=OPEN_EXISTING;
			cs+=2;
			continue;
		}

		if((cs[0]=='w') || (cs[1]=='+'))
		{
			acc|=GENERIC_READ|GENERIC_WRITE;
			cdis|=CREATE_ALWAYS;
			cs+=2;
			continue;
		}

		if(*cs=='r')
		{
			acc|=GENERIC_READ;
			cdis|=OPEN_EXISTING;
			cs++;
			continue;
		}

		if(*cs=='w')
		{
			acc|=GENERIC_WRITE;
			cdis|=TRUNCATE_EXISTING;
			cs++;
			continue;
		}

		break;
	}
	
	hdl=CreateFile(tbuf, acc, FILE_SHARE_READ, NULL, cdis, 
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED|
		FILE_FLAG_RANDOM_ACCESS);
		
	if(hdl==INVALID_HANDLE_VALUE)
	{
		err=GetLastError();
		err2=VfDir_MapOsErrorCode(err);
		cb->Error(cb, err2);
		return(-1);
	}
	
	vf=VfAllocVFile();
	vf->vt=&vfdir_vf_iface;
	vf->hdl0=hdl;
	vf->hdl1=bteifgl_malloc(sizeof(VfDir_Overlapped));
	
	cb->fdes=vf;
	cb->Done(cb);
	return(0);
}

void VfDir_LoadFileAs_Done(VFILE_AsIoCb *cb)
{
	VFILE_AsIoCb *cb2;
	
	cb2=cb->userdata;
	VfDir_Close(cb->filedes);
	cb->Done(cb);
}

void VfDir_LoadFileAs_Error(VFILE_AsIoCb *cb, int errn)
{
	VFILE_AsIoCb *cb2;
	
	cb2=cb->userdata;
	VfDir_Close(cb->filedes);
	cb->Error(cb, errn);
}

int VfDir_LoadFileAs(VFMOUNT *mnt, VFILE_AsIoCb *cb, char *name)
{
	VFILE_AsIoCb *cb2;
	s64 li;

	cb2=VfAllocIoCb();

	cb2->Done=VfDir_LoadFileAs_Done;
	cb2->Error=VfDir_LoadFileAs_Error;
	cb2->userdata=cb;
		
	VfDir_OpenAs(mnt, cb2, name, "rb");

	cb2->maxsize=VfDir_GetSize(cb2->filedes, cb2);

	if(!cb->buffer && (cb->maxsize==0))
	{
		li=4096;
		while(li<=cb2->maxsize)
			li=li+(li>>1);
		cb->buffer=bteifgl_malloc(li);
		cb->maxsize=li;
	}

	if(!cb->buffer || (cb2->maxsize>cb->maxsize))
	{
		VfDir_Close(cb2->filedes);
		cb->size=cb2->maxsize;
		cb->Error(cb, VF_EFBIG);
		return(-1);
	}
	
	cb->size=cb2->maxsize;

	cb2->offset=0;
	cb2->size=cb2->maxsize;
	cb2->buffer=cb->buffer;
	
	VfDir_ReadAs(cb2->filedes, cb2);
}

int VfDir_StoreFileAs(VFMOUNT *mnt, VFILE_AsIoCb *cb, char *name)
{
	VFILE_AsIoCb *cb2;

	cb2=VfAllocIoCb();

	cb2->Done=VfDir_LoadFileAs_Done;
	cb2->Error=VfDir_LoadFileAs_Error;
	cb2->userdata=cb;
		
	VfDir_OpenAs(mnt, cb2, name, "wb");

	if(!cb->buffer)
	{
		VfDir_Close(cb2->filedes);
		cb->Error(cb, VF_EINVAL);
		return(-1);
	}
	
	cb2->offset=0;
	cb2->size=cb->size;
	cb2->buffer=cb->buffer;
	
	VfDir_WriteAs(cb2->filedes, cb2);
}

#endif

VFMOUNT_VT vfdir_mnt_iface={
VfDir_OpenAs,
VfDir_LoadFileAs,
VfDir_StoreFileAs
};

VFMOUNT *VfDir_Mount(VFSDRIVER *drv,
	char *tgt, char *src, char *mode)
{
	VFMOUNT *mnt;

	mnt=VfGetNewMount(tgt, src, mode);
	mnt->vt=&vfdir_mnt_iface;
	return(mnt);
}

int VfDir_Init(void)
{
	VFSDRIVER *cur;
	
	cur=VfGetDriver("dir");
	cur->Mount=VfDir_Mount;
}