#include <bteifgl.h>

BTEIFGL_API void btews_diode_Update(BTEWS_WireNode *self, double dt)
{
	BTEWS_WireNode *npx, *nnx, *npy, *nny;
	double f, g, d, v, c, im, in, vm, vn;
	double v0, v1, v2, v3;
	double c0, c1, c2, c3;

	if((self->wfl&BTEWS_WFL_CN_LFRT)==BTEWS_WFL_CN_LFRT)
	{
		npx=BTEWS_GetNode(self->grid, self->x+1, self->y);
		nnx=BTEWS_GetNode(self->grid, self->x-1, self->y);

		vm=999999; vn=-999999;
		v0=nnx->vpx;
		v1=npx->vnx;
		if(v0<vm)vm=v0;
		if(v0>vn)vn=v0;
		if(v1<vm)vm=v1;
		if(v1>vn)vn=v1;
		v=(v0+v1)/2;
		self->vnx=v;	self->vpx=v;
		self->vny=v;	self->vpy=v;

		self->icc=(vn-vm)/self->ohm;
		self->watt=(vn-vm)*self->icc;
		
		d=self->icc*self->ohm;
		v2=v+d*0.4;
		v3=v-d*0.4;
		if(v0>v1)
			{ self->vnx=v2;	self->vpx=v3; }
		else
			{ self->vnx=v3;	self->vpx=v2; }
	}

	if((self->wfl&BTEWS_WFL_CN_UPDN)==BTEWS_WFL_CN_UPDN)
	{
		npy=BTEWS_GetNode(self->grid, self->x  , self->y+1);
		nny=BTEWS_GetNode(self->grid, self->x  , self->y-1);

		vm=999999; vn=-999999;

		if(npy->vny>(nny->vpy+0.8))
		{
			v0=nny->vpy;
			v1=npy->vny;
			c0=nny->ipy;
			c1=npy->iny;
			if(v0<vm)vm=v0;
			if(v0>vn)vn=v0;
			if(v1<vm)vm=v1;
			if(v1>vn)vn=v1;
			v=(v0+v1)/2;
			c=(c0+c1)/2;
		
			self->vnx=v;	self->vpx=v;
			self->vny=v;	self->vpy=v;

			c2=(vn-vm)/self->ohm;
//			c3=(c2+c)*0.5;
			c3=c2;
			self->icc=c3;

			self->inx=c3;	self->ipx=c3;
			self->iny=c3;	self->ipy=c3;

//			self->icc=(vn-vm)/self->ohm;
			self->watt=(vn-vm)*self->icc;


#if 1
			d=self->icc*self->ohm;

			v2=v+d*0.5;
			v3=v-d*0.5;
				
//			v2=v+d*0.4999;
//			v3=v-d*0.4999;

			if(self->icc>c)
			{
				f=(self->icc-c)/self->icc;
				f=(f+1);
//				f=f*f-1;
				f=f*f*f-1;
//				g=0.5-(10*f);
//				g=0.5-(100*f);
				g=0.5-(1000*f);
				if(g<0.35)g=0.35;
//				g=0.5-f;

				self->vc3=(self->vc3*0.999)+(g*0.001);
//				self->vc3=(self->vc3*0.999)+(0.49*0.01);
			}else
			{
				self->vc3=(self->vc3*0.999)+(0.50*0.001);
			}

			v2=v+d*self->vc3;
			v3=v-d*self->vc3;

			if(v2<vm)v2=vm; if(v2>vn)v2=vn;
			if(v3<vm)v3=vm; if(v3>vn)v3=vn;
			if(v0>v1)
				{ self->vny=v2;	self->vpy=v3; }
			else
				{ self->vny=v3;	self->vpy=v2; }
#endif
		}
	}

	self->temp=self->temp+((1.0/0.004)*self->watt*dt*dt)-
		dt*((self->temp-25)/25.0);

	btews_wire_UpdateAdjacent(self);
}

BTEIFGL_API void btews_diode_Draw(BTEWS_WireNode *self,
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

	s="images/diode0.png";

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

BTEIFGL_API void btews_diode_Place(BTEWS_Component *self,
	BTEWS_WireGrid *grid, int x, int y)
{
	BTEWS_WireNode *ncur;
	int i, j;

	for(i=0; i<5; i++)
	{
		j=BTEWS_CheckEmptyNode(grid, x, y+(i-2));
		if(!j)
			break;
	}
	
	if(i<5)return;

	for(i=0; i<5; i++)
	{
		ncur=BTEWS_GetAllocNode(grid, x, y+(i-2));
		ncur->Draw=btews_diode_Draw;
		ncur->Update=btews_wire_Update;
		ncur->wfl=3;
		ncur->ohm=BTEWS_WIREOHM;
		ncur->temp=25;

		if((i==0) || (i==4))
			{ ncur->wfl|=BTEWS_WFL_JOIN; }
		else
			{ ncur->wfl=BTEWS_WFL_VERT|BTEWS_WFL_CN_UPDN; }

		if(i==2)
		{
			ncur->Update=btews_diode_Update;
			ncur->ohm=1000;
		}

		ncur->cx=x;
		ncur->cy=y;
		
		ncur->cc_x=0;
		ncur->cc_y=i;
		ncur->cc_r=0;
	}
}
