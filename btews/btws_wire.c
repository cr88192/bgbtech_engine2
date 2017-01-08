#include <bteifgl.h>

char *btews_wire_Describe(BTEWS_WireNode *self)
{
	char tb[256];
	double f;
	
	f=(self->vnx+self->vpx+self->vny+self->vpy)*0.25;
	sprintf(tb, "%d,%d: V=%2.2f A=%2.2f W=%2.2f ~%2.2fC Pw=%5.2f",
		self->widx, self->widy,
		f, self->icc, self->watt, self->temp, self->pwatt);
	return(frgl_rstrdup(tb));
}

char *btews_wire_DescribeDigital(BTEWS_WireNode *self)
{
	char tb[256];
	double f;
	
	f=(self->vnx+self->vpx+self->vny+self->vpy)*0.25;
	sprintf(tb, "%d,%d: V=%2.2f Wfl=%08X DsTTL=%d",
		self->widx, self->widy,
		f, self->wfl, self->dsttl);
	return(frgl_rstrdup(tb));
}

BTEIFGL_API int btews_wire_checkConnect(BTEWS_WireNode *self,
	BTEWS_WireNode *other)
{
	if(!other)
		return(0);
	if(self==other)
		return(0);
		
	if(self->x==other->x)
	{
		if((self->y<other->y) &&
			((self->wfl&BTEWS_WFL_NOCN_UP) ||
			(other->wfl&BTEWS_WFL_NOCN_DN)))
				return(0);

		if((self->y>other->y) &&
			((self->wfl&BTEWS_WFL_NOCN_DN) ||
			(other->wfl&BTEWS_WFL_NOCN_UP)))
				return(0);

		if((self->wfl&BTEWS_WFL_VERT) && (other->wfl&BTEWS_WFL_VERT))
			return(1);
	}

	if(self->y==other->y)
	{
		if((self->x<other->x) &&
			((self->wfl&BTEWS_WFL_NOCN_RT) ||
			(other->wfl&BTEWS_WFL_NOCN_LF)))
				return(0);

		if((self->x>other->x) &&
			((self->wfl&BTEWS_WFL_NOCN_LF) ||
			(other->wfl&BTEWS_WFL_NOCN_RT)))
				return(0);

		if((self->wfl&BTEWS_WFL_HORZ)&&(other->wfl&BTEWS_WFL_HORZ))
			return(1);
	}

	return(0);
}

BTEIFGL_API int btews_wire_MouseXY(BTEWS_WireNode *self,
	float x0, float y0, float x1, float y1)
{
	float xc, yc;
	float x2, y2, x3, y3;
	int mx, my, mb, fl;

	mx=frgl_state->mx;
	my=-frgl_state->my;
	mb=frgl_state->mb;

	if((mx<x0) || (mx>x1) || (my<y0) || (my>y1))
		return(0);
	
	xc=(x0+x1)*0.5;
	yc=(y0+y1)*0.5;
	
	x2=(x0+xc)*0.5;
	y2=(y0+yc)*0.5;
	x3=(x1+xc)*0.5;
	y3=(y1+yc)*0.5;
	
	fl=0;

	if(mx<xc)fl|=BTEWS_MXY_LF_HALF;
	if(mx>xc)fl|=BTEWS_MXY_RT_HALF;
	if(my>yc)fl|=BTEWS_MXY_UP_HALF;
	if(my<yc)fl|=BTEWS_MXY_DN_HALF;
	
	if(my<y2)
	{
		if(mx<x2)
			{ fl|=BTEWS_MXY_DN_LF; }
		else if(mx>x3)
			{ fl|=BTEWS_MXY_DN_RT; }
		else
			{ fl|=BTEWS_MXY_DN_MI; }
	}else if(my>y3)
	{
		if(mx<x2)
			{ fl|=BTEWS_MXY_UP_LF; }
		else if(mx>x3)
			{ fl|=BTEWS_MXY_UP_RT; }
		else
			{ fl|=BTEWS_MXY_UP_MI; }
	}else
	{
		if(mx<x2)
			{ fl|=BTEWS_MXY_MI_LF; }
		else if(mx>x3)
			{ fl|=BTEWS_MXY_MI_RT; }
		else
			{ fl|=BTEWS_MXY_MI_MI; }
	}
	
	return(fl);
}

BTEIFGL_API void BTEWS_Wire_ColorForVoltage(BTEWS_WireNode *self, double v)
{
	float cr, cg, cb;
	int i, j, k;

//	frglColor4f(0.75+0.1*(v-2.5), 0.75, 0.75-0.1*(v-2.5), 1);

	cr=0.5+0.200*(v-2.5);
	cg=0.5+0.042*(v-6);
	cb=0.5-0.100*(v-2.5);
	if(cb<0)cb=0;
	if(cb>1)cb=1;
	cb+=fabs(v*0.01);

	if(self->cty==BTEWS_CTY_DGTWIRE)
	{
		i=(self->wfl>>BTEWS_WFL_DGTLX_SHR)&15;
		j=(self->wfl>>BTEWS_WFL_DGTLY_SHR)&15;
		k=i;
		if((j^1)>(k^1))k=j;
		switch(k)
		{
		case 0: case 1:
			cr=0.40; cg=0.40; cb=0.15; break;
		case 2: cr=0.85; cg=0.50; cb=0.25; break;
		case 3: cr=0.02; cg=0.05; cb=0.15; break;
		case 4: cr=0.15; cg=0.50; cb=0.75; break;
		case 5: cr=0.05; cg=0.10; cb=0.25; break;
		case 6: cr=0.15; cg=0.50; cb=0.85; break;
		case 7: cr=0.05; cg=0.10; cb=0.20; break;
		default: break;
		}
	}

	frglColor4f(cr, cg, cb, 1);

//	if(self->temp>200)
	if(self->temp>150)
	{
		i=frgl_state->time_f*2;
		if(i&1)
		{
			frglColor4f(1.0, cg*0.25, cb*0.25, 1);
			if(self->temp>1000)
				frglColor4f(0.2, 0.2, 0.2, 1);
		}
	}
}

