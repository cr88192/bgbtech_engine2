#include <bteifgl.h>

BTEIFGL_API void btews_dippin_Update(BTEWS_WireNode *self, double dt)
{
	BTEWS_WireNode *npx, *nnx, *npy, *nny, *nc;
	double f, g, d, v, vm, vn, v0, v1;
	double ic, im, in;
	int i, j, k, n, m;

	npx=BTEWS_GetNode(self->grid, self->x+1, self->y  );
	nnx=BTEWS_GetNode(self->grid, self->x-1, self->y  );
//	npy=BTEWS_GetNode(self->grid, self->x  , self->y+1);
//	nny=BTEWS_GetNode(self->grid, self->x  , self->y-1);

	nc=BTEWS_GetNode(self->grid, self->cx, self->cy);
	m=(1<<(self->pin-1));

	v0=0; n=0;
	if(self->wfl&BTEWS_WFL_CN_LF)
		{ v0+=nnx->vpx; n++; }
	if(self->wfl&BTEWS_WFL_CN_RT)
		{ v0+=npx->vnx; n++; }

	if(n>0)
	{
		v0=v0/n;
	}else
	{
		v0=0.8;
	}

	if(self->pin==nc->pin_v0)
	{
		v0=(nc->vc0+v0)*0.5;
		nc->vc0=v0;
	}
	if(self->pin==nc->pin_v1)
	{
		v0=(nc->vc1+v0)*0.5;
		nc->vc1=v0;
	}
	if(self->pin==nc->pin_v2)
	{
		v0=(nc->vc2+v0)*0.5;
		nc->vc2=v0;
	}
	if(self->pin==nc->pin_v3)
	{
		v0=(nc->vc3+v0)*0.5;
		nc->vc3=v0;
	}

	vm=nc->vc3;
	vn=nc->vc0;
	v=(vm+vn)*0.5;

	if(!(nc->io_pdir&m))
	{	
		self->vpx=v0;
		self->vnx=v0;
		
		nc->io_ipin&=~m;
//		if(v0>1.2)
		if(v0>v)
			nc->io_ipin|=m;
	}else
	{
		v1=v0;
		if(nc->io_opin&m)
		{
//			if(v1<3.3)v1=3.3;
			if(v1<vn)v1=vn;
		}else
		{
//			if(v1>0.0)v1=0.0;
			if(v1>vm)v1=vm;
		}
		self->vpx=v1;
		self->vnx=v1;
		self->dsttl=4096;
	}

	btews_wire_UpdateAdjacent(self);
}

BTEIFGL_API void btews_dipcore_Update(BTEWS_WireNode *self, double dt)
{
	double vm, vn, v, v0, v1;
	double t;
	int i, j, k;

	vm=self->vc3;
	vn=self->vc0;
	v=(vm+vn)*0.5;
	
	v0=vm*(1-dt)+(v*dt);
	v1=vn*(1-dt)+(v*dt);
	if(v0<vm)v0=vm;
	if(v1>vn)v1=vn;
	self->vc3=v0;
	self->vc0=v1;
	
	self->io_pdir=0x0FFC;
	
	t=frgl_state->time_f*2;
	for(i=0; i<14; i++)
	{
		j=(int)(t+(i/14.0));
		if(j&1)
		{
			self->io_opin|=1<<i;
		}else
		{
			self->io_opin&=~(1<<i);
		}
	}
}

BTEIFGL_API void btews_dip14_Draw(BTEWS_WireNode *self,
	float x0, float y0, float x1, float y1)
{
	float s0, s1, t0, t1;
	float sf, tf;
	float v;
	char *s;
	int tex, txs, tys;

//	self->wfl|=BTEWS_WFL_JOIN;

	if(self->wfl&BTEWS_WFL_JOIN)
//	if(1)
	{
		btews_wire_Draw(self, x0, y0, x1, y1);
	}else
	{
		v=(self->vnx+self->vpx+self->vny+self->vpy)*0.25;
		BTEWS_Wire_ColorForVoltage(self, v);
	}

	s="images/base_dip14.png";

	tex=Tex_LoadFile(s, &txs, &tys);
	sf=64.0/txs;
	tf=64.0/tys;
	s0=self->cc_x*sf;
	t0=self->cc_y*tf;
	s1=s0+sf;
	t1=t0+tf;

	frglEnableTexture2D();
	frglBindTexture2D(tex);
	frglBegin(GL_QUADS);
	frglTexCoord2f(s0, t0);
	frglVertex2f(x0, y0);
	frglTexCoord2f(s1, t0);
	frglVertex2f(x1, y0);
	frglTexCoord2f(s1, t1);
	frglVertex2f(x1, y1);
	frglTexCoord2f(s0, t1);
	frglVertex2f(x0, y1);
	frglEnd();
}

BTEIFGL_API void btews_dip14_Place(BTEWS_Component *self,
	BTEWS_WireGrid *grid, int x, int y)
{
	BTEWS_WireNode *ncur;
	int i, j, k;

	for(i=0; i<7; i++)
		for(j=0; j<4; j++)
	{
		k=BTEWS_CheckEmptyNode(grid, x+(j-1), y+(i-3));
		if(!k)
			break;
	}
	
	if(i<7)return;

	for(i=0; i<7; i++)
		for(j=0; j<4; j++)
	{
		ncur=BTEWS_GetAllocNode(grid, x+(j-1), y+(i-3));
		ncur->Draw=btews_dip14_Draw;
//		ncur->Update=btews_resistor_Update;
		ncur->Update=btews_wire_Update;
//		ncur->wfl=BTEWS_WFL_HORZ|BTEWS_WFL_VERT;
//		ncur->wfl=BTEWS_WFL_HORZ;
		ncur->wfl=0;
//		ncur->ohm=1000/5;
//		ncur->ohm=0.00022;
//		ncur->ohm=0.001;
		ncur->ohm=BTEWS_WIREOHM;
		ncur->temp=25;

		ncur->pwnx=0.01;	ncur->pwpx=0.01;
		ncur->pwny=0.01;	ncur->pwpy=0.01;

		if((j==0) || (j==3))
		{
			ncur->wfl|=BTEWS_WFL_HORZ;
			ncur->wfl|=BTEWS_WFL_JOIN;
			ncur->Update=btews_dippin_Update;
		}

		if(i==1)
		{
//			ncur->wfl=BTEWS_WFL_HORZ;
//			ncur->Update=btews_transistor_Update;
//			ncur->ohm=0.001;
//			ncur->ohm=BTEWS_WIREOHM;
		}
		
		if((i==3) && (j==1))
		{
			ncur->Update=btews_dipcore_Update;
			ncur->pin_v0=1;
			ncur->pin_v3=14;
		}

		ncur->cx=x;
		ncur->cy=y;
		
		ncur->cc_x=j;
		ncur->cc_y=i;
		ncur->cc_r=0;
		ncur->cty=self->cty;
		
		if(j==0)
			{ ncur->pin=7-i; }
		if(j==3)
			{ ncur->pin=8+i; }
	}
}
