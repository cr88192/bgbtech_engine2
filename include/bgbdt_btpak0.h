#define BTPAK_CM_STORE		0
#define BTPAK_CM_DEFLATE	8
#define BTPAK_CM_DEFLATE64	9
#define BTPAK_CM_BTLZH		10

#define BTPAK_CFL_MULTI		1
#define BTPAK_CFL_ISBM		2

#define BTPAK_DE_NONE		1
#define BTPAK_DE_FILE		1
#define BTPAK_DE_DIR		2


typedef struct { byte v[2]; } btpk_u16;
typedef struct { byte v[4]; } btpk_u32;
typedef struct { byte v[8]; } btpk_u64;

typedef struct BtPak0_Header_s BtPak0_Header;
typedef struct BtPak0_ChunkInfo_s BtPak0_ChunkInfo;
typedef struct BtPak0_DirEnt_s BtPak0_DirEnt;

typedef struct BtPak0_LoadChunk_s BtPak0_LoadChunk;
typedef struct BtPak0_Image_s BtPak0_Image;
typedef struct BtPak0_OpenFile_s BtPak0_OpenFile;

struct BtPak0_Header_s {
btpk_u64 magic;			//"BT2PACK0"
btpk_u32 dirChkId;		//Directory Chunk Id
btpk_u32 nDirEnts;		//Number of directory entries.
btpk_u64 ofsChkInfo;	//Offset of chunk info table.
btpk_u32 nChunks;		//Number of chunks in image.
btpk_u32 blkmChkId;		//Block map chunk id (0 if unused)
};

struct BtPak0_ChunkInfo_s {
btpk_u64 ofsChk;		//offset of chunk within image
btpk_u32 szChk;			//size of chunk (logical)
btpk_u32 cszChk;		//compressed size of chunk
/* 16B */
btpk_u32 nextChk;		//next chunk (multi-chunk files)
byte cm;				//Compression Method
byte cfl;				//Chunk Flags
byte shOsz;				//Shift for offset+size
byte shCell;			//Shift for cells
btpk_u16 ctId;			//Content ID
byte pad[6];
/* 32B */
};

struct BtPak0_DirEnt_s {
byte name[40];			//name (excludes path, UTF-8)
btpk_u64 oszFile;		//offset + size of file (within chunk)
/* 48B */
btpk_u32 chkId;			//chunk containing file data
btpk_u32 deNext;		//next DirEnt in current directory
byte deTy;				//DirEnt type
byte deFl;				//DirEnt flags
byte resv[6];			//
/* 64B */
};


struct BtPak0_LoadChunk_s {
BtPak0_LoadChunk *next;
u32 chkId;
void *data;
int szData;
byte isDirty;
byte cm;
byte cfl;
byte shOsz;				//Shift for offset+size
byte shCell;			//Shift for cells
u16 ctid;				//Content Type ID

BtPak0_LoadChunk *lcBm;
byte *bm;
int n_cell, m_cell;
int rov;
};

struct BtPak0_Image_s {
BtPak0_Image *next;
BtPak0_Header head;
BtPak0_ChunkInfo *chki;
BtPak0_LoadChunk *lchk;
BtPak0_LoadChunk *lcDir;
BtPak0_LoadChunk *lcBlkm;
char *name;
FILE *fd;

int ofs_chunks;		//offset of chunks
int n_chunks;		//number of chunks
int m_chunks;		//max chunks

int n_dirent;		//num dirents
int m_dirent;		//max dirents

byte *blkm;			//block bitmap
int n_blocks;		//number of blocks in image
int m_blocks;		//max blocks in bitmap
int blkrov;
};

struct BtPak0_OpenFile_s {
VF31_VTAB *vt;
BtPak0_Image *img;
u32 deId;
byte *data;
u32 szData;
u32 pos;
};
