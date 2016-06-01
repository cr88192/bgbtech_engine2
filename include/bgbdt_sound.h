#define BGBDT_SND_FOURCC(a, b, c, d)	(((d)<<24)|((c)<<16)|((b)<<8)|(a))

#define BGBDT_SNDFCC_RIFF	BGBDT_SND_FOURCC('R', 'I', 'F', 'F')
#define BGBDT_SNDFCC_WAVE	BGBDT_SND_FOURCC('W', 'A', 'V', 'E')
#define BGBDT_SNDFCC_fmt	BGBDT_SND_FOURCC('f', 'm', 't', ' ')
#define BGBDT_SNDFCC_data	BGBDT_SND_FOURCC('d', 'a', 't', 'a')

#define BGBDT_SNDTY_PCM			0x0001
#define BGBDT_SNDTY_IMAADPCM	0x0011

#define BGBDT_SNDATT_NONE		0x0000
#define BGBDT_SNDATT_LINEAR		0x0001	//(1/x)
#define BGBDT_SNDATT_SQRT		0x0002	//(1/sqrt(x))
#define BGBDT_SNDATT_POW075		0x0003	//(1/pow(x, 0.75))

#define BGBDT_SNDFL_LOOP		0x0001	//loops continuously
#define BGBDT_SNDFL_KEEPDONE	0x0002	//keep channel after done

typedef struct BGBDT_SndSampler_s BGBDT_SndSampler;
typedef struct BGBDT_SndMixChan_s BGBDT_SndMixChan;
typedef struct BGBDT_SndListen_s BGBDT_SndListen;

struct BGBDT_SndSampler_s {
BGBDT_SndSampler *next;			//next sample
char *name;						//sample pathname
byte *data;						//raw audio data
int szdata;						//size of audio data

u16 wf_type;
u16 wf_chan;
int wf_rate;
int wf_bytesec;
u16 wf_blkalign;
u16 wf_bits;

u32 nhash;						//name hash
u32 rcpsbsz;
int len;						//length of sampler (samples)
int sblksz;						//sample block size (samples)
int cblg2;						//cache block log2
int sbrate;						//logical block sample rate

/** Get mono sample value at a specific point
  * pos: Sample number at the based frequency of the sampler.
  */
int (*GetSampleMono)(BGBDT_SndSampler *samp, int pos);
int (*GetSampleMonoMod)(BGBDT_SndSampler *samp, int pos);

/** Get sample at a point relative to a target frequency
  * Clz: Clamp endpoints to Zero
  * Mod: Treat sampler as Modular (Wraps at ends)
  * fq: Logical sample rate (Q28.4)
  * pos: Logical sample at this rate (Q28.4)
  */
int (*GetSampleMonoFqClz)(BGBDT_SndSampler *samp, int fq, int pos);
int (*GetSampleMonoFqMod)(BGBDT_SndSampler *samp, int fq, int pos);
};

struct BGBDT_SndMixChan_s {
BGBDT_SndMixChan *next;
BGBDT_SndSampler *samp;
int id;
u32 donemsk;			//done mask
double stime;			//starting time
double otime;			//origin time
double lotime;			//last origin time
vec3 org;				//origin
vec3 lorg;				//last origin
vec3 vel;				//velocity
s16 vol;				//base volume (0..256)
u16 flag;				//flags
byte att;				//attenuation type
};

struct BGBDT_SndListen_s {
BGBDT_SndListen *next;
int id;

s32 *mixbuf;
int szmixbuf;
int rate;
double ctime;			//current time
double otime;			//origin time
double lotime;			//last origin time
vec3 org;				//origin
vec3 lorg;				//last origin
vec3 vel;				//velocity

vec3 vld;				//listener direction
vec3 vrt;				//vector right
vec3 vfw;				//vector forward
vec3 vup;				//vector up
};
