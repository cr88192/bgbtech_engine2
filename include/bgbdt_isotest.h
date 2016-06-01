/*
Isometric Test Stuff
 */

#define BGBDT_ISOTFL_NX			0x0001
#define BGBDT_ISOTFL_PX			0x0002
#define BGBDT_ISOTFL_NY			0x0004
#define BGBDT_ISOTFL_PY			0x0008
#define BGBDT_ISOTFL_NONSOLID	0x0010

#define BGBDT_ISOTFL_PNX		0x0003
#define BGBDT_ISOTFL_PNY		0x000C

typedef struct BGBDT_IsoTile_s BGBDT_IsoTile;
typedef struct BGBDT_IsoMap_s BGBDT_IsoMap;

struct BGBDT_IsoTile_s {
char *tex_floor;
char *tex_wall_xz;
char *tex_wall_yz;
int x, y;
int fl;
};

struct BGBDT_IsoMap_s {
int xs, ys;
BGBDT_IsoTile **tile;

dtVal entarr;
int nents;
};
