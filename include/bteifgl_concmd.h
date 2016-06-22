#define CON_MAX_XS		256
#define CON_MAX_YS		192
#define CON_MAX_HIST	16

#define CON_EFF_MASK_FG		0x000F		//foreground
#define CON_EFF_MASK_BG		0x00F0		//background

#define CON_EFF_ITALIC		0x0100		//italic
#define CON_EFF_UNDERLINE	0x0200		//underline
#define CON_EFF_BLINK		0x0400		//blink
#define CON_EFF_STRIKEOUT	0x0800		//strikeout
#define CON_EFF_FAINT		0x1000		//faint
#define CON_EFF_BOLD		0x2000		//bold
#define CON_EFF_SUBSUPER	0x4000		//subsuper
#define CON_EFF_CHARESC		0x8000		//character escape

#define CON_FLAG_DIRTY		0x00010000	//console has updates
#define CON_FLAG_BLINKSTATE	0x00020000	//console blink state

typedef struct FRGL_Cvar_s FRGL_Cvar;
typedef struct FRGL_ConCmd_s FRGL_ConCmd;
typedef struct FRGL_ConEdit_s FRGL_ConEdit;

typedef struct FRGL_ConsoleInfo_s FRGL_ConsoleInfo;

typedef struct FRGL_MenuItem_s FRGL_MenuItem;
typedef struct FRGL_MenuInfo_s FRGL_MenuInfo;

struct FRGL_Cvar_s {
FRGL_Cvar *next;
char *name;
char *desc;
char *value;
};

struct FRGL_ConCmd_s {
FRGL_ConCmd *next;
char *name;
char *desc;
int (*Func)(FRGL_ConsoleInfo *con, char **args);
char *(*Complete)(FRGL_ConsoleInfo *con, char **args);
};

struct FRGL_ConEdit_s {
FRGL_ConEdit *next;
char *name;			//module name
char *filename;		//file name

u16 **linebuf;		//editor line buffer
int n_lines;		//number of lines
int m_lines;		//max lines

int win_x;			//edit window X position
int win_y;			//edit window Y position
int cur_x;			//edit cursor X position
int cur_y;			//edit cursor Y position

int sel_sx;			//selection start X
int sel_sy;			//selection start Y
int sel_ex;			//selection end X
int sel_ey;			//selection end Y
// int (*func)(char **args);
int flags;
};

struct FRGL_ConsoleInfo_s {
//short conbuf[75][100];
//short conclr[75][100];
//short promptbuf[100], promptpos;
//short histbuf[16][100], histpos;
//short completebuf[100];

u16 conbuf[CON_MAX_YS][CON_MAX_XS];
u16 conclr[CON_MAX_YS][CON_MAX_XS];
u16 promptbuf[CON_MAX_XS];
u16 histbuf[CON_MAX_HIST][CON_MAX_XS];
u16 completebuf[CON_MAX_XS];
int promptpos;
int histpos;

int curclr;
char *stuffcmd[1024];
int nstuffcmd;

int x, y;		//cursor XY
int xs, ys;		//console size
int chxs, chys;
int flags;

FRGL_ConEdit *edit;		//console edit context
void *vbo_info;			//console VBO context

void (*WriteString)(FRGL_ConsoleInfo *con, char *str);
int (*Update)(FRGL_ConsoleInfo *con, float dt);
int (*HandleKey)(FRGL_ConsoleInfo *con, int num, int down);
int (*Clear)(FRGL_ConsoleInfo *con);
int (*Render)(FRGL_ConsoleInfo *con);
int (*KeyDown)(FRGL_ConsoleInfo *con, int key);

int (*OldHandleKey)(FRGL_ConsoleInfo *con, int num, int down);

int (*CmdHandler)(FRGL_ConsoleInfo *con, char *s);
char *(*CmdComplete)(FRGL_ConsoleInfo *con, char *s);
};


struct FRGL_MenuItem_s {
char *name;
char *title;
char *opt_str;
void (*Action)(FRGL_MenuInfo *menu, FRGL_MenuItem *mitm);
};

struct FRGL_MenuInfo_s {
FRGL_MenuInfo *next;
char *name;
char *title;
char *back;
FRGL_MenuItem *item[64];
int n_item;
};
