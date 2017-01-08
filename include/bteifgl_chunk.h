/*

Voxels will be organized into chunks and regions.

Chunk = 16x16x16 voxels
Region = 16x16x16 chunks

Voxel Info Planes
  +0=Voxel Base Type (Bits 0..7)
  +1=Voxel Base Type (High 4), Voxel Mode
  +2=Voxel Attribute Bits
  +3=Block Light Color(0..3), Light Level (4..7)
  +4=Area Light ID (0..2), Light Level (3..7)
  +5=Voxel Data (Low)
  +6=Voxel Data (High)

Voxel Mode:
  &1=Powered
  &2=Unbreakable
  &4=Flexform
  &8=Reserved

Area light is monochromatic.
  ID Selects the logical light source.
    0=Constant, falls off uniformly in every direction.
    1/2/3=Sky 1/2/3, falls off in X/Y, no falloff in Z.
    4-7=Resv/Special

Block light may be colored, with a 2-level 16 color palette.
  0: White (Normalized)
  1-6: RGB colors, higher saturation.
  7/8: Reserved
  9-14: RGB colors, low saturation.
  15: White (Full)
  
  Higher saturation colors will be more obviously that color.
  Low saturation colors will be tinted in that direction.
    240,60,60 vs 192,128,128

Voxel Attribute Bits:
  Exact meaning depends mostly on block type

Voxel Data:
  Intended as a data field for certain block types.
  Nominally interpreted as a 16-bit number, or a string index.

  ent_mob: string index for classname of mob to spawn, or entity-def


Coordinate Space:
  20.12
  
  +X=East
  +Y=North
  +Z=Up


Chunk Format
  E4, Word24, 'CHK0'
    Context Data (E1 tag containing a coded bitstream)

Tag Rice(3)
  0: End of Chunk
  1: Chunk Table Size (TS), Rice(7)
    Gives the number of unique block types in the chunk.
    1..256: Use 8-bit indices.
    257..4096: Use 12-bit indices.
  2: Block Plane Mask, Rice(7)
    Encodes which planes are present in the Block Plane Data.
    Absent planes are assumed to be zeroes.
    TSz=TS*TP (Where, TP is the number of planes present).
  3: Paramater Value
    Rice(3) Index
    QExpBase(5) Value

  4: Index8 Plane (LZ:4096)
	May be absent if chunk only has a single block type.
	May be omitted in favor of an Index4 plane if TS<=16.
  5: Index12 or Index4 Plane (Optional, LZ:2048)
    Gives bits 8-11 of each index (Index12).
    Will be absent for chunks with 8-bit indices.
    If present w/o Index8 and TS<=16, Interpret as Index4
    Index4 will encode each index in 4 bits.
  6: Block Plane Data (LZ:TSz)
    Gives data for the planes which are present.
    Planes are laid out in order.
    Data for one plane for all blocks will precede that for the next plane.

Chunk LZ (Old)
  AdRiceDC(S) 1..256: SMTF Symbol
  AdRiceDC(S) 0: Esc
    AdSRiceDC(L)
      <=0 Run Length
        AdRiceDC: Distance
      >0: Special Commands
        1=EOB
	  Lengths and Distances of 0 will indicate to reuse the last value.

ChunkLZ data may not exceed the allowed number of bytes.
It may encode less than the allowed byte count, in which case any remaining 
bytes are to be zeroed.

Chunk LZ (New)
  AdRiceDC(R) Number of Raw Symbols (Raw)
	AdRiceDC(S) 0..255: Raw Symbols (STF2)
  AdRiceDC(L): Length (Len)
    0=No Run/EOB
    >0=Length
      AdRiceDC(D): Distance(Dist)

(Raw==0) && (Len==0): EOB
  No distance is encoded.
(Raw!=0) && (Len==0): No Match
(Len!=0): Match
  Len gives the number of bytes, with a minimum match of 3.
    (Len==1), Reuse last Length
    (Len==2), Special (Dist==Tag)
  Dist gives the distance in the sliding window.
    If (Dist==0), Reuse last dist.

Parameters:
  1=Chunk Flags


Region:
E4, Word24, 'RGN0'
  Region Header
  E4, Word24, 'OFS4'

'OFS4': Chunk offsets as 32-bit words.
'OFS3': Chunk offsets as 24-bit words.

'CEBM': Cell Bitmap (Inline)
	Directly stores the bitmap data.
'COBM': Cell Bitmap (Offset)
	Word32 offs;	//Offset of bitmap in region image
	Word32 size;	//Size of bitmap in region image

'STRS': String Data (Inline)
	String Table Data.

Cell Bitmap: 2 Bits/Cell
  00=Free Cell
  01=Alloc Head
  10=Alloc Data
  11=Reserved

Each cell is 64 bytes.
*/

