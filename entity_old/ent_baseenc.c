char *base42_enc=
	"BCDFGHJKLMNPQRSTVWXYZbcdfghjklmnpqrstvwxyz";
char *base64_enc=
	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";

char base42_dec[64]={
//@  A  B  C  D  E  F  G   H  I  J  K  L  M  N  O  (40-4F)
 -1,-1, 0, 1, 2,-1, 3, 4,  5,-1, 6, 7, 8, 9,10,-1,
//P  Q  R  S  T  U  V  W   X  Y  Z  [  \  ]  ^  _  (50-5F)
 11,12,13,14,15,-1,16,17, 18,19,20,-1,-1,-1,-1,-1,
//`  a  b  c  d  e  f  g   h  i  j  k  l  m  n  o  (60-6F)
 -1,-1,21,22,23,-1,24,25, 26,-1,27,28,29,30,31,-1,
//p  q  r  s  t  u  v  w   x  y  z  {  |  }  ~  ~  (70-7F)
 32,33,34,35,36,-1,37,38, 39,40,41,-1,-1,-1,-1,-1,
};

void Base42_EncUnit(int iv, char *ov)
{
	int v0, v1, v2;
	
//	v2=iv;
//	v1=(v2*780+328)>>15;
//	v2-=v1*42;
//	v0=(v1*780+328)>>15;
//	v1-=v0*42;
	
	v2=iv;
	v1=v2/42;
	v2-=v1*42;
	v0=v1/42;
	v1-=v0*42;

	ov[0]=base42_enc[v0];
	ov[1]=base42_enc[v1];
	ov[2]=base42_enc[v2];
}

int Base42_DecUnit(char *iv)
{
	int v0, v1, v2;
	int i, j, k;
	
	v0=base42_dec[iv[0]-0x40];
	v1=base42_dec[iv[1]-0x40];
	v2=base42_dec[iv[2]-0x40];
	i=v0*1764+v1*42+v2;
	return(i);
}

int Base42_EncBuffer(byte *ibuf, char *obuf, int isz)
{
	byte *cs, *cse, *cse1;
	char *ct;
	
	cs=ibuf; cse=cs+(isz&(~1)); ct=obuf;
	while(cs<cse)
	{
		cse1=cs+52;
		if(cse<cse1)
		{
			cse1=cse;
			while(cs<cse1)
			{
				Base42_EncUnit(*(u16 *)cs, ct);
				cs+=2; ct+=3;
			}
			if(isz&1)
			{
				Base42_EncUnit((*cs)<<8, ct);
				cs++; ct+=2;
			}
			*ct++='\n';
		}else
		{
			while(cs<cse1)
			{
				Base42_EncUnit(*(u16 *)cs, ct);
				cs+=2; ct+=3;
			}
			*ct++='\n';
		}
	}
	*ct++=0;
}
