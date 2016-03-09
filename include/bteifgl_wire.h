#define BTEWS_WFL_HORZ		0x0001	//can connect horizontally
#define BTEWS_WFL_VERT		0x0002	//can connect vertically
#define BTEWS_WFL_JOIN		0x0004	//connections join in center
#define BTEWS_WFL_OUT		0x0008	//output only

#define BTEWS_WFL_CN_LF		0x0010	//connect left
#define BTEWS_WFL_CN_RT		0x0020	//connect right
#define BTEWS_WFL_CN_UP		0x0040	//connect up
#define BTEWS_WFL_CN_DN		0x0080	//connect down

#define BTEWS_WFL_CN_ALL	0x00F0	//connect all

#define BTEWS_WFL_NOCN_LF	0x0100	//don't connect left
#define BTEWS_WFL_NOCN_RT	0x0200	//don't connect right
#define BTEWS_WFL_NOCN_UP	0x0400	//don't connect up
#define BTEWS_WFL_NOCN_DN	0x0800	//don't connect down

#define BTEWS_WFL_NOCN_ALL	0x0F00	//don't connect all

#define BTEWS_WFL_SDIR_LF	0x1000	//source left
#define BTEWS_WFL_SDIR_RT	0x2000	//source right
#define BTEWS_WFL_SDIR_UP	0x4000	//source up
#define BTEWS_WFL_SDIR_DN	0x8000	//source down
#define BTEWS_WFL_SDIR_ALL	0xF000	//source all
#define BTEWS_WFL_SDIR_LFRT	0x3000	//source left/right
#define BTEWS_WFL_SDIR_UPDN	0xC000	//source up/down

#define BTEWS_WFL_DGTL_ALL		0x000F0000	//digital mode (X)
#define BTEWS_WFL_DGTL_NO		0x00000000	//unknown/analog
#define BTEWS_WFL_DGTL_IND		0x00010000	//(I) indeterminate signal
#define BTEWS_WFL_DGTL_VCC		0x00020000	//(V) supply voltage
#define BTEWS_WFL_DGTL_VGND		0x00030000	//(G) ground
#define BTEWS_WFL_DGTL_PULLUP	0x00040000	//(U) pulled up (resistor)
#define BTEWS_WFL_DGTL_PULLDN	0x00050000	//(D) pulled down (resistor)
#define BTEWS_WFL_DGTL_PINHI	0x00060000	//(H) digital pin high
#define BTEWS_WFL_DGTL_PINLO	0x00070000	//(L) digital pin low

#define BTEWS_WFL_DGTL2_ALL		0x00F00000	//digital mode 2 (Y)

#define BTEWS_WFL_DGTLX_ALL		0x000F0000	//digital mode (X)
#define BTEWS_WFL_DGTLY_ALL		0x00F00000	//digital mode 2 (Y)

#define BTEWS_WFL_DGTLX_SHR		16			//digital mode (X)
#define BTEWS_WFL_DGTLY_SHR		20			//digital mode 2 (Y)


#define BTEWS_WFL_CN_UPDN	(BTEWS_WFL_CN_UP|BTEWS_WFL_CN_DN)
#define BTEWS_WFL_CN_LFRT	(BTEWS_WFL_CN_LF|BTEWS_WFL_CN_RT)

#define BTEWS_WFL_CN_UPLF	(BTEWS_WFL_CN_UP|BTEWS_WFL_CN_LF)
#define BTEWS_WFL_CN_UPRT	(BTEWS_WFL_CN_UP|BTEWS_WFL_CN_RT)
#define BTEWS_WFL_CN_DNLF	(BTEWS_WFL_CN_DN|BTEWS_WFL_CN_LF)
#define BTEWS_WFL_CN_DNRT	(BTEWS_WFL_CN_DN|BTEWS_WFL_CN_RT)
#define BTEWS_WFL_CN_UPLRT	(BTEWS_WFL_CN_UP|BTEWS_WFL_CN_LF|BTEWS_WFL_CN_RT)
#define BTEWS_WFL_CN_DNLRT	(BTEWS_WFL_CN_DN|BTEWS_WFL_CN_LF|BTEWS_WFL_CN_RT)

#define BTEWS_WFL_CN_UDNLF	(BTEWS_WFL_CN_UP|BTEWS_WFL_CN_DN|BTEWS_WFL_CN_LF)
#define BTEWS_WFL_CN_UDNRT	(BTEWS_WFL_CN_UP|BTEWS_WFL_CN_DN|BTEWS_WFL_CN_RT)

#define BTEWS_WFL_SDIR_UPDN	0xC000	//source up/down
#define BTEWS_WFL_SDIR_LFRT	0x3000	//source left/right

#define BTEWS_MXY_UP_HALF	1		//left half
#define BTEWS_MXY_DN_HALF	2		//bottom half
#define BTEWS_MXY_LF_HALF	4		//left half
#define BTEWS_MXY_RT_HALF	8		//right half