#define BGBDT_ACCFL_NOLOAD		1		//don't load chunks, fail silently
#define BGBDT_ACCFL_ENNEWRGN	2		//enable creating new regions
#define BGBDT_ACCFL_ENNEWCHK	4		//enable creating new chunks
#define BGBDT_ACCFL_CHKADJ		8		//check adjacent blocks
#define BGBDT_ACCFL_LIGHTDIRTY	16		//only lighting has changed
#define BGBDT_ACCFL_DISTANT		32		//represents a distant chunk

#define BGBDT_TRFL_NOLOAD		1		//don't load chunks during trace
#define BGBDT_TRFL_SOLID		2		//stop if we hit something solid
#define BGBDT_TRFL_NONEMPTY		4		//stop if we hit something not air
#define BGBDT_TRFL_OPAQUE		8		//stop if we hit something opaque
#define BGBDT_TRFL_NONCHUNK		16		//stop if missing chunk

#define BGBDT_VOXFL_NONSOLID	0x0001	//non-solid to entities
#define BGBDT_VOXFL_TRANSPARENT	0x0002	//transparent
#define BGBDT_VOXFL_NOFACES		0x0004	//does not emit faces
#define BGBDT_VOXFL_ATLAS		0x0008	//uses a texture atlas
#define BGBDT_VOXFL_CONT0		0x0010	//contents bit 0
#define BGBDT_VOXFL_CONT1		0x0020	//contents bit 1
#define BGBDT_VOXFL_CONT2		0x0040	//contents bit 2
#define BGBDT_VOXFL_CONT3		0x0080	//contents bit 3
#define BGBDT_VOXFL_FLUID		0x0100	//is a fluid
#define BGBDT_VOXFL_GLOWLIGHT	0x0200	//light source
#define BGBDT_VOXFL_PHYSEFF		0x0400	//physics effect
#define BGBDT_VOXFL_CROSSSPR	0x0800	//cross sprite
#define BGBDT_VOXFL_SOLIDNSBOX	0x1000	//solid non-standard box

#define BGBDT_VOXFL_CONT_0		0x0000		//contents=0
#define BGBDT_VOXFL_CONT_1		0x0010		//contents=1
#define BGBDT_VOXFL_CONT_2		0x0020		//contents=2
#define BGBDT_VOXFL_CONT_3		0x0030		//contents=3

#define BGBDT_VOXFL_CONT_MASK	0x00F0		//contents=3

#define BGBDT_VOXFL_FLUID_WATER	0x0110
#define BGBDT_VOXFL_FLUID_LAVA	0x0120
#define BGBDT_VOXFL_FLUID_SLIME	0x0130

#define BGBDT_VOXFL_FLUID_MASK	0x01F0

#define BGBDT_VOXFL_PHYS_FLIPUP	0x0410
#define BGBDT_VOXFL_PHYS_FLIPDN	0x0420
#define BGBDT_VOXFL_PHYS_TRIG	0x0430
#define BGBDT_VOXFL_PHYS_MASK	0x04F0

