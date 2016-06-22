typedef struct VF31_VTAB_s VF31_VTAB;
typedef struct VF31_VFMNT_VTAB_s VF31_VFMNT_VTAB;

typedef struct VF31_VTAB_s *VF31;
typedef struct VF31_VFMNT_s VF31_VFMNT;

struct VF31_VTAB_s {
void *rsv0, *rsv1, *rsv2, *rsv3;
int (*VfClose)(VF31 *fd);
int (*VfRead)(void *buf, int s1, int s2, VF31 *fd);
int (*VfWrite)(void *buf, int s1, int s2, VF31 *fd);
int (*VfGetc)(VF31 *fd);
int (*VfPutc)(int v, VF31 *fd);
s64 (*VfTell)(VF31 *fd);
int (*VfSeek)(VF31 *fd, s64 ofs, int rel);
};

struct VF31_VFMNT_VTAB_s {
void *rsv0, *rsv1, *rsv2, *rsv3;
char *drvname;
VF31_VFMNT_VTAB *next;
VF31_VFMNT *(*VfMount)(char *devpath, char *mntpath,
	char *fstype, char *mode);
int (*VfUmount)(VF31_VFMNT *mnt);

VF31 *(*VfOpen)(VF31_VFMNT *mnt, char *path, char *mode);
int (*VfLoadFile)(VF31_VFMNT *mnt, char *path, void **rbuf, int *rsz);
int (*VfStoreFile)(VF31_VFMNT *mnt, char *path, void *buf, int sz);
};

struct VF31_VFMNT_s {
struct VF31_VFMNT_VTAB_s *vt;
VF31_VFMNT *next;
VF31_VFMNT *chain;
char *mntpath;
char *devpath;
void *dptr0;		//driver pointer
void *dptr1;
void *dptr2;
void *dptr3;
};