BTEIFGL_API void btews_wire_Draw(BTEWS_WireNode *self,
	float x0, float y0, float x1, float y1)
{
	BTEWS_WireNode *npx, *nnx, *npy, *nny;
	double f, g, d, v;
	char *s;
	int mx, my, mb, mfl;
	int tex, ch, cv;
	int i, j, k;

	mfl=btews_wire_MouseXY(self, x0, y0, x1, y1);

	mx=frgl_state->mx;
	my=-frgl_state->my;
	mb=frgl_state->mb;

	if(self->wfl&BTEWS_WFL_JOIN)
		{ v=(self->vnx+self->vpx+self->vny+self->vpy)*0.25; }
	else if((self->wfl&BTEWS_WFL_CN_LFRT) &&
		(self->wfl&BTEWS_WFL_CN_UPDN))
			{ v=(self->vnx+self->vpx+self->vny+self->vpy)*0.25; }
	else if(self->wfl&BTEWS_WFL_CN_LFRT)
		{ v=(self->vnx+self->vpx)*0.5; }
	else if(self->wfl&BTEWS_WFL_CN_UPDN)
		{ v=(self->vny+self->vpy)*0.5; }
	else
		{ v=0; }
	
	BTEWS_Wire_ColorForVoltage(self, v);

//	frglColor4f(1, 1, 1, 1);

	npx=BTEWS_GetNode(self->grid, self->x+1, self->y  );
	nnx=BTEWS_GetNode(self->grid, self->x-1, self->y  );
	npy=BTEWS_GetNode(self->grid, self->x  , self->y+1);
	nny=BTEWS_GetNode(self->grid, self->x  , self->y-1);

	if((mx>=x0) && (mx<=x1) &&
		(my>=y0) && (my<=y1))
	{
		frglColor4f(0.5, 1, 0.5, 1);
		
		if(mb&1)
		{
			frglColor4f(1, 0.5, 1, 1);
//			ncur=BTEWS_GetAllocNode(grid, x, y);
//			ncur->Draw=btews_wire_Draw;

#if 1
			if(mfl&BTEWS_MXY_MI_LF)
			{
				self->wfl&=~BTEWS_WFL_NOCN_LF;
				if(nnx)nnx->wfl&=~BTEWS_WFL_NOCN_RT;
			}

			if(mfl&BTEWS_MXY_MI_RT)
			{
				self->wfl&=~BTEWS_WFL_NOCN_RT;
				if(npx)npx->wfl&=~BTEWS_WFL_NOCN_LF;
			}

			if(mfl&BTEWS_MXY_UP_MI)
			{
				self->wfl&=~BTEWS_WFL_NOCN_UP;
				if(npy)npy->wfl&=~BTEWS_WFL_NOCN_DN;
			}

			if(mfl&BTEWS_MXY_DN_MI)
			{
				self->wfl&=~BTEWS_WFL_NOCN_DN;
				if(nny)nny->wfl&=~BTEWS_WFL_NOCN_UP;
			}
#endif
			
#if 0
			if((mfl&BTEWS_MXY_MI_LF) ||
				(mfl&BTEWS_MXY_MI_RT))
			{
				self->wfl|=BTEWS_WFL_HORZ;
			}

			if((mfl&BTEWS_MXY_UP_MI) ||
				(mfl&BTEWS_MXY_DN_MI))
			{
				self->wfl|=BTEWS_WFL_VERT;
			}
#endif
			
			if(mfl&BTEWS_MXY_MI_MI)
			{
				self->wfl|=BTEWS_WFL_JOIN;
			}
		}

		if(mb&4)
		{
			frglColor4f(1, 0.5, 1, 1);
//			ncur=BTEWS_GetAllocNode(grid, x, y);
//			ncur->Draw=btews_wire_Draw;

#if 1
			if(mfl&BTEWS_MXY_MI_LF)
			{
				self->wfl|=BTEWS_WFL_NOCN_LF;
				if(nnx)nnx->wfl|=BTEWS_WFL_NOCN_RT;
			}

			if(mfl&BTEWS_MXY_MI_RT)
			{
				self->wfl|=BTEWS_WFL_NOCN_RT;
				if(npx)npx->wfl|=BTEWS_WFL_NOCN_LF;
			}

			if(mfl&BTEWS_MXY_UP_MI)
			{
				self->wfl|=BTEWS_WFL_NOCN_UP;
				if(npy)npy->wfl|=BTEWS_WFL_NOCN_DN;
			}

			if(mfl&BTEWS_MXY_DN_MI)
			{
				self->wfl|=BTEWS_WFL_NOCN_DN;
				if(nny)nny->wfl|=BTEWS_WFL_NOCN_UP;
			}
#endif

#if 0
			if((mfl&BTEWS_MXY_MI_LF) ||
				(mfl&BTEWS_MXY_MI_RT))
			{
				self->wfl&=~BTEWS_WFL_HORZ;
			}

			if((mfl&BTEWS_MXY_UP_MI) ||
				(mfl&BTEWS_MXY_DN_MI))
			{
				self->wfl&=~BTEWS_WFL_VERT;
			}
#endif

			if(mfl&BTEWS_MXY_MI_MI)
			{
				self->wfl&=~BTEWS_WFL_JOIN;
			}
		}
	}
	
	ch=0; cv=0;
	self->wfl&=~BTEWS_WFL_CN_ALL;
	if(btews_wire_checkConnect(self, nnx))
		{ self->wfl|=BTEWS_WFL_CN_LF; ch=1; }
	if(btews_wire_checkConnect(self, npx))
		{ self->wfl|=BTEWS_WFL_CN_RT; ch=1; }
	if(btews_wire_checkConnect(self, npy))
		{ self->wfl|=BTEWS_WFL_CN_UP; cv=1; }
	if(btews_wire_checkConnect(self, nny))
		{ self->wfl|=BTEWS_WFL_CN_DN; cv=1; }

	if(self->wfl!=self->owfl)
	{
		self->owfl=self->wfl;
		BTEWS_MarkNodeDirtyStar(self->grid, self->x, self->y);
	}

#if 0
	if(ch && !cv)
	{
		tex=Tex_LoadFile("images/wire_horz.png", NULL, NULL);
	}else if(!ch && cv)
	{
		tex=Tex_LoadFile("images/wire_vert.png", NULL, NULL);
	}else if(ch && cv)
#endif

	if(ch || cv)
	{
		if(self->wfl&BTEWS_WFL_JOIN)
		{
			switch(self->wfl&BTEWS_WFL_CN_ALL)
			{
			case BTEWS_WFL_CN_LF:
				s="images/wire_cn_lf.png"; break;
			case BTEWS_WFL_CN_RT:
				s="images/wire_cn_rt.png"; break;
			case BTEWS_WFL_CN_UP:
				s="images/wire_cn_up.png"; break;
			case BTEWS_WFL_CN_DN:
				s="images/wire_cn_dn.png"; break;

			case BTEWS_WFL_CN_LFRT:
				s="images/wire_horz.png"; break;
			case BTEWS_WFL_CN_UPDN:
				s="images/wire_vert.png"; break;

			case BTEWS_WFL_CN_UPLF:
				s="images/wire_join_uplf.png"; break;
			case BTEWS_WFL_CN_UPRT:
				s="images/wire_join_uprt.png"; break;
			case BTEWS_WFL_CN_DNLF:
				s="images/wire_join_dnlf.png"; break;
			case BTEWS_WFL_CN_DNRT:
				s="images/wire_join_dnrt.png"; break;
			case BTEWS_WFL_CN_UPLRT:
				s="images/wire_join_uplrt.png"; break;
			case BTEWS_WFL_CN_DNLRT:
				s="images/wire_join_dnlrt.png"; break;
			case BTEWS_WFL_CN_UDNLF:
				s="images/wire_join_udnlf.png"; break;
			case BTEWS_WFL_CN_UDNRT:
				s="images/wire_join_udnrt.png"; break;
			default:
				s="images/wire_join.png"; break;
			}
			
			tex=Tex_LoadFile(s, NULL, NULL);
		}else
		{
			switch(self->wfl&BTEWS_WFL_CN_ALL)
			{
			case BTEWS_WFL_CN_LF:
				s="images/wire_cn_lf.png"; break;
			case BTEWS_WFL_CN_RT:
				s="images/wire_cn_rt.png"; break;
			case BTEWS_WFL_CN_UP:
				s="images/wire_cn_up.png"; break;
			case BTEWS_WFL_CN_DN:
				s="images/wire_cn_dn.png"; break;

			case BTEWS_WFL_CN_LFRT:
				s="images/wire_horz.png"; break;
			case BTEWS_WFL_CN_UPDN:
				s="images/wire_vert.png"; break;

			default:
				s="images/wire_cross.png";
				break;
			}

			tex=Tex_LoadFile(s, NULL, NULL);
		}

	}else
	{
		tex=Tex_LoadFile("images/closedhole0.png", NULL, NULL);
	}

	frglEnableTexture2D();
	frglBindTexture2D(tex);
	frglBegin(GL_QUADS);
	frglTexCoord2f(0, 0);
	frglVertex2f(x0, y0);
	frglTexCoord2f(1, 0);
	frglVertex2f(x1, y0);
	frglTexCoord2f(1, 1);
	frglVertex2f(x1, y1);
	frglTexCoord2f(0, 1);
	frglVertex2f(x0, y1);
	frglEnd();
}

