typedef struct FRGL_TextureMaterial_s FRGL_TextureMaterial;

struct FRGL_TextureMaterial_s {
FRGL_TextureMaterial *hnext;

FRGL_AVI_Context *vidctx;
double vidtime;

int matid;

char *name;
char *base_img;
char *alt_img;
char *norm_img;
char *far_img;
char *shader;
char *vidmap;

int basetex;		//sole image, or low-res base-texture/thumbnail
int alt_tex;		//higher resolution alternate image
int norm_tex;		//normal map
int far_tex;		//far image

int use_pgm;

int spr_tex[12];
};
