#include <bteifgl.h>

VF31_VTAB vf31_osdir_vtab={
NULL, NULL, NULL, NULL,
vf31_osdir_VfClose,
vf31_osdir_VfRead,
vf31_osdir_VfWrite,
vf31_osdir_VfGetc,
vf31_osdir_VfPutc,
vf31_osdir_VfTell,
vf31_osdir_VfSeek
};

VF31_VFMNT_VTAB vf31_osdir_mnt_vtab={
NULL, NULL, NULL, NULL,
"dir", NULL,
vf31_osdir_VfMount,
vf31_osdir_VfUmount,
vf31_osdir_VfOpen,
vf31_osdir_VfLoadFile,
vf31_osdir_VfStoreFile,
};

struct VF31_OSD_VFILE_s {
VF31_VTAB *vt;
FILE *fd;
};

int vf31_osdir_init(void)
{
	VfRegisterVfsDrv(&vf31_osdir_mnt_vtab);
	return(0);
}

int vf31_osdir_VfClose(VF31 *fd)
{
	struct VF31_OSD_VFILE_s *fd1;
	fd1=(struct VF31_OSD_VFILE_s *)fd;
	fclose(fd1->fd);
	frgl_free(fd);
	return(0);
}

int vf31_osdir_VfRead(void *buf, int s1, int s2, VF31 *fd)
{
	struct VF31_OSD_VFILE_s *fd1;
	fd1=(struct VF31_OSD_VFILE_s *)fd;
	return(fread(buf, s1, s2, fd1->fd));
}

int vf31_osdir_VfWrite(void *buf, int s1, int s2, VF31 *fd)
{
	struct VF31_OSD_VFILE_s *fd1;
	fd1=(struct VF31_OSD_VFILE_s *)fd;
	return(fwrite(buf, s1, s2, fd1->fd));
}

int vf31_osdir_VfGetc(VF31 *fd)
{
	struct VF31_OSD_VFILE_s *fd1;
	fd1=(struct VF31_OSD_VFILE_s *)fd;
	return(fgetc(fd1->fd));
}

int vf31_osdir_VfPutc(int v, VF31 *fd)
{
	struct VF31_OSD_VFILE_s *fd1;
	fd1=(struct VF31_OSD_VFILE_s *)fd;
	return(fputc(v, fd1->fd));
}

s64 vf31_osdir_VfTell(VF31 *fd)
{
	struct VF31_OSD_VFILE_s *fd1;
	fd1=(struct VF31_OSD_VFILE_s *)fd;
	return(ftell(fd1->fd));
}

int vf31_osdir_VfSeek(VF31 *fd, s64 ofs, int rel)
{
	struct VF31_OSD_VFILE_s *fd1;
	fd1=(struct VF31_OSD_VFILE_s *)fd;
	return(fseek(fd1->fd, ofs, rel));
}

VF31_VFMNT *vf31_osdir_VfMount(char *devpath, char *mntpath,
	char *fstype, char *mode)
{
	VF31_VFMNT *tmp;
	
	tmp=frgl_malloc(sizeof(VF31_VFMNT));
	tmp->vt=&vf31_osdir_mnt_vtab;
	tmp->devpath=frgl_strdup(devpath);
	tmp->mntpath=frgl_strdup(mntpath);
	
	return(tmp);
}

int vf31_osdir_VfUmount(VF31_VFMNT *mnt)
{
	VfUnlinkMount(mnt);
	frgl_free(mnt);
	return(0);
}

VF31 *vf31_osdir_VfOpen(VF31_VFMNT *mnt, char *path, char *mode)
{
	char tbuf[1024];
	struct VF31_OSD_VFILE_s *fd1;
	FILE *fd;
	
	sprintf(tbuf, "%s/%s", mnt->devpath, path);
	fd=fopen(tbuf, mode);
	if(!fd)
		return(NULL);
	fd1=frgl_malloc(sizeof(struct VF31_OSD_VFILE_s));
	fd1->vt=&vf31_osdir_vtab;
	fd1->fd=fd;
	
	return((VF31 *)fd1);
}

int vf31_osdir_VfLoadFile(
	VF31_VFMNT *mnt, char *path, void **rbuf, int *rsz)
{
	char tbuf[1024];
	FILE *fd;
	byte *buf;
	int sz, fsz;

	sprintf(tbuf, "%s/%s", mnt->devpath, path);
	fd=fopen(tbuf, "rb");
	if(!fd)return(-1);
	
	fseek(fd, 0, 2);
	fsz=ftell(fd);
	fseek(fd, 0, 0);
	
	buf=*rbuf;
	sz=*rsz;
	
	if(buf)
	{
		if(fsz>sz)
		{
			fclose(fd);
			*rsz=fsz;
			return(0);
		}
	}else
	{
		buf=frgl_malloc(fsz+16);
	}
	
	fread(buf, 1, fsz, fd);
	fclose(fd);
	*rbuf=buf;
	*rsz=fsz;
	return(1);
}

int vf31_osdir_VfStoreFile(VF31_VFMNT *mnt, char *path, void *buf, int sz)
{
	char tbuf[1024];
	FILE *fd;
	int fsz;

	sprintf(tbuf, "%s/%s", mnt->devpath, path);
	fd=fopen(tbuf, "wb");
	if(!fd)return(-1);
	
	fwrite(buf, 1, sz, fd);
	fclose(fd);
	return(1);
}
