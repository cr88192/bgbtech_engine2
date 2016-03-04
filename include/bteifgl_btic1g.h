typedef struct {
byte *framebuf;
byte *blkbuf;
byte *lblkbuf;
int xs, ys, qf;
int nBlocks;		//num blocks in image

int msgStartBlock;	//message starting block
int msgEndBlock;

int strmId;			//stream ID
int msgFrame;		//message frame number
int msgCurFrame;	//message current frame number

int texId;
byte clean;
}BTIC1G_DecoderContext;