BTEIFGL_API void btews_wire_UpdateAdjacent(BTEWS_WireNode *self)
{
	if(fabs(self->vnx-self->ovnx)>0.01)
	{
		self->ovnx=self->vnx;
		BTEWS_MarkNodeDirty(self->grid, self->x-1, self->y);
	}

	if(fabs(self->vpx-self->ovpx)>0.01)
	{
		self->ovpx=self->vpx;
		BTEWS_MarkNodeDirty(self->grid, self->x+1, self->y);
	}

	if(fabs(self->vny-self->ovny)>0.01)
	{
		self->ovny=self->vny;
		BTEWS_MarkNodeDirty(self->grid, self->x, self->y-1);
	}

	if(fabs(self->vpy-self->ovpy)>0.01)
	{
		self->ovpy=self->vpy;
		BTEWS_MarkNodeDirty(self->grid, self->x, self->y+1);
	}
}

void btews_wire_DigitalPick(BTEWS_WireNode *self, BTEWS_WireNode *other,
	int *rdg, int adg, int *rsrc, int smsk)
{
	int dg, src, srp;
	
	dg=*rdg;
	src=*rsrc;

	srp=other->dsttl>self->dsttl;

	if(src&smsk)
	{
		switch(dg)
		{
		case 0:
		case 1:
			switch(adg)
			{
			case 0: break;
			case 1: break;
			case 2: dg=2; src=smsk; break;
			case 3: dg=3; src=smsk; break;
			case 4: dg=4; src=smsk; break;
			case 5: dg=5; src=smsk; break;
			case 6: dg=6; src=smsk; break;
			case 7: dg=7; src=smsk; break;
			}
			break;
		case 2:
			if(srp && (adg==3))	{ dg=3; src=smsk; break; }
			if(adg!=2)	{ dg=1; src=0; break; }
			break;
		case 3:
			if(srp && (adg==2))	{ dg=2; src=smsk; break; }
			if(adg!=3)	{ dg=1; src=0; break; }
			break;

		case 4:
			if(adg==2)	{ dg=2; src=smsk; break; }
			if(adg==3)	{ dg=3; src=smsk; break; }
			if(adg==6)	{ dg=6; src=smsk; break; }
			if(adg==7)	{ dg=7; src=smsk; break; }
			if(srp && (adg==5))	{ dg=5; src=smsk; break; }
			if(adg!=4)	{ dg=1; src=0; break; }
			break;
		case 5:
			if(adg==2)	{ dg=2; src=smsk; break; }
			if(adg==3)	{ dg=3; src=smsk; break; }
			if(adg==6)	{ dg=6; src=smsk; break; }
			if(adg==7)	{ dg=7; src=smsk; break; }
			if(srp && (adg==4))	{ dg=4; src=smsk; break; }
			if(adg!=5)	{ dg=1; src=0; break; }
			break;

		case 6:
			if(adg==2)	{ dg=2; src=smsk; break; }
			if(adg==3)	{ dg=3; src=smsk; break; }
			if(srp && (adg==7))	{ dg=7; src=smsk; break; }
			if(adg!=6)	{ dg=1; src=0; break; }
			break;
		case 7:
			if(adg==2)	{ dg=2; src=smsk; break; }
			if(adg==3)	{ dg=3; src=smsk; break; }
			if(srp && (adg==6))	{ dg=6; src=smsk; break; }
			if(adg!=7)	{ dg=1; src=0; break; }
			break;
		}
	}else
	{
		switch(dg)
		{
		case 0:
		case 1:
			switch(adg)
			{
			case 0: break;
			case 1: break;
			case 2: dg=2; src=smsk; break;
			case 3: dg=3; src=smsk; break;
			case 4: dg=4; src=smsk; break;
			case 5: dg=5; src=smsk; break;
			case 6: dg=6; src=smsk; break;
			case 7: dg=7; src=smsk; break;
			}
			break;
		case 2:
			if(adg==3)
				{ dg=1; src=0; break; }
			break;
		case 3:
			if(adg==2)
				{ dg=1; src=0; break; }
			break;

		case 4:
			if(adg==2)	{ dg=2; src=smsk; break; }
			if(adg==3)	{ dg=3; src=smsk; break; }
			if(adg==6)	{ dg=6; src=smsk; break; }
			if(adg==7)	{ dg=7; src=smsk; break; }

			if(srp && (adg==4))	{ dg=4; src=smsk; break; }
			if(srp && (adg==5))	{ dg=5; src=smsk; break; }

//			if(adg==5)	{ dg=1; src=0; break; }
//			if(adg!=4)
//				{ dg=1; src=0; break; }
			break;
		case 5:
			if(adg==2)	{ dg=2; src=smsk; break; }
			if(adg==3)	{ dg=3; src=smsk; break; }
			if(adg==7)	{ dg=7; src=smsk; break; }
			if(adg==6)	{ dg=6; src=smsk; break; }

			if(srp && (adg==4))	{ dg=4; src=smsk; break; }
			if(srp && (adg==5))	{ dg=5; src=smsk; break; }

//			if(adg!=5)	{ dg=1; src=0; break; }
//			if(adg==4)	{ dg=1; src=0; break; }
			break;

		case 6:
			if(adg==2)	{ dg=2; src=smsk; break; }
			if(adg==3)	{ dg=3; src=smsk; break; }

			if(srp && (adg==6))	{ dg=6; src=smsk; break; }
			if(srp && (adg==7))	{ dg=7; src=smsk; break; }

//			if(adg!=6)	{ dg=1; src=0; break; }
//			if(adg==7)	{ dg=1; src=0; break; }
			break;
		case 7:
			if(adg==2)	{ dg=2; src=smsk; break; }
			if(adg==3)	{ dg=3; src=smsk; break; }

			if(srp && (adg==6))	{ dg=6; src=smsk; break; }
			if(srp && (adg==7))	{ dg=7; src=smsk; break; }

//			if(adg!=7)	{ dg=1; src=0; break; }
			break;
		}
	}
	
	*rdg=dg;
	*rsrc=src;
}