#define BGBDT_ADJFL_SOLID_NX	0x0001	//solid -X
#define BGBDT_ADJFL_SOLID_PX	0x0002	//solid +X
#define BGBDT_ADJFL_SOLID_NY	0x0004	//solid -Y
#define BGBDT_ADJFL_SOLID_PY	0x0008	//solid +Y
#define BGBDT_ADJFL_SOLID_NZ	0x0010	//solid -Z
#define BGBDT_ADJFL_SOLID_PZ	0x0020	//solid +Z
#define BGBDT_ADJFL_OPAQUE_NX	0x0040	//opaque -X
#define BGBDT_ADJFL_OPAQUE_PX	0x0080	//opaque +X
#define BGBDT_ADJFL_OPAQUE_NY	0x0100	//opaque -Y
#define BGBDT_ADJFL_OPAQUE_PY	0x0200	//opaque +Y
#define BGBDT_ADJFL_OPAQUE_NZ	0x0400	//opaque -Z
#define BGBDT_ADJFL_OPAQUE_PZ	0x0800	//opaque +Z

#define BGBDT_ADJFL_SOLID_MASK	0x003F	//solid -X
#define BGBDT_ADJFL_OPAQUE_MASK	0x0FC0	//solid -X

#define BGBDT_ADJFL_SOLID_S		0x1000	//solid self
#define BGBDT_ADJFL_OPAQUE_S	0x2000	//opaque self

#define BGBDT_XYZ_SHR_METER		12
#define BGBDT_XYZ_SHR_VOXEL		12
#define BGBDT_XYZ_SHR_CHUNK		16
#define BGBDT_XYZ_SHR_REGION	20
#define BGBDT_XYZ_SHR_REGION_XY	20
#define BGBDT_XYZ_SHR_REGION_Z	20

#define BGBDT_XYZ_SHR_VOXEL4	14

#define BGBDT_XYZ_MASK_CHKVOX	0x0F
#define BGBDT_XYZ_MASK_CHUNK_XY	0x0F
#define BGBDT_XYZ_MASK_CHUNK_Z	0x0F

#define BGBDT_XYZ_MASK_REGION	((1<<BGBDT_XYZ_SHR_REGION_XY)-1)

#define BGBDT_XYZ_SZ_CHUNK_XYZ	16
#define BGBDT_XYZ_TSZ_CHK_XY	(16*16)
#define BGBDT_XYZ_TSZ_CHK_XYZ	(16*16*16)

#define BGBDT_XYZ_SZ_REGION_XY	16
#define BGBDT_XYZ_SZ_REGION_Z	16
#define BGBDT_XYZ_TSZ_RGN_XYZ	(16*16*16)

#define BGBDT_XYZ_OFS_VOXEL		(1<<BGBDT_XYZ_SHR_VOXEL)
#define BGBDT_XYZ_MASK_VOXEL	((1<<BGBDT_XYZ_SHR_VOXEL)-1)

#define BGBDT_XYZ_SCALE_TOMETER		(1.0/4096)
#define BGBDT_XYZ_SCALE_TOCHUNK		(1.0/65536)
#define BGBDT_XYZ_SCALE_TOVOX4		(1.0/16384)
#define BGBDT_XYZ_SCALE_TOCHUNK4	(1.0/(4*65536))
#define BGBDT_XYZ_SCALE_TOCHUNK16	(1.0/(16*65536))

#define BGBDT_XYZ_SCALE_FROMMETER	(4096.0)

#define BGBDT_VTYFL_POWERED		0x1000
#define BGBDT_VTYFL_UNBREAKABLE	0x2000
#define BGBDT_VTYFL_FLEXFORM	0x4000

#define BGBDT_VTXFL_NOMOVE		0x0001	//vertex can't move

#define BGBDT_CHKFL_DIRTY		0x0001	//chunk is dirty
#define BGBDT_CHKFL_MESHDIRTY	0x0002	//chunk needs mesh updated
#define BGBDT_CHKFL_LIGHTDIRTY	0x0004	//should recalculate lighting.
#define BGBDT_CHKFL_SAVEDIRTY	0x0008	//needs to be resaved

#define BGBDT_CHKFL_ALLDIRTY	0x000F	//needs to be resaved

#define BGBDT_CHKFL_ONLYSOLID	0x0010	//only opaque solid blocks
#define BGBDT_CHKFL_ONLYAIR		0x0020	//only 'air'
#define BGBDT_CHKFL_MIXEDSOLID	0x0040	//mixture of solid and air
#define BGBDT_CHKFL_NOVISFACE	0x0080	//no visible faces (general)

