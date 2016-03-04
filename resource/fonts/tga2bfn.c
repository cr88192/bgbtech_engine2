#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;

typedef struct TargaHeader_s {
	unsigned char 	id_length, ctype, itype;
	unsigned short	cindex, clength;
	unsigned char	csize;
	unsigned short	x_origin, y_origin, width, height;
	unsigned char	bpp, attributes;
} TargaHeader;

byte *BGBBTJ_Img_LoadTGA(FILE *fd, int *w, int *h)
{
	TargaHeader head;
	int pixlin, i, j;
	byte *dest, *src, *buf, *obuf, r, g, b, a;

	head.id_length=fgetc(fd);
	head.ctype=fgetc(fd);
	head.itype=fgetc(fd);
	head.cindex=fgetc(fd)+(fgetc(fd)<<8);
	head.clength=fgetc(fd)+(fgetc(fd)<<8);
	head.csize=fgetc(fd);
	head.x_origin=fgetc(fd)+(fgetc(fd)<<8);
	head.y_origin=fgetc(fd)+(fgetc(fd)<<8);
	head.width=fgetc(fd)+(fgetc(fd)<<8);
	head.height=fgetc(fd)+(fgetc(fd)<<8);
	head.bpp=fgetc(fd);
	head.attributes=fgetc(fd);

	if(((head.itype!=2) && (head.itype!=10)) ||
			((head.bpp!=32) && (head.bpp!=24)) || (head.ctype!=0))
	{
		printf("Img_LoadTGA: only RGB(24)/RGBA(32) uncompressed "
			"images supported.\n");
		printf("   bpp: %d, type: %d\n", head.bpp, head.ctype);
		return(NULL);
	}

	fseek(fd, head.id_length, 1); /* skip comment */

	pixlin=head.width*head.height;
	obuf=malloc(pixlin*4);
	dest=obuf;

	buf=malloc(2*pixlin*(head.bpp>>3));
	fread(buf, 1, 2*pixlin*(head.bpp>>3), fd);
	src=buf;

	if(head.itype==2)
	{
		for(i=0; i<pixlin; i++)
		{
			b=*src++;
			g=*src++;
			r=*src++;
			if(head.bpp==32)
				a=*src++;
				else a=255;
			*dest++=r;
			*dest++=g;
			*dest++=b;
			*dest++=a;
		}
	}

	if(head.itype==10)
	{
		i=0;
		while(i<pixlin)
		{
			j=*src++;
			if(j<128)
			{
				j++;
				while(j)
				{
					b=*src++;
					g=*src++;
					r=*src++;
					if(head.bpp==32)
						a=*src++;
						else a=255;
					*dest++=r;
					*dest++=g;
					*dest++=b;
					*dest++=a;

					j--;
					i++;
				}
			}else
			{
				b=*src++;
				g=*src++;
				r=*src++;
				if(head.bpp==32)
					a=*src++;
					else a=255;

				j-=127;
				while(j)
				{
					*dest++=r;
					*dest++=g;
					*dest++=b;
					*dest++=a;

					j--;
					i++;
				}
			}
		}
	}

	free(buf);

	if(head.attributes&32)
	{
		dest=malloc(pixlin*4);
		for(i=0; i<head.height; i++)
			memcpy(&dest[i*head.width*4],
				&obuf[(head.height-(i+1))*head.width*4],
				head.width*4);
		memcpy(obuf, dest, pixlin*4);
		free(dest);
	}

	*w=head.width;
	*h=head.height;
	return(obuf);
}