BTEIFGL_API void btews_wire_UpdateDigital(BTEWS_WireNode *self, double dt)
{
	BTEWS_WireNode *npx, *nnx, *npy, *nny;
	double f, g, h, d, v, vm, vn, v0, v1, vc0, vct;
	double w, wm, wn, w0, w1;
	double ic, im, in, il;
	int sd, sd0, dg, vtr, dsttl;

//	if(self->wfl&(BTEWS_WFL_DGTL_ALL|BTEWS_WFL_DGTL2_ALL))
	if(1)
	{
		if((self->wfl&BTEWS_WFL_JOIN) &&
			(self->wfl&BTEWS_WFL_CN_UPDN) &&
			(self->wfl&BTEWS_WFL_CN_LFRT))
		{
			npx=BTEWS_GetNode(self->grid, self->x+1, self->y  );
			nnx=BTEWS_GetNode(self->grid, self->x-1, self->y  );
			npy=BTEWS_GetNode(self->grid, self->x  , self->y+1);
			nny=BTEWS_GetNode(self->grid, self->x  , self->y-1);
			sd0=self->wfl&BTEWS_WFL_SDIR_ALL;
			
			dg=(self->wfl>>16)&15;
			if(!sd0 && (dg>1))dg=1;
			dsttl=0;

			if(self->wfl&BTEWS_WFL_CN_RT)
			{
				if((npx->wfl>>16)&15)
				{
					btews_wire_DigitalPick(self, npx,
						&dg, (npx->wfl>>16)&15, &sd0, BTEWS_WFL_SDIR_RT);
//					if((sd0&BTEWS_WFL_SDIR_RT) &&
//						(npx->wfl&BTEWS_WFL_SDIR_LF))
//							{ dg=1; sd0=0; }
					if((sd0&BTEWS_WFL_SDIR_RT) &&
						(npx->dsttl<=self->dsttl))
							{ dg=1; sd0=0; }
				}else
				{
					if(npx->dsttl>dsttl)
						dg=(npx->vnx>1.5)?6:7;
				}
				if(npx->dsttl>dsttl)
					{ dsttl=npx->dsttl-1; sd0=BTEWS_WFL_SDIR_RT; }
			}else
			{
				if(sd0&BTEWS_WFL_SDIR_RT)
					{ dg=1; sd0=0; }
			}

			if(self->wfl&BTEWS_WFL_CN_LF)
			{
				if((nnx->wfl>>16)&15)
				{
					btews_wire_DigitalPick(self, nnx,
						&dg, (nnx->wfl>>16)&15, &sd0, BTEWS_WFL_SDIR_LF);
//					if((sd0&BTEWS_WFL_SDIR_LF) &&
//						(nnx->wfl&BTEWS_WFL_SDIR_RT))
//							{ dg=1; sd0=0; }
					if((sd0&BTEWS_WFL_SDIR_LF) &&
						(nnx->dsttl<=self->dsttl))
							{ dg=1; sd0=0; }
				}else
				{
					if(nnx->dsttl>dsttl)
						dg=(nnx->vpx>1.5)?6:7;
				}
				if(nnx->dsttl>dsttl)
					{ dsttl=nnx->dsttl-1; sd0=BTEWS_WFL_SDIR_LF; }
			}else
			{
				if(sd0&BTEWS_WFL_SDIR_LF)
					{ dg=1; sd0=0; }
			}
			
			if(self->wfl&BTEWS_WFL_CN_UP)
			{
				if((npy->wfl>>20)&15)
				{
					btews_wire_DigitalPick(self, npy,
						&dg, (npy->wfl>>20)&15, &sd0, BTEWS_WFL_SDIR_UP);
//					if((sd0&BTEWS_WFL_SDIR_UP) &&
//						(npy->wfl&BTEWS_WFL_SDIR_DN))
//							{ dg=1; sd0=0; }
					if((sd0&BTEWS_WFL_SDIR_UP) &&
						(npy->dsttl<=self->dsttl))
							{ dg=1; sd0=0; }
				}else
				{
					if(npy->dsttl>dsttl)
						dg=(npy->vny>1.5)?6:7;
				}
				if(npy->dsttl>dsttl)
					{ dsttl=npy->dsttl-1; sd0=BTEWS_WFL_SDIR_UP; }
			}else
			{
				if(sd0&BTEWS_WFL_SDIR_UP)
					{ dg=1; sd0=0; }
			}

			if(self->wfl&BTEWS_WFL_CN_DN)
			{
				if((nny->wfl>>20)&15)
				{
					btews_wire_DigitalPick(self, nny,
						&dg, (nny->wfl>>20)&15, &sd0, BTEWS_WFL_SDIR_DN);
//					if((sd0&BTEWS_WFL_SDIR_DN) &&
//						(nny->wfl&BTEWS_WFL_SDIR_UP))
//							{ dg=1; sd0=0; }
					if((sd0&BTEWS_WFL_SDIR_DN) &&
						(nny->dsttl<=self->dsttl))
							{ dg=1; sd0=0; }
				}else
				{
					if(nny->dsttl>dsttl)
						dg=(nny->vpy>1.5)?6:7;
				}
				if(nny->dsttl>dsttl)
					{ dsttl=nny->dsttl-1; sd0=BTEWS_WFL_SDIR_DN; }
			}else
			{
				if(sd0&BTEWS_WFL_SDIR_DN)
					{ dg=1; sd0=0; }
			}

			if(dsttl<=0)
					{ dg=1; sd0=0; }

			self->wfl&=~(BTEWS_WFL_SDIR_ALL|
				BTEWS_WFL_DGTL_ALL|BTEWS_WFL_DGTL2_ALL);
			self->wfl|=sd0|(dg<<16)|(dg<<20);
			self->dsttl=dsttl;

			switch(dg)
			{
			case 0:	v=1.6; break;
			case 1:	v=1.6; break;
			case 2:	v=3.3; break;
			case 3:	v=0.0; break;
			case 4:	v=3.3; break;
			case 5:	v=0.0; break;
			case 6:	v=3.3; break;
			case 7:	v=0.0; break;
			}
			
			if(dg==1)
				dg=0;
			
			ic=0; w=0;

//			il=(vn>vm)?(w/(vn-vm)):0;
//			if(ic>il)ic=il;
			if(dg)
			{
				self->vnx=v;	self->vpx=v;
				self->vny=v;	self->vpy=v;

				self->inx=ic;	self->ipx=ic;
				self->iny=ic;	self->ipy=ic;

				self->pwnx=w;	self->pwpx=w;
				self->pwny=w;	self->pwpy=w;
			}
		}else
		{
			if(self->wfl&BTEWS_WFL_CN_LFRT)
			{
				npx=BTEWS_GetNode(self->grid, self->x+1, self->y  );
				nnx=BTEWS_GetNode(self->grid, self->x-1, self->y  );
				sd0=self->wfl&BTEWS_WFL_SDIR_LFRT;
				
				dg=(self->wfl>>16)&15;
				if(!sd0 && (dg>1))dg=1;
				dsttl=0;

				if(self->wfl&BTEWS_WFL_CN_RT)
				{
					if((npx->wfl>>16)&15)
					{
						btews_wire_DigitalPick(self, npx,
							&dg, (npx->wfl>>16)&15,
							&sd0, BTEWS_WFL_SDIR_RT);
//						if((sd0&BTEWS_WFL_SDIR_RT) &&
//							(npx->wfl&BTEWS_WFL_SDIR_LF))
//								{ dg=1; sd0=0; }
						if((sd0&BTEWS_WFL_SDIR_RT) &&
							(npx->dsttl<=self->dsttl))
								{ dg=1; sd0=0; }
					}else
					{
						if(npx->dsttl>dsttl)
							dg=(npx->vnx>1.5)?6:7;
					}
					if(npx->dsttl>dsttl)
						{ dsttl=npx->dsttl-1; sd0=BTEWS_WFL_SDIR_RT; }
				}else
				{
					if(sd0&BTEWS_WFL_SDIR_RT)
						{ dg=1; sd0=0; }
				}

				if(self->wfl&BTEWS_WFL_CN_LF)
				{
					if((nnx->wfl>>16)&15)
					{
						btews_wire_DigitalPick(self, nnx,
							&dg, (nnx->wfl>>16)&15, &sd0, BTEWS_WFL_SDIR_LF);
//						if((sd0&BTEWS_WFL_SDIR_LF) &&
//							(nnx->wfl&BTEWS_WFL_SDIR_RT))
//								{ dg=1; sd0=0; }
						if((sd0&BTEWS_WFL_SDIR_LF) &&
							(nnx->dsttl<=self->dsttl))
								{ dg=1; sd0=0; }
					}else
					{
						if(nnx->dsttl>dsttl)
							dg=(nnx->vpx>1.5)?6:7;
					}
					if(nnx->dsttl>dsttl)
						{ dsttl=nnx->dsttl-1; sd0=BTEWS_WFL_SDIR_LF; }
				}else
				{
					if(sd0&BTEWS_WFL_SDIR_LF)
						{ dg=1; sd0=0; }
				}

				if(dsttl<=0)
					{ dg=1; sd0=0; }

				self->wfl&=~(BTEWS_WFL_SDIR_LFRT|BTEWS_WFL_DGTL_ALL);
				self->wfl|=sd0|(dg<<16);
				self->dsttl=dsttl;

				switch(dg)
				{
				case 0:	v=1.6; break;
				case 1:	v=1.6; break;
				case 2:	v=3.3; break;
				case 3:	v=0.0; break;
				case 4:	v=3.3; break;
				case 5:	v=0.0; break;
				case 6:	v=3.3; break;
				case 7:	v=0.0; break;
				}

				if(dg==1)
					dg=0;

				if(dg)
				{
					ic=0; w=0;
					self->vnx=v;	self->vpx=v;
					self->inx=ic;	self->ipx=ic;
					self->pwnx=w;	self->pwpx=w;

					if(!(self->wfl&BTEWS_WFL_CN_UPDN))
					{
						self->vny=v;	self->vpy=v;
						self->iny=ic;	self->ipy=ic;
						self->pwny=w;	self->pwpy=w;
					}
				}
			}


			if(self->wfl&BTEWS_WFL_CN_UPDN)
			{
				npy=BTEWS_GetNode(self->grid, self->x  , self->y+1);
				nny=BTEWS_GetNode(self->grid, self->x  , self->y-1);
				sd0=self->wfl&BTEWS_WFL_SDIR_UPDN;
				
				dg=(self->wfl>>20)&15;
				if(!sd0 && (dg>1))dg=1;
				dsttl=0;

				if(self->wfl&BTEWS_WFL_CN_UP)
				{
					if((npy->wfl>>20)&15)
					{
						btews_wire_DigitalPick(self, npy,
							&dg, (npy->wfl>>20)&15,
							&sd0, BTEWS_WFL_SDIR_UP);
//						if((sd0&BTEWS_WFL_SDIR_UP) &&
//							(npy->wfl&BTEWS_WFL_SDIR_DN))
//								{ dg=1; sd0=0; }
						if((sd0&BTEWS_WFL_SDIR_UP) &&
							(npy->dsttl<=self->dsttl))
								{ dg=1; sd0=0; }
					}else
					{
						if(npy->dsttl>dsttl)
							dg=(npy->vny>1.5)?6:7;
					}
					if(npy->dsttl>dsttl)
						{ dsttl=npy->dsttl-1; sd0=BTEWS_WFL_SDIR_UP; }
				}else
				{
					if(sd0&BTEWS_WFL_SDIR_UP)
						{ dg=1; sd0=0; }
				}

				if(self->wfl&BTEWS_WFL_CN_DN)
				{
					if((nny->wfl>>20)&15)
					{
						btews_wire_DigitalPick(self, nny,
							&dg, (nny->wfl>>20)&15, &sd0, BTEWS_WFL_SDIR_DN);
//						if((sd0&BTEWS_WFL_SDIR_DN) &&
//							(nny->wfl&BTEWS_WFL_SDIR_UP))
//								{ dg=1; sd0=0; }
						if((sd0&BTEWS_WFL_SDIR_DN) &&
							(nny->dsttl<=self->dsttl))
								{ dg=1; sd0=0; }
					}else
					{
						if(nny->dsttl>dsttl)
							dg=(nny->vpy>1.5)?6:7;
					}
					if(nny->dsttl>dsttl)
						{ dsttl=nny->dsttl-1; sd0=BTEWS_WFL_SDIR_DN; }
				}else
				{
					if(sd0&BTEWS_WFL_SDIR_DN)
						{ dg=1; sd0=0; }
				}
	
				if(dsttl<=0)
					{ dg=1; sd0=0; }

				self->wfl&=~(BTEWS_WFL_SDIR_UPDN|BTEWS_WFL_DGTL2_ALL);
				self->wfl|=sd0|(dg<<20);
				self->dsttl=dsttl;

				switch(dg)
				{
				case 0:	v=1.6; break;
				case 1:	v=1.6; break;
				case 2:	v=3.3; break;
				case 3:	v=0.0; break;
				case 4:	v=3.3; break;
				case 5:	v=0.0; break;
				case 6:	v=3.3; break;
				case 7:	v=0.0; break;
				}
				
				if(dg==1)
					dg=0;

				if(dg)
				{
					ic=0; w=0;
					self->vny=v;	self->vpy=v;
					self->iny=ic;	self->ipy=ic;
					self->pwny=w;	self->pwpy=w;

					if(!(self->wfl&BTEWS_WFL_CN_LFRT))
					{
						self->vnx=v;	self->vpx=v;
						self->inx=ic;	self->ipx=ic;
						self->pwnx=w;	self->pwpx=w;
					}
				}
			}

		}

//		return;
	}
}

