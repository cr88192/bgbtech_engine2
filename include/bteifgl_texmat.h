typedef struct FRGL_TextureMaterial_s FRGL_TextureMaterial;

struct FRGL_TextureMaterial_s {
FRGL_TextureMaterial *hnext;
int matid;

char *name;
char *base_img;
char *alt_img;
char *norm_img;

int basetex;		//sole image, or low-res base-texture/thumbnail
int alt_tex;		//higher resolution alternate image
int norm_tex;		//normal map

int use_pgm;
};
