typedef struct BGBDT_SkelInfo_s BGBDT_SkelInfo;
typedef struct BGBDT_SkelBone_s BGBDT_SkelBone;
typedef struct BGBDT_SkelAnim_s BGBDT_SkelAnim;
typedef struct BGBDT_SkelAnimPose_s BGBDT_SkelAnimPose;
typedef struct BGBDT_SkelInstance_s BGBDT_SkelInstance;


struct BGBDT_SkelInfo_s {
BGBDT_SkelBone **bone;
BGBDT_SkelAnim **anim;
short n_bone;
short n_anim;

vec3 origin;
float scale;

BGBDT_SkelBone *t_bone[32];

};

struct BGBDT_SkelBone_s {
char *name;		//name of bone
vec3 absorg;	//absolute origin (base skeleton)
vec3 relorg;	//relative origin (base skeleton)
short id;		//ID for bone
short parid;	//ID for parent
};

struct BGBDT_SkelAnim_s {
char *name;
short n_frame;
short n_pbone;
BGBDT_SkelAnimPose **pose;
byte *pbone;
byte t_pbone[16];
};

struct BGBDT_SkelAnimPose_s {
vec3 org;
quat rot;
byte flag;
};

struct BGBDT_SkelInstance_s {
BGBDT_SkelInfo *skel;
BGBDT_SkelAnimPose *pose;
int anim;
float frame;
};