BTEIFGL_API void btews_wire_Update(BTEWS_WireNode *self, double dt)
{
	BTEWS_WireNode *npx, *nnx, *npy, *nny;
	double f, g, h, d, v, vm, vn, v0, v1, vc0, vct;
	double w, wm, wn, w0, w1;
	double ic, im, in, il;
	int sd, sd0, dg, vtr, dsttl;

//	if(((self->wfl&BTEWS_WFL_DGTL_ALL) ||
//			!(self->wfl&BTEWS_WFL_CN_LFRT)) &&
//		((self->wfl&BTEWS_WFL_DGTL2_ALL) ||
//			!(self->wfl&BTEWS_WFL_CN_UPDN)))
//	{
//		return;
//	}

	if((self->wfl&BTEWS_WFL_JOIN) &&
		(self->wfl&BTEWS_WFL_CN_UPDN) &&
		(self->wfl&BTEWS_WFL_CN_LFRT))
	{
		npx=BTEWS_GetNode(self->grid, self->x+1, self->y  );
		nnx=BTEWS_GetNode(self->grid, self->x-1, self->y  );
		npy=BTEWS_GetNode(self->grid, self->x  , self->y+1);
		nny=BTEWS_GetNode(self->grid, self->x  , self->y-1);

		v=0; d=0; ic=0; w=0; sd=0;
		vm=999999; vn=-999999;
		im=999999; in=-999999;
		wm=999999; wn=-999999;
		dsttl=0;

#if 1
		f=(self->vnx+self->vpx+self->vny+self->vpy)*0.25;
		g=(self->inx+self->ipx+self->iny+self->ipy)*0.25;
		h=(self->pwnx+self->pwpx+self->pwny+self->pwpy)*0.25;
		v+=f; ic+=g; d+=1; w+=h;

		vc0=f;
		vct=0; vtr=0;

		if(f<vm)vm=f;
		if(f>vn)vn=f;
		if(g<im)im=g;
		if(g>in)in=g;
		if(h<wm)wm=h;
		if(h>wn)wn=h;
#endif

		if(self->wfl&BTEWS_WFL_CN_LF)
		{
			f=nnx->vpx; v+=f; d+=1;
			g=nnx->ipx; ic+=g;
			h=nnx->pwpx; w+=h;
			if(f<vm)vm=f;
			if(f>vn) { vn=f; sd=BTEWS_WFL_SDIR_LF; }
			if(g<im)im=g;
			if(g>in)in=g;
			if(h<wm)wm=h;
			if(h>wn)wn=h;
			
			if((self->wfl&BTEWS_WFL_SDIR_LF) && (f<vc0))
				{ vct=nnx->vpx; vtr=1; }
			if(nnx->dsttl>dsttl)
				dsttl=nnx->dsttl-1;
		}else
		{
			if(self->wfl&BTEWS_WFL_SDIR_LF)
				{ vct=0; vtr=1; }
		}

		if(self->wfl&BTEWS_WFL_CN_RT)
		{
			f=npx->vnx; v+=f; d+=1;
			g=npx->inx; ic+=g;
			h=npx->pwnx; w+=h;
			if(f<vm)vm=f;
			if(f>vn) { vn=f; sd=BTEWS_WFL_SDIR_RT; }
			if(g<im)im=g;
			if(g>in)in=g;
			if(h<wm)wm=h;
			if(h>wn)wn=h;

			if((self->wfl&BTEWS_WFL_SDIR_RT) && (f<vc0))
				{ vct=npx->vnx; vtr=1; }
			if(npx->dsttl>dsttl)
				dsttl=npx->dsttl-1;
		}else
		{
			if(self->wfl&BTEWS_WFL_SDIR_RT)
				{ vct=0; vtr=1; }
		}

		if(self->wfl&BTEWS_WFL_CN_UP)
		{
			f=npy->vny; v+=f; d+=1;
			g=npy->iny; ic+=g;
			h=npy->pwny; w+=h;
			if(f<vm)vm=f;
			if(f>vn) { vn=f; sd=BTEWS_WFL_SDIR_UP; }
			if(g<im)im=g;
			if(g>in)in=g;
			if(h<wm)wm=h;
			if(h>wn)wn=h;

			if((self->wfl&BTEWS_WFL_SDIR_UP) && (f<vc0))
				{ vct=npy->vny; vtr=1; }
			if(npy->dsttl>dsttl)
				dsttl=npy->dsttl-1;
		}else
		{
			if(self->wfl&BTEWS_WFL_SDIR_UP)
				{ vct=0; vtr=1; }
		}

		if(self->wfl&BTEWS_WFL_CN_DN)
		{
			f=nny->vpy; v+=f; d+=1;
			g=nny->ipy; ic+=g;
			h=nny->pwpy; w+=h;
			if(f<vm)vm=f;
			if(f>vn) { vn=f; sd=BTEWS_WFL_SDIR_DN; }
			if(g<im)im=g;
			if(g>in)in=g;
			if(h<wm)wm=h;
			if(h>wn)wn=h;

			if((self->wfl&BTEWS_WFL_SDIR_DN) && (f<vc0))
				{ vct=nny->vpy; vtr=1; }
			if(nny->dsttl>dsttl)
				dsttl=nny->dsttl-1;
		}else
		{
			if(self->wfl&BTEWS_WFL_SDIR_DN)
				{ vct=0; vtr=1; }
		}

		if(d>0)
		{
			v/=d;
			ic/=d;
			w/=d;
		}
		
		if(vtr)
		{
			v=vct;
			w=0;
		}

//		if(wm==wn)w=w*(1.0-dt);
//		if(wm==wn)w=w*0.9;
		if(fabs(wm-wn)<0.001)w=w*0.9;

		self->wfl&=~BTEWS_WFL_SDIR_ALL;
		self->wfl|=sd;
		self->dsttl=dsttl;

		il=(vn>vm)?(w/(vn-vm)):0;
		if(ic>il)ic=il;
		
		self->vnx=v;	self->vpx=v;
		self->vny=v;	self->vpy=v;

		self->inx=ic;	self->ipx=ic;
		self->iny=ic;	self->ipy=ic;

		self->pwnx=w;	self->pwpx=w;
		self->pwny=w;	self->pwpy=w;

		if(vn>=vm)
		{
//			il=w/(vn-vm);
//			il=(vn>vm)?(w/(vn-vm)):0;
			
			self->icc=(vn-vm)/self->ohm;
			if(self->icc>il)self->icc=il;
			
//			self->icc=(self->icc+ic)*0.5;
			self->inx=self->icc;
			self->ipx=self->icc;
			self->iny=self->icc;
			self->ipy=self->icc;
			
			self->watt=(vn-vm)*self->icc;
			self->pwatt=wn;
		
//			self->temp=(self->temp+self->watt)-dt*((self->temp-25)/10.0+1);
//			self->temp=(self->temp+self->watt)-dt*(self->temp-25);
//			self->temp=(self->temp+self->watt*dt)-dt*((self->temp-25)/10.0+1);
//			self->temp=(self->temp+self->watt*dt)-dt*((self->temp-25)/10.0);
			self->temp=self->temp+((1.0/0.004)*self->watt*dt*dt)-
				dt*((self->temp-25)/25.0);
//			if(self->temp<25)self->temp=25;
		}else
		{
			self->icc=0;
			self->watt=0;
			self->temp=25;
			self->pwatt=0;
		}
	}else
	{
		if(self->wfl&BTEWS_WFL_CN_LFRT)
		{
			npx=BTEWS_GetNode(self->grid, self->x+1, self->y  );
			nnx=BTEWS_GetNode(self->grid, self->x-1, self->y  );

			vm=999999; vn=-999999;
			im=999999; in=-999999;
			wm=999999; wn=-999999;
			v=0; d=0; ic=0; w=0; sd=0;
			dsttl=0;

#if 1
			f=(self->vnx+self->vpx)*0.5;
			g=(self->inx+self->ipx)*0.5;
			h=(self->pwnx+self->pwpx)*0.5;
			v+=f; ic+=g; d+=1; w+=h;

			vc0=f;
			vct=0; vtr=0;

			if(f<vm)vm=f;
			if(f>vn)vn=f;
			if(g<im)im=g;
			if(g>in)in=g;
			if(h<wm)wm=h;
			if(h>wn)wn=h;
#endif

			if(self->wfl&BTEWS_WFL_CN_LF)
			{
				f=nnx->vpx; v+=f; d+=1;
				g=nnx->ipx; ic+=g;
				h=nnx->pwpx; w+=h;
				if(f<vm)vm=f;
				if(f>vn) { vn=f; sd=BTEWS_WFL_SDIR_LF; }
				if(g<im)im=g;
				if(g>in)in=g;
				if(h<wm)wm=h;
				if(h>wn)wn=h;

				if((self->wfl&BTEWS_WFL_SDIR_LF) && (f<vc0))
					{ vct=nnx->vpx; vtr=1; }
				if(nnx->dsttl>dsttl)
					dsttl=nnx->dsttl-1;
			}else
			{
				if(self->wfl&BTEWS_WFL_SDIR_LF)
					{ vct=0; vtr=1; }
			}

			if(self->wfl&BTEWS_WFL_CN_RT)
			{
				f=npx->vnx; v+=f; d+=1;
				g=npx->inx; ic+=g;
				h=npx->pwnx; w+=h;
				if(f<vm)vm=f;
				if(f>vn) { vn=f; sd=BTEWS_WFL_SDIR_RT; }
				if(g<im)im=g;
				if(g>in)in=g;
				if(h<wm)wm=h;
				if(h>wn)wn=h;

				if((self->wfl&BTEWS_WFL_SDIR_RT) && (f<vc0))
					{ vct=npx->vnx; vtr=1; }
				if(npx->dsttl>dsttl)
					dsttl=npx->dsttl-1;
			}else
			{
				if(self->wfl&BTEWS_WFL_SDIR_RT)
					{ vct=0; vtr=1; }
			}

			if(d>0)
			{
				v/=d;
				ic/=d;
				w/=d;
			}

			if(vtr)
			{
				v=vct;
				w=0;
			}

//			if(wm==wn)w=w*(1.0-dt);
//			if(wm==wn)w=w*0.9;
			if(fabs(wm-wn)<0.001)w=w*0.9;
		
			self->wfl&=~BTEWS_WFL_SDIR_LFRT;
			self->wfl|=sd;
			self->dsttl=dsttl;
		
			self->vnx=v;	self->vpx=v;
//			self->inx=ic;	self->ipx=ic;

			self->pwnx=w;	self->pwpx=w;

			il=(vn>vm)?(w/(vn-vm)):0;
			self->icc1=(vn-vm)/self->ohm;
			if(self->icc1>il)self->icc1=il;

//			self->icc1=(self->icc1+ic)*0.5;
			
			self->watt1=(vn-vm)*self->icc1;
			self->pwatt1=wn;

			self->inx=self->icc1;	self->ipx=self->icc1;
		}else
		{
			npx=NULL;
			nnx=NULL;
			self->icc1=0;
			self->watt1=0;
			self->pwatt1=0;

			if(self->wfl&BTEWS_WFL_CN_UPDN)
			{
				v=(self->vny+self->vpy)*0.5;
				self->vnx=v;	self->vpx=v;
				self->inx=0;	self->ipx=0;
			}
		}

		if(self->wfl&BTEWS_WFL_CN_UPDN)
		{
			npy=BTEWS_GetNode(self->grid, self->x  , self->y+1);
			nny=BTEWS_GetNode(self->grid, self->x  , self->y-1);

			vm=999999; vn=-999999;
			im=999999; in=-999999;
			wm=999999; wn=-999999;
			v=0; d=0; ic=0; w=0; sd=0;
			dsttl=0;

#if 1
			f=(self->vny+self->vpy)*0.5;
			g=(self->iny+self->ipy)*0.5;
			h=(self->pwny+self->pwpy)*0.5;
			v+=f; ic+=g; d+=1; w+=h;

			vc0=f;
			vct=0; vtr=0;

			if(f<vm)vm=f;
			if(f>vn)vn=f;
			if(g<im)im=g;
			if(g>in)in=g;
			if(h<wm)wm=h;
			if(h>wn)wn=h;
#endif

			if(self->wfl&BTEWS_WFL_CN_UP)
			{
				f=npy->vny; v+=f; d+=1;
				g=npy->iny; ic+=g;
				h=npy->pwny; w+=h;
				if(f<vm)vm=f;
				if(f>vn) { vn=f; sd=BTEWS_WFL_SDIR_UP; }
				if(g<im)im=g;
				if(g>in)in=g;
				if(h<wm)wm=h;
				if(h>wn)wn=h;

				if((self->wfl&BTEWS_WFL_SDIR_UP) && (f<vc0))
					{ vct=npy->vny; vtr=1; }
				if(npy->dsttl>dsttl)
					dsttl=npy->dsttl-1;
			}else
			{
				if(self->wfl&BTEWS_WFL_SDIR_UP)
					{ vct=0; vtr=1; }
			}

			if(self->wfl&BTEWS_WFL_CN_DN)
			{
				f=nny->vpy; v+=f; d+=1;
				g=nny->ipy; ic+=g;
				h=nny->pwpy; w+=h;
				if(f<vm)vm=f;
				if(f>vn){ vn=f; sd=BTEWS_WFL_SDIR_DN; }
				if(g<im)im=g;
				if(g>in)in=g;
				if(h<wm)wm=h;
				if(h>wn)wn=h;

				if((self->wfl&BTEWS_WFL_SDIR_DN) && (f<vc0))
					{ vct=nny->vpy; vtr=1; }
				if(nny->dsttl>dsttl)
					dsttl=nny->dsttl-1;
			}else
			{
				if(self->wfl&BTEWS_WFL_SDIR_DN)
					{ vct=0; vtr=1; }
			}

			if(d>0)
			{
				v/=d;
				ic/=d;
				w/=d;
			}

			if(vtr)
			{
				v=vct;
				w=0;
			}
			
//			if(wm==wn)w=w*(1.0-dt);
			if(fabs(wm-wn)<0.001)w=w*0.9;
		
			self->wfl&=~BTEWS_WFL_SDIR_UPDN;
			self->wfl|=sd;
			self->dsttl=dsttl;
		
			self->vny=v;	self->vpy=v;
			self->pwny=w;	self->pwpy=w;

//			il=w/(vn-vm);
			il=(vn>vm)?(w/(vn-vm)):0;
			self->icc2=(vn-vm)/self->ohm;
			if(self->icc2>il)self->icc2=il;

//			self->icc2=(self->icc2+ic)*0.5;

			self->watt2=(vn-vm)*self->icc2;
			self->pwatt2=wn;

			self->iny=self->icc2;	self->ipy=self->icc2;
		}else
		{
			npy=NULL;
			nny=NULL;
			self->icc2=0;
			self->watt2=0;
			self->pwatt2=0;

			if(self->wfl&BTEWS_WFL_CN_LFRT)
			{
				v=(self->vnx+self->vpx)*0.5;
				self->vny=v;	self->vpy=v;
				self->iny=0;	self->ipy=0;
				self->pwny=0;	self->pwpy=0;
			}
		}

#if 1
		self->icc=fmax(self->icc1, self->icc2);
		self->watt=fmax(self->watt1, self->watt2);
		self->pwatt=fmax(self->pwatt1, self->pwatt2);
//		self->temp=(self->temp+self->watt*dt)-dt*((self->temp-25)/10.0);
		self->temp=self->temp+((1.0/0.004)*self->watt*dt*dt)-
			dt*((self->temp-25)/25.0);
#endif
	}
	
	btews_wire_UpdateAdjacent(self);
}