#define BGBDT_CHKFL_ENTSPAWN	0x1000	//entities spawned

#define BGBDT_RGNFL_NEWRGN		0x0001	//newly created region
#define BGBDT_RGNFL_RGNDIRTY	0x0002	//region chunks have changed

#define BGBDT_RGN_CELLSHR		6		//region cell shift
#define BGBDT_RGN_CELLPAD		63		//region cell shift

#define BGBDT_ENTFL_ONGROUND	0x0001	//onground
#define BGBDT_ENTFL_ZFLIP		0x0010	//entity Z flip

#define BGBDT_MESHFL_QLIVE		0x0001	//query live
#define BGBDT_MESHFL_QCLIP		0x0002	//query clipped
#define BGBDT_MESHFL_QINHIBIT	0x0004	//query inhibited

#define BGBDT_MESHFL_VISALL		0x0007	//query clipped

#define BGBDT_MESHFL_NANFAIL	0x0010	//query inhibited
#define BGBDT_MESHFL_DRAW1		0x0020	//drawn
#define BGBDT_MESHFL_RELOAD1	0x0040	//drawn


typedef struct BGBDT_VoxCoord_s BGBDT_VoxCoord;
typedef struct BGBDT_VoxVertex_s BGBDT_VoxVertex;
typedef struct BGBDT_VoxTriangle_s BGBDT_VoxTriangle;

typedef struct BGBDT_VoxData_s BGBDT_VoxData;
typedef struct BGBDT_VoxDataStatus_s BGBDT_VoxDataStatus;

typedef struct BGBDT_VoxRenderEntity_s BGBDT_VoxRenderEntity;

typedef struct BGBDT_VoxChunkMesh_s BGBDT_VoxChunkMesh;
typedef struct BGBDT_VoxChunk_s BGBDT_VoxChunk;
typedef struct BGBDT_VoxRegion_s BGBDT_VoxRegion;
typedef struct BGBDT_VoxWorld_s BGBDT_VoxWorld;

typedef struct BGBDT_VoxTypeInfo_s BGBDT_VoxTypeInfo;

struct BGBDT_VoxCoord_s {
s32 x;		//X coordinate
s32 y;		//Y coordinate
s32 z;		//Z coordinate
};

struct BGBDT_VoxVertex_s {
s32 x, y, z;
int fl;
int next;
};

struct BGBDT_VoxTriangle_s {
int v0, v1, v2;		//vertices
int mat;			//material
int atxy;			//atlas position
int light;			//light level
int matnext;		//next for a given material
};

struct BGBDT_VoxData_s {
// u16 vtype;		//low 12: voxel type, high 4: voxel mode
byte vtypel;	//low 8 bits of voxel type
byte vtypeh;	//low 4: voxel type (high), high 4: voxel mode
byte vattr;		//voxel attribute flags
byte vlight;	//voxel block light
byte alight;	//voxel area light
byte vdatal;	//voxel data low
byte vdatah;	//voxel data high
byte resv0;		//voxel reserved
};

struct BGBDT_VoxDataStatus_s {
u32 adjfl;
u16 adjlit[6];
};

struct BGBDT_VoxRenderEntity_s {
BGBDT_VoxRenderEntity *next;		//entity in region
BGBDT_VoxRenderEntity *visnext;		//next visible entity in region
BGBDT_VoxCoord org;					//origin
int entid;							//entity ID
int entfl;							//entity flags
int xs, ys;							//sprite size
int xo, yo;							//sprite offset
int mat;							//sprite frame atlas
int mat_w, mat_h;					//size of atlas in frames
int frame;							//frame
};

