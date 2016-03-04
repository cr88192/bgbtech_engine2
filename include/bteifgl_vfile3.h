#define VF_EPERM            1      /* Operation not permitted */
#define VF_ENOENT           2      /* No such file or directory */
#define VF_ESRCH            3      /* No such process */
#define VF_EINTR            4      /* Interrupted system call */
#define VF_EIO              5      /* I/O error */
#define VF_ENXIO            6      /* No such device or address */
#define VF_EBADF            9      /* Bad file number */
#define VF_ECHILD          10      /* No child processes */
#define VF_EAGAIN          11      /* Try again */
#define VF_ENOMEM          12      /* Out of memory */
#define VF_EACCES          13      /* Permission denied */
#define VF_EFAULT          14      /* Bad address */
#define VF_ENOTBLK         15      /* Block device required */
#define VF_EBUSY           16      /* Device or resource busy */
#define VF_EEXIST          17      /* File exists */
#define VF_ENODEV          19      /* No such device */
#define VF_ENOTDIR         20      /* Not a directory */
#define VF_EISDIR          21      /* Is a directory */
#define VF_EINVAL          22      /* Invalid argument */
#define VF_ENOTTY          25      /* Not a typewriter */
#define VF_EFBIG           27      /* File too large */
#define VF_ENOSPC          28      /* No space left on device */
#define VF_ESPIPE          29      /* Illegal seek */
#define VF_EROFS           30      /* Read-only file system */
#define VF_EDEADLK         35      /* Resource deadlock would occur */
#define VF_ENAMETOOLONG    36      /* File name too long */
#define VF_ENOSYS          38      /* Function not implemented */
#define VF_ENOTEMPTY       39      /* Directory not empty */
#define VF_EWOULDBLOCK     VF_EAGAIN  /* Operation would block */

#define VFILE_ERROR_INV

typedef struct VFSCH_SchedTask_s VFSCH_SchedTask;

typedef struct VFILE_AsIoCb_s VFILE_AsIoCb;
typedef struct VFILE_VT_s VFILE_VT;
typedef struct VFILE_s VFILE;
typedef struct VFMOUNT_VT_s VFMOUNT_VT;
typedef struct VFMOUNT_s VFMOUNT;
typedef struct VFSDRIVER_s VFSDRIVER;

struct VFSCH_SchedTask {
VFSCH_SchedTask *next;
void *data;
void (*Run)(VFSCH_SchedTask *task, void *data);
int (*Idle)(VFSCH_SchedTask *task, void *data, int usec);
double ticktime;		//tick time in microseconds
int prio;				//priority (0=highest, 255=lowest)
};

struct VFILE_AsIoCb_s {
void (*Done)(VFILE_AsIoCb *cb);
void (*Error)(VFILE_AsIoCb *cb, int errn);
void *userdata;

VFILE *fdes;		//File Descriptor
byte *buffer;		//IO Request Buffer
s64 offset;			//IO Request File Offset
s64 size;			//IO Request Size (LoadAs File Size)
s64 maxsize;		//IO Request Maximum Size (Buffer Size)
};

struct VFILE_VT_s {
int (*ReadAs)(VFILE *vf, VFILE_AsIoCb *cb);
int (*WriteAs)(VFILE *vf, VFILE_AsIoCb *cb);
s64 (*GetSize)(VFILE *vf);

// int (*Read)(byte *buf, int sz1, int sz2, VFILE *vf);
// int (*Write)(byte *buf, int sz1, int sz2, VFILE *vf);
// int (*Seek)(VFILE *vf, s64 pos, int rel);
// s64 (*Tell)(VFILE *vf);
int (*Close)(VFILE *vf);
};

struct VFILE_s {
struct VFILE_VT_s *vt;
void *hdl0, *hdl1, *hdl2, *hdl3;
byte data[1];
};

struct VFMOUNT_VT_s {
// VFILE *(*Open)(VFMOUNT *mnt, char *name, char *mode);
int (*OpenAs)(VFMOUNT *mnt, VFILE_AsIoCb *cb,
	char *name, char *mode);
int (*LoadFileAs)(VFMOUNT *mnt, VFILE_AsIoCb *cb, char *name);
int (*StoreFileAs)(VFMOUNT *mnt, VFILE_AsIoCb *cb, char *name);
};

struct VFMOUNT_s {
struct VFMOUNT_VT_s *vt;
VFMOUNT *next;
VFMOUNT *chain;
char *mount_tgt;
char *mount_src;
char *mount_mode;
};

struct VFSDRIVER_s {
VFSDRIVER *next;
char *name;
VFMOUNT *(*Mount)(VFSDRIVER *drv,
	char *tgt, char *src, char *mode);
};
