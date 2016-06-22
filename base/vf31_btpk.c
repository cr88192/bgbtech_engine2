#include <bteifgl.h>

VF31_VTAB vf31_btpk_vtab={
NULL, NULL, NULL, NULL,
vf31_btpk_VfClose,
vf31_btpk_VfRead,
vf31_btpk_VfWrite,
vf31_btpk_VfGetc,
vf31_btpk_VfPutc,
vf31_btpk_VfTell,
vf31_btpk_VfSeek
};

VF31_VFMNT_VTAB vf31_btpk_mnt_vtab_ro={
NULL, NULL, NULL, NULL,
"btpack", NULL,
vf31_btpk_VfMount_RO,
vf31_btpk_VfUmount,
vf31_btpk_VfOpen,
vf31_btpk_VfLoadFile,
vf31_btpk_VfStoreFile_RO,
};

VF31_VFMNT_VTAB vf31_btpk_mnt_vtab_rw={
NULL, NULL, NULL, NULL,
"btpack_rw", NULL,
vf31_btpk_VfMount_RW,
vf31_btpk_VfUmount,
vf31_btpk_VfOpen,
vf31_btpk_VfLoadFile,
vf31_btpk_VfStoreFile,
};

int vf31_btpk_init(void)
{
	VfRegisterVfsDrv(&vf31_btpk_mnt_vtab_ro);
	VfRegisterVfsDrv(&vf31_btpk_mnt_vtab_rw);
	return(0);
}

int vf31_btpk_VfClose(VF31 *fd)
{
	BtPak0_OpenFile *fd1;
	fd1=(BtPak0_OpenFile *)fd;
	BtPak_FileClose(fd1);
	return(0);
}

int vf31_btpk_VfRead(void *buf, int s1, int s2, VF31 *fd)
{
	BtPak0_OpenFile *fd1;
	fd1=(BtPak0_OpenFile *)fd;
	return(BtPak_FileRead(buf, s1, s2, fd1));
}

int vf31_btpk_VfWrite(void *buf, int s1, int s2, VF31 *fd)
{
	BtPak0_OpenFile *fd1;
	fd1=(BtPak0_OpenFile *)fd;
	return(BtPak_FileWrite(buf, s1, s2, fd1));
}

int vf31_btpk_VfGetc(VF31 *fd)
{
	byte tb[16];
	BtPak0_OpenFile *fd1;
	fd1=(BtPak0_OpenFile *)fd;
	BtPak_FileRead(tb, 1, 1, fd1);
	return(tb[0]);
//	return(fgetc(fd1->fd));
}

int vf31_btpk_VfPutc(int v, VF31 *fd)
{
	byte tb[16];
	BtPak0_OpenFile *fd1;
	int i;

	fd1=(BtPak0_OpenFile *)fd;
	tb[0]=v;
	i=BtPak_FileWrite(tb, 1, 1, fd1);
	return(i);
}

s64 vf31_btpk_VfTell(VF31 *fd)
{
	BtPak0_OpenFile *fd1;
	fd1=(BtPak0_OpenFile *)fd;
	return(BtPak_FileTell(fd1));
}

int vf31_btpk_VfSeek(VF31 *fd, s64 ofs, int rel)
{
	BtPak0_OpenFile *fd1;
	fd1=(BtPak0_OpenFile *)fd;
	return(BtPak_FileSeek(fd1, ofs, rel));
}

VF31_VFMNT *vf31_btpk_VfMount_RO(char *devpath, char *mntpath,
	char *fstype, char *mode)
{
	VF31_VFMNT *tmp;
	BtPak0_Image *img;
	
	img=BtPak_LoadImageRO(devpath);
	if(!img)
		return(NULL);
	
	tmp=frgl_malloc(sizeof(VF31_VFMNT));
	tmp->vt=&vf31_btpk_mnt_vtab_ro;
	tmp->devpath=frgl_strdup(devpath);
	tmp->mntpath=frgl_strdup(mntpath);
	
	tmp->dptr0=img;
	
	return(tmp);
}

VF31_VFMNT *vf31_btpk_VfMount_RW(char *devpath, char *mntpath,
	char *fstype, char *mode)
{
	VF31_VFMNT *tmp;
	BtPak0_Image *img;
	
	img=BtPak_LoadImageRW(devpath);
	if(!img)
		return(NULL);
	
	tmp=frgl_malloc(sizeof(VF31_VFMNT));
	tmp->vt=&vf31_btpk_mnt_vtab_rw;
	tmp->devpath=frgl_strdup(devpath);
	tmp->mntpath=frgl_strdup(mntpath);
	
	tmp->dptr0=img;
	
	return(tmp);
}

int vf31_btpk_VfUmount(VF31_VFMNT *mnt)
{
	VfUnlinkMount(mnt);
	frgl_free(mnt);
	return(0);
}

VF31 *vf31_btpk_VfOpen(VF31_VFMNT *mnt, char *path, char *mode)
{
	char tbuf[1024];
	BtPak0_OpenFile *fd;
	
	sprintf(tbuf, "%s/%s", mnt->devpath, path);
	fd=BtPak_ImageFileOpen(mnt->dptr0, path, mode);
	if(!fd)return(NULL);
	fd->vt=&vf31_btpk_vtab;
	return((VF31 *)fd);
}

int vf31_btpk_VfLoadFile(
	VF31_VFMNT *mnt, char *path, void **rbuf, int *rsz)
{
	int i;
	i=BtPak_ImageLoadFile(mnt->dptr0, path, (byte **)rbuf, rsz);
	return(i);
}

int vf31_btpk_VfStoreFile(VF31_VFMNT *mnt, char *path, void *buf, int sz)
{
	int i;
	i=BtPak_ImageStoreFile(mnt->dptr0, path, buf, sz);
	return(i);
}

int vf31_btpk_VfStoreFile_RO(VF31_VFMNT *mnt, char *path, void *buf, int sz)
{
	return(-1);
}