struct BGBDT_VoxChunkMesh_s {
BGBDT_VoxRegion *rgn;		//parent region
BGBDT_VoxChunkMesh *next;	//next chunk mesh
byte bx, by, bz;			//chunk location (in region)
byte vistick;				//visibility tick delay

BGBDT_VoxChunkMesh *pvsnext;	//next chunk mesh (PVS)
BGBDT_VoxChunkMesh *cvsnext;	//next chunk mesh (CVS)

//for rebuilding chunk
BGBDT_VoxVertex *xyz;		//vertices
BGBDT_VoxTriangle *tris;	//triangles
int nxyz, mxyz;				//num and max vertices
int ntris, mtris;			//num and max triangles
int *mat;					//material triangle chains
int nmat, mmat;				//number of materials

int xyzh[64];
int tmat[16*2];

float *va_xyz;				//XYZ
float *va_st;				//ST
byte *va_rgba;				//RGBA
byte *va_norm;				//xyz
int *va_elem;				//element indices (3 for each triangle)
int *va_mesh;				//+0=elem start, +1=elem count, +2=mat, +3=resv
int va_nvec;
int va_nelem;
int va_nmesh;

byte *vabuf;
int sz_vabuf;
int vbo_id;
int oqm_id;
int flags;

int ofs_xyz, ofs_st;
int ofs_rgba, ofs_norm;
int ofs_elem, ofs_mesh;
};

struct BGBDT_VoxChunk_s {
BGBDT_VoxRegion *rgn;		//parent region
BGBDT_VoxChunk *next;		//next (visible) chunk
byte *voxidx;				//voxel index (low bits)
byte *voxidxh;				//voxel index (high bits)
BGBDT_VoxData *voxinfo;		//voxel info
short nvoxinfo;				//number of voxel-info entries
short mvoxinfo;				//max number of voxel-info entries
byte bx, by, bz;			//chunk location (in region)
byte ticklim;				//tick limit
int flags;					//chunk flags
};

struct BGBDT_VoxRegion_s {
BGBDT_VoxWorld *world;		//parent world
BGBDT_VoxRegion *next;		//next region is world
int bx, by, bz;				//base X/Y/Z

int tbx, tby, tbz;			//tick X/Y/Z
int flags;

byte *rgnmap;				//region cell bitmap
byte *rgnbuf;				//region buffer
int szrgnbuf;				//size of region buffer
int nrgncell;				//number of region cells

byte *strtab_buf;
int strtab_nsz;
int strtab_msz;

int *strix_ofs;
int strix_num;
int strix_max;

BGBDT_VoxChunk *chkptr[16*16*16];
BGBDT_VoxChunkMesh *chkmesh[16*16*16];
int chkofs[16*16*16];

BGBDT_VoxRenderEntity *ent;		//entity
BGBDT_VoxRenderEntity *visent;	//visible entity

BGBDT_VoxChunkMesh *pvs;	//chunk PVS
BGBDT_VoxChunkMesh *cvs;	//chunk CVS
int lastpvs;
int lastcvs;
int lasttick;

short pvsrov;

int pvschk_tot;
int pvschk_tris;
int cvschk_tot;
int cvschk_tris;
int rawchk_tot;
int rawchk_tris;
};

struct BGBDT_VoxWorld_s {
BGBDT_VoxRegion *region;
BGBDT_VoxTypeInfo *voxtypes[4096];
char *worldname;
char *worldtype;

BGBDT_VoxRegion *freergn;
BGBDT_VoxChunk *freechk;
BGBDT_VoxChunkMesh *freemesh;
void *freeidxl;
void *freeidxh;

byte *wrlmap;				//world cell bitmap
byte *wrlbuf;				//world buffer
int szwrlbuf;				//size of region buffer

BGBDT_VoxRegion *lastrgn;
BGBDT_VoxChunk *lastchk;

u32 wrlseed;				//world seed
byte seedperm_x[256];		//seed permutation x
byte seedperm_y[256];		//seed permutation y
byte seedperm_z[256];		//seed permutation z
byte seedperm_w[256];		//seed permutation w

int tickstart;				//start time for tick
int dt_tick;
int dt_pvs;
int dt_draw;
int dt_sound;
int dt_frame;
int dt_phytick;

int meshchk_tot;
int meshchk_nvis;

int vischk_tot;
int vischk_tris;
int pvschk_tot;
int pvschk_tris;
int rawchk_tot;
int rawchk_tris;

byte insky;
byte inwater;

double reforg[3];			//camera reference origin
float camorg[3];			//camera origin
float camrot[9];			//camera rotation

int (*CheckRgnGenChunkP)(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn, int bx, int by, int bz);
int (*GenerateChunk)(BGBDT_VoxWorld *world, BGBDT_VoxChunk *chk);

void *freetmp[256];

dtVal entarr;
int nents;
dtVal ent_player;
};

