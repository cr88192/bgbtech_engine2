/*
Isometric Test Stuff
 */

typedef struct BGBDT_IsoTile_s BGBDT_IsoTile;
typedef struct BGBDT_IsoMap_s BGBDT_IsoMap;

struct BGBDT_IsoTile_s {
char *tex_floor;
char *tex_wall_xz;
char *tex_wall_yz;
int x, y;
};

struct BGBDT_IsoMap_s {
int xs, ys;
BGBDT_IsoTile **tile;

dtVal entarr;
int nents;
};