BTEIFGL_API void btews_wire_Place(BTEWS_Component *self,
	BTEWS_WireGrid *grid, int x, int y)
{
	BTEWS_WireNode *ncur;

	ncur=BTEWS_GetAllocNode(grid, x, y);
	ncur->Draw=btews_wire_Draw;
	ncur->Update=btews_wire_Update;
	ncur->Describe=btews_wire_Describe;
	ncur->wfl=3;
//	ncur->ohm=0.00022;
//	ncur->ohm=0.001;
	ncur->ohm=BTEWS_WIREOHM;
	ncur->temp=25;
	ncur->cty=BTEWS_CTY_WIRE;

//	ncur->cx=x;
//	ncur->cy=y;

	BTEWS_MarkNodeDirtyStar(grid, x, y);
}

BTEIFGL_API void btews_wire_PlaceDigital(BTEWS_Component *self,
	BTEWS_WireGrid *grid, int x, int y)
{
	BTEWS_WireNode *ncur;

	ncur=BTEWS_GetAllocNode(grid, x, y);
	ncur->Draw=btews_wire_Draw;
	ncur->Update=btews_wire_UpdateDigital;
	ncur->Describe=btews_wire_DescribeDigital;
	ncur->wfl=3;
//	ncur->ohm=0.00022;
//	ncur->ohm=0.001;
	ncur->ohm=BTEWS_WIREOHM;
	ncur->temp=25;
	ncur->cty=BTEWS_CTY_DGTWIRE;

//	ncur->cx=x;
//	ncur->cy=y;

	BTEWS_MarkNodeDirtyStar(grid, x, y);
}