struct BGBDT_VoxTypeInfo_s {
char *name;
int tidx;
int flags;
int mat_side, mat_side_x, mat_side_y;
int mat_top, mat_top_x, mat_top_y;
int mat_bot, mat_bot_x, mat_bot_y;
int mat_ico, mat_ico_x, mat_ico_y;
int glow;

int (*EmitBlockFaces)(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh, BGBDT_VoxChunk *chk,
	int bx, int by, int bz,
	BGBDT_VoxData td, BGBDT_VoxDataStatus tds);
void (*RandomTick)(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn, BGBDT_VoxCoord xyz,
	BGBDT_VoxData td);
};

typedef struct BGBDT_NoiseSample_s BGBDT_NoiseSample;
typedef struct BGBDT_RiceContext_s BGBDT_RiceContext;

struct BGBDT_NoiseSample_s {
int bh;				//hash for chunk (optimize locally)
u16 lvt[8];			//random numbers for chunk corners
float lvf[8];		//floating point chunk corners

u16 lvs[5*5*5];		//random numbers for chunk corners
float lvg[5*5*5];	//values for 4x4x4 grid

u16 lvt4[8];		//random numbers for chunk corners
float lvf4[8];		//floating point chunk corners

u16 lvt16[8];		//random numbers for chunk corners
float lvf16[8];		//floating point chunk corners
};

struct BGBDT_RiceContext_s {
u32 mark1;

BGBDT_RiceContext *next;			//next context

byte *bs_ct;						//bitstream output
byte *bs_cts;						//bitstream output start
byte *bs_cte;						//bitstream output end

byte *bs_cs;						//bitstream input
byte *bs_cse;						//bitstream input end

u32 bs_win;							//bitstream window
int bs_pos;							//bitstream window position
int bs_bits;						//bitstream total bits

byte *blks;							//blocks buffer
byte *lblks;						//last-frame blocks buffer

byte cmdwin[256];					//command window
short cmdidx[256];					//command window index
byte cmdwpos;						//command window position

u32 mark2;

byte lzwin[256];					//command window
byte lzidx[256];					//command window index
byte lzwpos;						//command window position

// byte *lzhash[256];					//LZ hash
// byte *lzhash[2048];					//LZ hash
byte *lzhash[4096];					//LZ hash

int lzctrl;

int (*ReadAdRiceLL)(BGBDT_RiceContext *ctx, int *rk);
int (*ReadAdDist)(BGBDT_RiceContext *ctx, int *rk);
int (*ReadSym)(BGBDT_RiceContext *ctx, int *rk);

void (*WriteAdRiceLL)(BGBDT_RiceContext *ctx, int val, int *rk);
void (*WriteAdDist)(BGBDT_RiceContext *ctx, int val, int *rk);
void (*WriteSym)(BGBDT_RiceContext *ctx, int sym, int *rk);

};


typedef struct BGBDT_Particle_s BGBDT_Particle;
typedef struct BGBDT_ParticleEmission_s BGBDT_ParticleEmission;

struct BGBDT_Particle_s {
BGBDT_Particle *next;
// vec3d org;		//origin
// vec3 vel;		//velocity
double org[3];
float vel[3];
float lgvel[3];
byte atxy;		//atlas X/Y
byte zacc;		//Z accel (Q4.4 m/s^2)
byte rad;		//"radius" (in 1mm units).
u32 clr;		//color
u32 clrvel;		//color velocity
short ttl;		//time-to-live in milliseconds
};

struct BGBDT_ParticleEmission_s {
vec3d org;		//origin
vec3 bvel;		//base velocity
vec3 rvel;		//radial velocity (spherical)
int cnt;		//number of particles to emit
byte atxy;		//atlas position
byte zacc;		//z acceleration
u32 clr;		//color
u32 clrvel;		//color velocity
short rad;		//particle radius
short ttl;		//time-to-live in milliseconds
};