int BGBBTJ_Img_StoreTGA(FILE *fd, byte *buf, int w, int h)
{
	TargaHeader head;
	int i;
	byte *s;

	head.id_length=0;
	head.ctype=0;
	head.itype=2;
	head.cindex=0;
	head.clength=0;
	head.csize=0;
	head.x_origin=0;
	head.y_origin=0;
	head.width=w;
	head.height=h;
	head.bpp=24;
	head.attributes=0;

	fputc(head.id_length, fd);
	fputc(head.ctype, fd);
	fputc(head.itype, fd);

	fputc(head.cindex, fd);
	fputc(head.cindex>>8, fd);
	fputc(head.clength, fd);
	fputc(head.clength>>8, fd);

	fputc(head.csize, fd);

	fputc(head.x_origin, fd);
	fputc(head.x_origin>>8, fd);
	fputc(head.y_origin, fd);
	fputc(head.y_origin>>8, fd);

	fputc(head.width, fd);
	fputc(head.width>>8, fd);
	fputc(head.height, fd);
	fputc(head.height>>8, fd);

	fputc(head.bpp, fd);
	fputc(head.attributes, fd);

//	vfwrite(buf, w*4, h, fd);

	s=buf;
	for(i=0; i<w*h; i++)
	{
		fputc(s[2], fd);
		fputc(s[1], fd);
		fputc(s[0], fd);
		s+=4;
	}

	return(0);
}

byte *BGBBTJ_TGA_LoadRaw(char *name, int *w, int *h)
{
	FILE *fd;
	byte *tmp;

	fd=fopen(name, "rb");
	if(!fd)
	{
		printf("PCX_LoadRaw: can't open '%s'\n", name);
		return(NULL);
	}

	tmp=BGBBTJ_Img_LoadTGA(fd, w, h);
	fclose(fd);

	return(tmp);
}

