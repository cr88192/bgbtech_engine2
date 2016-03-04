#include <bteifgl.h>

VFILE_AsIoCb *vfile_asiocb_free=NULL;
VFILE *vfile_vfile_free=NULL;

VFSDRIVER *vfile_driver=NULL;
VFMOUNT *vfile_mtab=NULL;

BTEIFGL_API VFILE_AsIoCb *VfAllocIoCb(void)
{
	VFILE_AsIoCb *cb;
	
	cb=vfile_asiocb_free;
	
	if(cb)
	{
		vfile_asiocb_free=cb->userdata;
		memset(cb, 0, sizeof(VFILE_AsIoCb));
		return(cb);
	}
	
	cb=bteifgl_malloc(sizeof(VFILE_AsIoCb));
	return(cb);
}

BTEIFGL_API int VfFreeIoCb(VFILE_AsIoCb *cb)
{
	cb->userdata=vfile_asiocb_free;
	vfile_asiocb_free=cb;
}

BTEIFGL_API int VfReadAs(VFILE *vf, VFILE_AsIoCb *cb)
	{ return(vf->vt->ReadAs(vf, cb)); }

BTEIFGL_API int VfWriteAs(VFILE *vf, VFILE_AsIoCb *cb)
	{ return(vf->vt->WriteAs(vf, cb)); }

// BTEIFGL_API int VfRead(void *buf, int sz1, int sz2, VFILE *vf)
//	{ return(vf->vt->Read(buf, sz1, sz2, vf)); }

// BTEIFGL_API int VfWrite(void *buf, int sz1, int sz2, VFILE *vf)
//	{ return(vf->vt->Write(buf, sz1, sz2, vf)); }

// BTEIFGL_API int VfSeek(VFILE *vf, s64 pos, int rel)
//	{ return(vf->vt->Seek(vf, pos, rel)); }

// BTEIFGL_API s64 VfTell(VFILE *vf)
//	{ return(vf->vt->Tell(vf)); }

BTEIFGL_API s64 VfGetSize(VFILE *vf)
	{ return(vf->vt->GetSize(vf)); }

BTEIFGL_API int VfClose(VFILE *vf)
	{ return(vf->vt->Close(vf)); }

/** Check if mount matches a path, optionally returning a path suffix.
  * Suffix will give the remainder of the path characters.
  * Return value of 0 means non-match.
  * Return values >0 encode the number of characters matched.
  */
BTEIFGL_API int VfCheckMountPath(VFMOUNT *vm, char *path, char **rsfx)
{
	char *cs, *ct;
	
	cs=vm->mount_tgt; ct=path;
	while(*cs=='/')cs++;
	while(*ct=='/')ct++;
	
	while(*cs && *ct)
	{
		if((*cs=='/') && (*ct=='/'))
		{
			while(*cs=='/')cs++;
			while(*ct=='/')ct++;
			continue;
		}
		if(*cs!=*ct)break;
		cs++; ct++;
	}
	
	if(*cs)
	{
		if(rsfx)*rsfx=NULL;
		return(0);
	}

	while(*ct=='/')ct++;
	if(rsfx)*rsfx=ct;
	return(ct-path);
}

BTEIFGL_API VFMOUNT *VfFindMountForPath(char *path, char **rsfx)
{
	VFMOUNT *cur, *best;
	char *sfx, *bsfx;
	int nc, ncb;
	
	cur=vfile_mtab;
	best=NULL; ncb=-1; bsfx=NULL;
	while(cur)
	{
		nc=VfCheckMountPath(cur, path, &sfx);
		if((nc>0) && nc>ncb)
		{
			best=cur;
			bsfx=sfx;
			ncb=nc;
		}
		cur=cur->next;
	}
	
	if(rsfx)*rsfx=bsfx;
	return(best);
}

BTEIFGL_API VFILE *VfAllocVFile(void)
{
	VFILE *tmp;
	
	tmp=vfile_vfile_free;
	if(tmp)
	{
		vfile_vfile_free=tmp->hdl0;
		memset(tmp, 0, sizeof(VFILE));
		return(tmp);
	}
	
	tmp=bteifgl_malloc(sizeof(VFILE));
	return(tmp);
}

BTEIFGL_API int VfFreeVFile(VFILE *tmp)
{
	tmp->hdl0=vfile_vfile_free;
	vfile_vfile_free=tmp;
	return(0);
}

BTEIFGL_API VFMOUNT *VfGetNewMount(char *tgt, char *src, char *mode)
{
	VFMOUNT *mnt;
	
	mnt=bteifgl_malloc(sizeof(VFMOUNT));
	mnt->mount_tgt=bteifgl_strdup(tgt);
	mnt->mount_src=bteifgl_strdup(src);
	mnt->mount_mode=bteifgl_strdup(mode);
	
	mnt->next=vfile_mtab;
	vfile_mtab=mnt;
	
	return(mnt);
}


BTEIFGL_API int VfOpenAs(VFILE *vf,
	VFILE_AsIoCb *cb, char *name, char *mode)
{
	VFMOUNT *cur;
	char *sfx;
	
	cur=VfFindMountForPath(path, &sfx);
	if(!cur)
	{
		cb->Error(cb, VF_ENOENT);
		return(-1);
	}
	
	return(cur->OpenAs(cur, cb, sfx, mode));
//	return(vf->vt->WriteAs(vf, cb));
}

BTEIFGL_API VFSDRIVER *VfLookupDriver(char *name)
{
	VFSDRIVER *cur;
	
	cur=vfile_driver;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BTEIFGL_API VFSDRIVER *VfGetDriver(char *name)
{
	VFSDRIVER *cur;
	
	cur=VfLookupDriver(name);
	if(cur)
		return(cur);

	cur=bteifgl_malloc(sizeof(VFSDRIVER));
	cur->name=bteifgl_strdup(name);
	
	cur->next=vfile_driver;
	vfile_driver=cur;
	
	return(cur);
}

BTEIFGL_API VFMOUNT *VfMount(char *tgt, char *src, char *fs, char *mode)
{
	VFSDRIVER *drv;
	VFMOUNT *mnt;
	
	drv=VfLookupDriver(fs);
	if(!drv)return(NULL);
	
	mnt=drv->Mount(drv, tgt, src, mode);
	retutn(mnt);
}
