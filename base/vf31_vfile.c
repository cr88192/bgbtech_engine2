#include <bteifgl.h>

VF31_VFMNT_VTAB *vf31_vfsdrv;
VF31_VFMNT *vf31_mtab;

BTEIFGL_API int VfClose(VF31 *fd)
	{ return((*fd)->VfClose(fd)); }

BTEIFGL_API int VfRead(void *buf, int s1, int s2, VF31 *fd)
	{ return((*fd)->VfRead(buf, s1, s2, fd)); }

BTEIFGL_API int VfWrite(void *buf, int s1, int s2, VF31 *fd)
	{ return((*fd)->VfWrite(buf, s1, s2, fd)); }

BTEIFGL_API int VfGetc(VF31 *fd)
	{ return((*fd)->VfGetc(fd)); }

BTEIFGL_API int VfPutc(int v, VF31 *fd)
	{ return((*fd)->VfPutc(v, fd)); }

BTEIFGL_API s64 VfTell(VF31 *fd)
	{ return((*fd)->VfTell(fd)); }

BTEIFGL_API int VfSeek(VF31 *fd, s64 ofs, int rel)
	{ return((*fd)->VfSeek(fd, ofs, rel)); }


BTEIFGL_API VF31_VFMNT *VfMount(char *devpath, char *mntpath,
	char *fstype, char *mode)
{
	char pbuf[1024];
	VF31_VFMNT_VTAB *vcur;
	VF31_VFMNT *mnt;
	char *s, *t;

	VfInitVfs();

	s=mntpath; t=pbuf;
	while((*s=='/') || *s=='\\')
		s++;
	while(*s)
	{
		if((*s=='/') || *s=='\\')
		{
			*t++='/';
			while((*s=='/') || *s=='\\')
				s++;
			continue;
		}
		*t++=*s++;
	}
	if(t>pbuf)
		*t++='/';
	*t++=0;

	vcur=vf31_vfsdrv;
	while(vcur)
	{
		if(vcur->drvname && !strcmp(vcur->drvname, fstype))
		{
			mnt=vcur->VfMount(devpath, pbuf, fstype, mode);
			if(mnt)
			{
				mnt->next=vf31_mtab;
				vf31_mtab=mnt;
				return(mnt);
			}
		}
		vcur=vcur->next;
	}

	vcur=vf31_vfsdrv;
	while(vcur)
	{
		if(!vcur->drvname)
		{
			mnt=vcur->VfMount(devpath, pbuf, fstype, mode);
			if(mnt)
			{
				mnt->next=vf31_mtab;
				vf31_mtab=mnt;
				return(mnt);
			}
		}
		vcur=vcur->next;
	}

	return(NULL);
}

BTEIFGL_API int VfUmount(VF31_VFMNT *mnt)
{
	return(mnt->vt->VfUmount(mnt));
}

BTEIFGL_API int VfUnlinkMount(VF31_VFMNT *mnt)
{
	VF31_VFMNT *mcur, *mlst;
	
	mcur=vf31_mtab; mlst=NULL;
	while(mcur)
	{
		if(mcur==mnt)
		{
			if(mlst)
			{
				mlst->next=mcur->next;
			}else
			{
				vf31_mtab=mcur->next;
			}
		}
	
		mlst=mcur;
		mcur=mcur->next;
	}
}

BTEIFGL_API void VfNormalizePath(char *path, char *pbuf)
{
	char *s, *t;
	
	s=path; t=pbuf;
	while((*s=='/') || *s=='\\')
		s++;
	while(*s)
	{
		if((*s=='/') || *s=='\\')
		{
			*t++='/';
			while((*s=='/') || *s=='\\')
				s++;
			continue;
		}
		*t++=*s++;
	}
	*t++=0;
}

BTEIFGL_API VF31_VFMNT *VfQueryPathMounts(char *path)
{
	char pbuf[1024];
	VF31_VFMNT *mcur, *mlst;
	VF31 *fd;
	int l;

	VfNormalizePath(path, pbuf);

	mcur=vf31_mtab; mlst=NULL;
	while(mcur)
	{
		l=strlen(mcur->mntpath);
		if(!strncmp(mcur->mntpath, pbuf, l))
		{
			mcur->chain=mlst;
			mlst=mcur;
		}
		mcur=mcur->next;
	}

	return(mlst);

//	return(mnt->vt->VfOpen(mnt, path, mode));
}

BTEIFGL_API VF31 *VfOpen(char *path, char *mode)
{
	char pbuf[1024];
	VF31_VFMNT *mcur, *mlst;
	VF31 *fd;
	int l;

	VfInitVfs();

	VfNormalizePath(path, pbuf);
	mlst=VfQueryPathMounts(path);

	mcur=mlst;
	while(mcur)
	{
		l=strlen(mcur->mntpath);
		fd=mcur->vt->VfOpen(mcur, pbuf+l, mode);
		if(fd)
			return(fd);
		mcur=mcur->chain;
	}
	return(NULL);
}

BTEIFGL_API int VfLoadFile(char *path, void **rbuf, int *rsz)
{
	char pbuf[1024];
	VF31_VFMNT *mcur, *mlst;
	VF31 *fd;
	int i, j, k, l;

	VfInitVfs();

	VfNormalizePath(path, pbuf);
	mlst=VfQueryPathMounts(path);

	mcur=mlst;
	while(mcur)
	{
		l=strlen(mcur->mntpath);
		i=mcur->vt->VfLoadFile(mcur, pbuf+l, rbuf, rsz);
		if(i>=0)return(i);
		mcur=mcur->chain;
	}
	return(-1);
}

BTEIFGL_API void *VfLoadFile2(char *path, int *rsz)
{
	void *buf;
	int i, sz;
	
	buf=NULL;
	i=VfLoadFile(path, &buf, &sz);
	if(buf && (i>0))
	{
		*rsz=sz;
		return(buf);
	}
	
	return(NULL);
}

BTEIFGL_API int VfStoreFile(char *path, void *buf, int sz)
{
	char pbuf[1024];
	VF31_VFMNT *mcur, *mlst;
	VF31 *fd;
	int i, j, k, l;

	VfInitVfs();

	VfNormalizePath(path, pbuf);
	mlst=VfQueryPathMounts(path);

	mcur=mlst;
	while(mcur)
	{
		l=strlen(mcur->mntpath);
		i=mcur->vt->VfStoreFile(mcur, pbuf+l, buf, sz);
		if(i>=0)return(i);
		mcur=mcur->chain;
	}
	return(-1);
}

BTEIFGL_API int VfRegisterVfsDrv(VF31_VFMNT_VTAB *drv)
{
	drv->next=vf31_vfsdrv;
	vf31_vfsdrv=drv;
	return(0);
}

BTEIFGL_API int VfInitVfs(void)
{
	static int init=0;
	if(init)
		return(0);
	init=1;
	
	vf31_osdir_init();
	vf31_btpk_init();
	
	VfMount(".", "/", "dir", "");
	VfMount("resource", "/", "dir", "");
	return(1);
}