#define BTEWS_MXY_UP_LF	16			//top left
#define BTEWS_MXY_UP_MI	32			//top middle
#define BTEWS_MXY_UP_RT	64			//top right
#define BTEWS_MXY_MI_LF	128			//middle left
#define BTEWS_MXY_MI_MI	256			//middle
#define BTEWS_MXY_MI_RT	512			//middle right
#define BTEWS_MXY_DN_LF	1024		//bottom left
#define BTEWS_MXY_DN_MI	2048		//bottom middle
#define BTEWS_MXY_DN_RT	4096		//bottom right

#define BTEWS_CTY_UNKNOWN	0		//unknown
#define BTEWS_CTY_PNP		1		//PNP transistor
#define BTEWS_CTY_NPN		2		//NPN transistor
#define BTEWS_CTY_PNPDAR	3		//PNP Darlington transistor
#define BTEWS_CTY_NPNDAR	4		//NPN Darlington transistor
#define BTEWS_CTY_NMOS		5		//N-MOSFET
#define BTEWS_CTY_PMOS		6		//P_MOSFET

#define BTEWS_CTY_WIRE		16		//analog wire
#define BTEWS_CTY_DGTWIRE	17		//digital wire


#define BTEWS_WIREOHM	0.01


typedef struct BTEWS_WireGrid_s BTEWS_WireGrid;
typedef struct BTEWS_WireNode_s BTEWS_WireNode;
typedef struct BTEWS_Component_s BTEWS_Component;

typedef struct BTEWS_NetWire_s BTEWS_NetWire;
typedef struct BTEWS_NetComponent_s BTEWS_NetComponent;

struct BTEWS_WireGrid_s {
int xs, ys;						//grid size
int mgx, mgy;					//mouse grid offset
double sim_ct;					//simulation current time
double sim_accdt;				//simulated accumulated delta time
double sim_tf;					//simulation time fraction (of real-time)
double sim_tfs;				//simulation time fraction (for step)
BTEWS_WireNode **node;			//node grid
BTEWS_WireNode *live;			//ready to execute
BTEWS_WireNode *liveend;		//ready to execute (list end)

BTEWS_NetWire *netwire;			//netlist wire
BTEWS_NetComponent *netcmp;		//netlist component

BTEWS_NetWire *freenetwire;		//free netlist wire
BTEWS_NetComponent *freenetcmp;	//free netlist component

void (*DrawDefault)(BTEWS_WireGrid *grid, int x, int y,
	float x0, float y0, float x1, float y1);
};

struct BTEWS_WireNode_s {
BTEWS_WireGrid *grid;			//owning grid
BTEWS_WireNode *next;			//next in list
BTEWS_WireNode *lnext;			//next in live list
double vnx, vpx, vny, vpy;		//voltage on each side
double ovnx, ovpx, ovny, ovpy;	//old voltage on each side
double inx, ipx, iny, ipy;		//current at each side
double pwnx, pwpx, pwny, pwpy;	//watt potentials on each side
double vc0, vc1, vc2, vc3;		//node specific voltages
double vcc, icc, watt;			//volts, amps and watts (center).
double vcc1, icc1, watt1;		//volts, amps and watts (sub-1)
double vcc2, icc2, watt2;		//volts, amps and watts (sub-2)
double pwatt1, pwatt2;
double temp, pwatt;			//temperature
double ohm, farad, henry;		//resistance, capacitance, inductance
short x, y;						//XY coords in grid
short cx, cy;					//center XY for component
int wfl;						//wire flags
int owfl;						//old wire flags
byte cc_x, cc_y, cc_r;			//current cell XY and rotate
byte cty;						//component type
u16 dsttl;						//digital signal time-to-live

int widx;						//Wire ID (X)
int widy;						//Wire ID (Y)

byte pin;						//pin number
u32 io_pdir;					//IO pin direction
u32 io_ipin;					//IO pin input
u32 io_opin;					//IO pin output

byte pin_v0;					//Pin V0 (Vcc1)
byte pin_v1;					//Pin V1 (Vcc2)
byte pin_v2;					//Pin V2 (Vcc3)
byte pin_v3;					//Pin V3 (Vgnd)

void (*Draw)(BTEWS_WireNode *self,
	float x0, float y0, float x1, float y1);
void (*Update)(BTEWS_WireNode *self, double dt);
char *(*Describe)(BTEWS_WireNode *self);
};

struct BTEWS_Component_s {
BTEWS_Component *next;
char *name;
char *image;
int cty;

void (*Place)(BTEWS_Component *self,
	BTEWS_WireGrid *grid, int x, int y);
};


struct BTEWS_NetWire_s {
BTEWS_NetWire *next;
int wid;
double vcc;					//wire voltage
double icc;					//wire current
short cwid[4];					//connection WID
};

struct BTEWS_NetComponent_s {
BTEWS_NetComponent *next;		//next component in netlist
BTEWS_NetComponent *cnext;		//next component in update chain
BTEWS_NetComponent *tnext;		//next component in think chain
int cid;						//component ID
int cty;						//component type
short pwid[64];					//pin wire ID
void (*Update)(BTEWS_WireNode *self);			//inputs have changed
void (*Think)(BTEWS_WireNode *self, double dt);		//time has elapsed
};