static int cp437[256]={
/* 0x00*/ 0x0000,	//NULL

/* 0x01*/ 0x263A,	//01	01,	//	WHITE SMILING FACE
/* 0x02*/ 0x263B,	//02	--,	//	BLACK SMILING FACE
/* 0x03*/ 0x2665,	//03	--,	//	BLACK HEART SUIT
/* 0x04*/ 0x2666,	//04	--,	//	BLACK DIAMOND SUIT
/* 0x05*/ 0x2663,	//05	--,	//	BLACK CLUB SUIT
/* 0x06*/ 0x2660,	//06	--,	//	BLACK SPADE SUIT
/* 0x07*/ 0x2022,	//07	--,	//	BULLET
/* 0x08*/ 0x25D8,	//08	--,	//	INVERSE BULLET
/* 0x09*/ 0x25CB,	//09	--,	//	WHITE CIRCLE
/* 0x0A*/ 0x25D9,	//0A	--,	//	INVERSE WHITE CIRCLE
/* 0x0B*/ 0x2642,	//0B	--,	//	MALE SIGN
/* 0x0C*/ 0x2640,	//0C	--,	//	FEMALE SIGN
/* 0x0D*/ 0x266A,	//0D	02,	//	EIGHTH NOTE
/* 0x0E*/ 0x266B,	//0E	03,	//	BEAMED EIGHTH NOTES
/* 0x0F*/ 0x263C,	//0F	04,	//	WHITE SUN WITH RAYS

/* 0x10*/ 0x25BA,	//10	10,	//	BLACK RIGHT-POINTING POINTER
/* 0x11*/ 0x25C4,	//11	11,	//	BLACK LEFT-POINTING POINTER
/* 0x12*/ 0x2195,	//12	12,	//	UP DOWN ARROW
/* 0x13*/ 0x203C,	//13	13,	//	DOUBLE EXCLAMATION MARK
/* 0x14*/ 0x00B6,	//14	14,	//	PILCROW SIGN
/* 0x15*/ 0x00A7,	//15	15,	//	SECTION SIGN
/* 0x16*/ 0x25AC,	//16	16,	//	BLACK RECTANGLE
/* 0x17*/ 0x21A8,	//17	17,	//	UP DOWN ARROW WITH BASE
/* 0x18*/ 0x2191,	//18	18,	//	UPWARDS ARROW
/* 0x19*/ 0x2193,	//19	19,	//	DOWNWARDS ARROW
/* 0x1A*/ 0x2192,	//1A	1A,	//	RIGHTWARDS ARROW
/* 0x1B*/ 0x2190,	//1B	1B,	//	LEFTWARDS ARROW
/* 0x1C*/ 0x221F,	//1C	1C,	//	RIGHT ANGLE
/* 0x1D*/ 0x2194,	//1D	1D,	//	LEFT RIGHT ARROW
/* 0x1E*/ 0x25B2,	//1E	1E,	//	BLACK UP-POINTING TRIANGLE
/* 0x1F*/ 0x25BC,	//1F	1F,	//	BLACK DOWN-POINTING TRIANGLE

/* 0x20 */ 0x0020,	//SPACE
/* 0x21 */ 0x0021,	//EXCLAMATION MARK
/* 0x22 */ 0x0022,	//QUOTATION MARK
/* 0x23 */ 0x0023,	//NUMBER SIGN
/* 0x24 */ 0x0024,	//DOLLAR SIGN
/* 0x25 */ 0x0025,	//PERCENT SIGN
/* 0x26 */ 0x0026,	//AMPERSAND
/* 0x27 */ 0x0027,	//APOSTROPHE
/* 0x28 */ 0x0028,	//LEFT PARENTHESIS
/* 0x29 */ 0x0029,	//RIGHT PARENTHESIS
/* 0x2A */ 0x002A,	//ASTERISK
/* 0x2B */ 0x002B,	//PLUS SIGN
/* 0x2C */ 0x002C,	//COMMA
/* 0x2D */ 0x002D,	//HYPHEN-MINUS
/* 0x2E */ 0x002E,	//FULL STOP
/* 0x2F */ 0x002F,	//SOLIDUS
/* 0x30 */ 0x0030,	//DIGIT ZERO
/* 0x31 */ 0x0031,	//DIGIT ONE
/* 0x32 */ 0x0032,	//DIGIT TWO
/* 0x33 */ 0x0033,	//DIGIT THREE
/* 0x34 */ 0x0034,	//DIGIT FOUR
/* 0x35 */ 0x0035,	//DIGIT FIVE
/* 0x36 */ 0x0036,	//DIGIT SIX
/* 0x37 */ 0x0037,	//DIGIT SEVEN
/* 0x38 */ 0x0038,	//DIGIT EIGHT
/* 0x39 */ 0x0039,	//DIGIT NINE
/* 0x3A */ 0x003A,	//COLON
/* 0x3B */ 0x003B,	//SEMICOLON
/* 0x3C */ 0x003C,	//LESS-THAN SIGN
/* 0x3D */ 0x003D,	//EQUALS SIGN
/* 0x3E */ 0x003E,	//GREATER-THAN SIGN
/* 0x3F */ 0x003F,	//QUESTION MARK
/* 0x40 */ 0x0040,	//COMMERCIAL AT
/* 0x41 */ 0x0041,	//LATIN CAPITAL LETTER A
/* 0x42 */ 0x0042,	//LATIN CAPITAL LETTER B
/* 0x43 */ 0x0043,	//LATIN CAPITAL LETTER C
/* 0x44 */ 0x0044,	//LATIN CAPITAL LETTER D
/* 0x45 */ 0x0045,	//LATIN CAPITAL LETTER E
/* 0x46 */ 0x0046,	//LATIN CAPITAL LETTER F
/* 0x47 */ 0x0047,	//LATIN CAPITAL LETTER G
/* 0x48 */ 0x0048,	//LATIN CAPITAL LETTER H
/* 0x49 */ 0x0049,	//LATIN CAPITAL LETTER I
/* 0x4A */ 0x004A,	//LATIN CAPITAL LETTER J
/* 0x4B */ 0x004B,	//LATIN CAPITAL LETTER K
/* 0x4C */ 0x004C,	//LATIN CAPITAL LETTER L
/* 0x4D */ 0x004D,	//LATIN CAPITAL LETTER M
/* 0x4E */ 0x004E,	//LATIN CAPITAL LETTER N
/* 0x4F */ 0x004F,	//LATIN CAPITAL LETTER O
/* 0x50 */ 0x0050,	//LATIN CAPITAL LETTER P
/* 0x51 */ 0x0051,	//LATIN CAPITAL LETTER Q
/* 0x52 */ 0x0052,	//LATIN CAPITAL LETTER R
/* 0x53 */ 0x0053,	//LATIN CAPITAL LETTER S
/* 0x54 */ 0x0054,	//LATIN CAPITAL LETTER T
/* 0x55 */ 0x0055,	//LATIN CAPITAL LETTER U
/* 0x56 */ 0x0056,	//LATIN CAPITAL LETTER V
/* 0x57 */ 0x0057,	//LATIN CAPITAL LETTER W
/* 0x58 */ 0x0058,	//LATIN CAPITAL LETTER X
/* 0x59 */ 0x0059,	//LATIN CAPITAL LETTER Y
/* 0x5A */ 0x005A,	//LATIN CAPITAL LETTER Z
/* 0x5B */ 0x005B,	//LEFT SQUARE BRACKET
/* 0x5C */ 0x005C,	//REVERSE SOLIDUS
/* 0x5D */ 0x005D,	//RIGHT SQUARE BRACKET
/* 0x5E */ 0x005E,	//CIRCUMFLEX ACCENT
/* 0x5F */ 0x005F,	//LOW LINE
/* 0x60 */ 0x0060,	//GRAVE ACCENT
/* 0x61 */ 0x0061,	//LATIN SMALL LETTER A
/* 0x62 */ 0x0062,	//LATIN SMALL LETTER B
/* 0x63 */ 0x0063,	//LATIN SMALL LETTER C
/* 0x64 */ 0x0064,	//LATIN SMALL LETTER D
/* 0x65 */ 0x0065,	//LATIN SMALL LETTER E
/* 0x66 */ 0x0066,	//LATIN SMALL LETTER F
/* 0x67 */ 0x0067,	//LATIN SMALL LETTER G
/* 0x68 */ 0x0068,	//LATIN SMALL LETTER H
/* 0x69 */ 0x0069,	//LATIN SMALL LETTER I
/* 0x6A */ 0x006A,	//LATIN SMALL LETTER J
/* 0x6B */ 0x006B,	//LATIN SMALL LETTER K
/* 0x6C */ 0x006C,	//LATIN SMALL LETTER L
/* 0x6D */ 0x006D,	//LATIN SMALL LETTER M
/* 0x6E */ 0x006E,	//LATIN SMALL LETTER N
/* 0x6F */ 0x006F,	//LATIN SMALL LETTER O
/* 0x70 */ 0x0070,	//LATIN SMALL LETTER P
/* 0x71 */ 0x0071,	//LATIN SMALL LETTER Q
/* 0x72 */ 0x0072,	//LATIN SMALL LETTER R
/* 0x73 */ 0x0073,	//LATIN SMALL LETTER S
/* 0x74 */ 0x0074,	//LATIN SMALL LETTER T
/* 0x75 */ 0x0075,	//LATIN SMALL LETTER U
/* 0x76 */ 0x0076,	//LATIN SMALL LETTER V
/* 0x77 */ 0x0077,	//LATIN SMALL LETTER W
/* 0x78 */ 0x0078,	//LATIN SMALL LETTER X
/* 0x79 */ 0x0079,	//LATIN SMALL LETTER Y
/* 0x7A */ 0x007A,	//LATIN SMALL LETTER Z
/* 0x7B */ 0x007B,	//LEFT CURLY BRACKET
/* 0x7C */ 0x007C,	//VERTICAL LINE
/* 0x7D */ 0x007D,	//RIGHT CURLY BRACKET
/* 0x7E */ 0x007E,	//TILDE
/* 0x7F */ 0x007F,	//DEL

/* 0x80*/ 0x00c7,	//LATIN CAPITAL LETTER C WITH CEDILLA
/* 0x81*/ 0x00fc,	//LATIN SMALL LETTER U WITH DIAERESIS
/* 0x82*/ 0x00e9,	//LATIN SMALL LETTER E WITH ACUTE
/* 0x83*/ 0x00e2,	//LATIN SMALL LETTER A WITH CIRCUMFLEX
/* 0x84*/ 0x00e4,	//LATIN SMALL LETTER A WITH DIAERESIS
/* 0x85*/ 0x00e0,	//LATIN SMALL LETTER A WITH GRAVE
/* 0x86*/ 0x00e5,	//LATIN SMALL LETTER A WITH RING ABOVE
/* 0x87*/ 0x00e7,	//LATIN SMALL LETTER C WITH CEDILLA
/* 0x88*/ 0x00ea,	//LATIN SMALL LETTER E WITH CIRCUMFLEX
/* 0x89*/ 0x00eb,	//LATIN SMALL LETTER E WITH DIAERESIS
/* 0x8a*/ 0x00e8,	//LATIN SMALL LETTER E WITH GRAVE
/* 0x8b*/ 0x00ef,	//LATIN SMALL LETTER I WITH DIAERESIS
/* 0x8c*/ 0x00ee,	//LATIN SMALL LETTER I WITH CIRCUMFLEX
/* 0x8d*/ 0x00ec,	//LATIN SMALL LETTER I WITH GRAVE
/* 0x8e*/ 0x00c4,	//LATIN CAPITAL LETTER A WITH DIAERESIS
/* 0x8f*/ 0x00c5,	//LATIN CAPITAL LETTER A WITH RING ABOVE
/* 0x90*/ 0x00c9,	//LATIN CAPITAL LETTER E WITH ACUTE
/* 0x91*/ 0x00e6,	//LATIN SMALL LIGATURE AE
/* 0x92*/ 0x00c6,	//LATIN CAPITAL LIGATURE AE
/* 0x93*/ 0x00f4,	//LATIN SMALL LETTER O WITH CIRCUMFLEX
/* 0x94*/ 0x00f6,	//LATIN SMALL LETTER O WITH DIAERESIS
/* 0x95*/ 0x00f2,	//LATIN SMALL LETTER O WITH GRAVE
/* 0x96*/ 0x00fb,	//LATIN SMALL LETTER U WITH CIRCUMFLEX
/* 0x97*/ 0x00f9,	//LATIN SMALL LETTER U WITH GRAVE
/* 0x98*/ 0x00ff,	//LATIN SMALL LETTER Y WITH DIAERESIS
/* 0x99*/ 0x00d6,	//LATIN CAPITAL LETTER O WITH DIAERESIS
/* 0x9a*/ 0x00dc,	//LATIN CAPITAL LETTER U WITH DIAERESIS
/* 0x9b*/ 0x00a2,	//CENT SIGN
/* 0x9c*/ 0x00a3,	//POUND SIGN
/* 0x9d*/ 0x00a5,	//YEN SIGN
/* 0x9e*/ 0x20a7,	//PESETA SIGN
/* 0x9f*/ 0x0192,	//LATIN SMALL LETTER F WITH HOOK
/* 0xa0*/ 0x00e1,	//LATIN SMALL LETTER A WITH ACUTE
/* 0xa1*/ 0x00ed,	//LATIN SMALL LETTER I WITH ACUTE
/* 0xa2*/ 0x00f3,	//LATIN SMALL LETTER O WITH ACUTE
/* 0xa3*/ 0x00fa,	//LATIN SMALL LETTER U WITH ACUTE
/* 0xa4*/ 0x00f1,	//LATIN SMALL LETTER N WITH TILDE
/* 0xa5*/ 0x00d1,	//LATIN CAPITAL LETTER N WITH TILDE
/* 0xa6*/ 0x00aa,	//FEMININE ORDINAL INDICATOR
/* 0xa7*/ 0x00ba,	//MASCULINE ORDINAL INDICATOR
/* 0xa8*/ 0x00bf,	//INVERTED QUESTION MARK
/* 0xa9*/ 0x2310,	//REVERSED NOT SIGN
/* 0xaa*/ 0x00ac,	//NOT SIGN
/* 0xab*/ 0x00bd,	//VULGAR FRACTION ONE HALF
/* 0xac*/ 0x00bc,	//VULGAR FRACTION ONE QUARTER
/* 0xad*/ 0x00a1,	//INVERTED EXCLAMATION MARK
/* 0xae*/ 0x00ab,	//LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
/* 0xaf*/ 0x00bb,	//RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
/* 0xb0*/ 0x2591,	//LIGHT SHADE
/* 0xb1*/ 0x2592,	//MEDIUM SHADE
/* 0xb2*/ 0x2593,	//DARK SHADE
/* 0xb3*/ 0x2502,	//BOX DRAWINGS LIGHT VERTICAL
/* 0xb4*/ 0x2524,	//BOX DRAWINGS LIGHT VERTICAL AND LEFT
/* 0xb5*/ 0x2561,	//BOX DRAWINGS VERTICAL SINGLE AND LEFT DOUBLE
/* 0xb6*/ 0x2562,	//BOX DRAWINGS VERTICAL DOUBLE AND LEFT SINGLE
/* 0xb7*/ 0x2556,	//BOX DRAWINGS DOWN DOUBLE AND LEFT SINGLE
/* 0xb8*/ 0x2555,	//BOX DRAWINGS DOWN SINGLE AND LEFT DOUBLE
/* 0xb9*/ 0x2563,	//BOX DRAWINGS DOUBLE VERTICAL AND LEFT
/* 0xba*/ 0x2551,	//BOX DRAWINGS DOUBLE VERTICAL
/* 0xbb*/ 0x2557,	//BOX DRAWINGS DOUBLE DOWN AND LEFT
/* 0xbc*/ 0x255d,	//BOX DRAWINGS DOUBLE UP AND LEFT
/* 0xbd*/ 0x255c,	//BOX DRAWINGS UP DOUBLE AND LEFT SINGLE
/* 0xbe*/ 0x255b,	//BOX DRAWINGS UP SINGLE AND LEFT DOUBLE
/* 0xbf*/ 0x2510,	//BOX DRAWINGS LIGHT DOWN AND LEFT
/* 0xc0*/ 0x2514,	//BOX DRAWINGS LIGHT UP AND RIGHT
/* 0xc1*/ 0x2534,	//BOX DRAWINGS LIGHT UP AND HORIZONTAL
/* 0xc2*/ 0x252c,	//BOX DRAWINGS LIGHT DOWN AND HORIZONTAL
/* 0xc3*/ 0x251c,	//BOX DRAWINGS LIGHT VERTICAL AND RIGHT
/* 0xc4*/ 0x2500,	//BOX DRAWINGS LIGHT HORIZONTAL
/* 0xc5*/ 0x253c,	//BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL
/* 0xc6*/ 0x255e,	//BOX DRAWINGS VERTICAL SINGLE AND RIGHT DOUBLE
/* 0xc7*/ 0x255f,	//BOX DRAWINGS VERTICAL DOUBLE AND RIGHT SINGLE
/* 0xc8*/ 0x255a,	//BOX DRAWINGS DOUBLE UP AND RIGHT
/* 0xc9*/ 0x2554,	//BOX DRAWINGS DOUBLE DOWN AND RIGHT
/* 0xca*/ 0x2569,	//BOX DRAWINGS DOUBLE UP AND HORIZONTAL
/* 0xcb*/ 0x2566,	//BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL
/* 0xcc*/ 0x2560,	//BOX DRAWINGS DOUBLE VERTICAL AND RIGHT
/* 0xcd*/ 0x2550,	//BOX DRAWINGS DOUBLE HORIZONTAL
/* 0xce*/ 0x256c,	//BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL
/* 0xcf*/ 0x2567,	//BOX DRAWINGS UP SINGLE AND HORIZONTAL DOUBLE
/* 0xd0*/ 0x2568,	//BOX DRAWINGS UP DOUBLE AND HORIZONTAL SINGLE
/* 0xd1*/ 0x2564,	//BOX DRAWINGS DOWN SINGLE AND HORIZONTAL DOUBLE
/* 0xd2*/ 0x2565,	//BOX DRAWINGS DOWN DOUBLE AND HORIZONTAL SINGLE
/* 0xd3*/ 0x2559,	//BOX DRAWINGS UP DOUBLE AND RIGHT SINGLE
/* 0xd4*/ 0x2558,	//BOX DRAWINGS UP SINGLE AND RIGHT DOUBLE
/* 0xd5*/ 0x2552,	//BOX DRAWINGS DOWN SINGLE AND RIGHT DOUBLE
/* 0xd6*/ 0x2553,	//BOX DRAWINGS DOWN DOUBLE AND RIGHT SINGLE
/* 0xd7*/ 0x256b,	//BOX DRAWINGS VERTICAL DOUBLE AND HORIZONTAL SINGLE
/* 0xd8*/ 0x256a,	//BOX DRAWINGS VERTICAL SINGLE AND HORIZONTAL DOUBLE
/* 0xd9*/ 0x2518,	//BOX DRAWINGS LIGHT UP AND LEFT
/* 0xda*/ 0x250c,	//BOX DRAWINGS LIGHT DOWN AND RIGHT
/* 0xdb*/ 0x2588,	//FULL BLOCK
/* 0xdc*/ 0x2584,	//LOWER HALF BLOCK
/* 0xdd*/ 0x258c,	//LEFT HALF BLOCK
/* 0xde*/ 0x2590,	//RIGHT HALF BLOCK
/* 0xdf*/ 0x2580,	//UPPER HALF BLOCK
/* 0xe0*/ 0x03b1,	//GREEK SMALL LETTER ALPHA
/* 0xe1*/ 0x00df,	//LATIN SMALL LETTER SHARP S
/* 0xe2*/ 0x0393,	//GREEK CAPITAL LETTER GAMMA
/* 0xe3*/ 0x03c0,	//GREEK SMALL LETTER PI
/* 0xe4*/ 0x03a3,	//GREEK CAPITAL LETTER SIGMA
/* 0xe5*/ 0x03c3,	//GREEK SMALL LETTER SIGMA
/* 0xe6*/ 0x00b5,	//MICRO SIGN
/* 0xe7*/ 0x03c4,	//GREEK SMALL LETTER TAU
/* 0xe8*/ 0x03a6,	//GREEK CAPITAL LETTER PHI
/* 0xe9*/ 0x0398,	//GREEK CAPITAL LETTER THETA
/* 0xea*/ 0x03a9,	//GREEK CAPITAL LETTER OMEGA
/* 0xeb*/ 0x03b4,	//GREEK SMALL LETTER DELTA
/* 0xec*/ 0x221e,	//INFINITY
/* 0xed*/ 0x03c6,	//GREEK SMALL LETTER PHI
/* 0xee*/ 0x03b5,	//GREEK SMALL LETTER EPSILON
/* 0xef*/ 0x2229,	//INTERSECTION
/* 0xf0*/ 0x2261,	//IDENTICAL TO
/* 0xf1*/ 0x00b1,	//PLUS-MINUS SIGN
/* 0xf2*/ 0x2265,	//GREATER-THAN OR EQUAL TO
/* 0xf3*/ 0x2264,	//LESS-THAN OR EQUAL TO
/* 0xf4*/ 0x2320,	//TOP HALF INTEGRAL
/* 0xf5*/ 0x2321,	//BOTTOM HALF INTEGRAL
/* 0xf6*/ 0x00f7,	//DIVISION SIGN
/* 0xf7*/ 0x2248,	//ALMOST EQUAL TO
/* 0xf8*/ 0x00b0,	//DEGREE SIGN
/* 0xf9*/ 0x2219,	//BULLET OPERATOR
/* 0xfa*/ 0x00b7,	//MIDDLE DOT
/* 0xfb*/ 0x221a,	//SQUARE ROOT
/* 0xfc*/ 0x207f,	//SUPERSCRIPT LATIN SMALL LETTER N
/* 0xfd*/ 0x00b2,	//SUPERSCRIPT TWO
/* 0xfe*/ 0x25a0,	//BLACK SQUARE
/* 0xff*/ 0x00a0,	//NO-BREAK SPACE
};

