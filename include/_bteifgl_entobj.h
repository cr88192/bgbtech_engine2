/*
Object points to layout;

Ca: Vec2 fixed Q20.12
Cb: Vec3 fixed Q20.12
Cc: Vec4 fixed Q20.12

Co: ObjRef
Cq: ArrayRef
*/

#define BGBDT_ENTBTY_INVALID	0
#define BGBDT_ENTBTY_INT		1
#define BGBDT_ENTBTY_FLOAT		2
#define BGBDT_ENTBTY_STRING		3
#define BGBDT_ENTBTY_BOOL		4
#define BGBDT_ENTBTY_OBJREF		5
#define BGBDT_ENTBTY_ARRAYREF	6
#define BGBDT_ENTBTY_FXV3		7

typedef struct BGBDT_EntTagType_s		BGBDT_EntTagType;

typedef struct BGBDT_EntField_s		BGBDT_EntField;
typedef struct BGBDT_EntClass_s		BGBDT_EntClass;
typedef struct BGBDT_EntObj_s			BGBDT_EntObj;
typedef struct BGBDT_EntArray_s		BGBDT_EntArray;
typedef struct BGBDT_EntValue_s		BGBDT_EntValue;

struct BGBDT_EntTagType_s {
char *name;
int tyid;
};

struct BGBDT_EntField_s {
char *name;
char *sig;
int type;
};

struct BGBDT_EntClass_s {
BGBDT_EntClass *next;
BGBDT_EntField **field;
int *field_ofs;
int n_field;
int sz_data;
};

struct BGBDT_EntObj_s {
BGBDT_EntClass *clinf;
byte *data;
};

struct BGBDT_EntArray_s {
char *sig;
int size;
byte *data;
};

struct BGBDT_EntValue_s {
byte tag;
union {
	s64 li;
	double lf;
	char *ls;
	int fxv[4];
	void *ptr;
};
};