int main(int argc, char *argv[])
{
	int *remap;
	FILE *ofd;
	byte *cells, *cmask;
	byte *ibuf;
	char *ifn, *ofn;
	int xs, ys, cw, ch, csz;
	int cx, cy, cn, px, py, pn;
	int vy, s, e, isz;
	int i, j;

	remap=malloc(65536*sizeof(int));
	for(i=0; i<65536; i++)remap[i]=i;

	isz=256;
	ifn=NULL; ofn=NULL;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-latin1"))
			{
				isz=16;
			}

			if(!strcmp(argv[i], "-cp437"))
			{
				isz=16;
				
				for(j=0; j<256; j++)
					{ remap[j]=cp437[j]; }
			}
		}else
		{
			if(!ifn)
				{ ifn=argv[i]; }
			else if(!ofn)
				{ ofn=argv[i]; }
		}
	}

//	ibuf=BGBBTJ_TGA_LoadRaw(argv[1], &xs, &ys);
	ibuf=BGBBTJ_TGA_LoadRaw(ifn, &xs, &ys);
	
//	cw=xs/256;
//	ch=ys/256;
	cw=xs/isz;
	ch=ys/isz;
	csz=(cw*ch+7)/8;

	cells=malloc(65536*csz);
	cmask=malloc(65536/8);
	memset(cells, 0, 65536*csz);
	memset(cmask, 0, 65536/8);

//	for(cy=0; cy<256; cy++)
//		for(cx=0; cx<256; cx++)
	for(cy=0; cy<isz; cy++)
		for(cx=0; cx<isz; cx++)
	{
//		cn=cy*256+cx;
		cn=cy*isz+cx;
		cn=remap[cn];
		for(py=0; py<ch; py++)
			for(px=0; px<cw; px++)
		{
			pn=py*cw+px;
			vy=ibuf[((ys-(cy*ch+py)-1)*xs+(cx*cw+px))*4+1];
			if(vy>128)
			{
				cmask[cn>>3]|=1<<(cn&7);
//				cells[(cn*csz)+(pn>>3)]|=1<<(pn&7);
				cells[(cn*csz)+(pn>>3)]|=128>>(pn&7);
			}
		}
	}
	
	
//	ofd=fopen(argv[2], "wb");
	ofd=fopen(ofn, "wb");

	//write head
	fputc(0x00, ofd);	//0xBF00
	fputc(0xBF, ofd);

	fputc(0x04, ofd);	//head len
	fputc(0x00, ofd);

	fputc(0x00, ofd);
	fputc(cw, ofd);
	fputc(ch, ofd);
	fputc(0x01, ofd);

	s=0;
	e=0;

	for(i=0; i<65536; i++)
	{
		if(cmask[i>>3]&(1<<(i&7)))
		{
			if(!s)
			{
				s=i;
				e=i;
			}else e=i;
		}else
		{
			if(s || e)
			{
//				printf("%d-%d ", s, e);

				fputc(s&0xff, ofd);
				fputc((s>>8)&0xff, ofd);
				fputc(e&0xff, ofd);
				fputc((e>>8)&0xff, ofd);
				fwrite(cells+(s*csz), e-s+1, csz, ofd);
			}
			s=0;
			e=0;
		}
	}
	if(s || e)
	{
//		printf("%d-%d ", s, e);

		fputc(s&0xff, ofd);
		fputc((s>>8)&0xff, ofd);
		fputc(e&0xff, ofd);
		fputc((e>>8)&0xff, ofd);
		fwrite(cells+(s*csz), e-s+1, csz, ofd);
	}

	//special end of font marker...
	fputc(0, ofd);
	fputc(0, ofd);
	fputc(0, ofd);
	fputc(0, ofd);

	return(0);
}
