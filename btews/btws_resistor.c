#include <bteifgl.h>

void btews_resistor_UpdatePin(BTEWS_WireNode *self, double dt)
{
	BTEWS_WireNode *npx, *nnx, *npy, *nny;
	int dgt;

	if((ncur->cc_y==1) ||
		(ncur->cc_y==3))
	{
		return;
	}
	
	if(ncur->cc_y==0)
	{
		npx=BTEWS_GetNode(self->grid, self->x+1, self->y  );
		nnx=BTEWS_GetNode(self->grid, self->x-1, self->y  );
		nny=BTEWS_GetNode(self->grid, self->x  , self->y-1);
		npy=NULL;
	}

	if(ncur->cc_y==4)
	{
		npx=BTEWS_GetNode(self->grid, self->x+1, self->y  );
		nnx=BTEWS_GetNode(self->grid, self->x-1, self->y  );
		npy=BTEWS_GetNode(self->grid, self->x  , self->y+1);
		nny=NULL;
	}

	dgt=0;
	if((self->wfl&BTEWS_WFL_CN_RT) && (npx->cty==BTEWS_CTY_DGTWIRE))dgt=1;
	if((self->wfl&BTEWS_WFL_CN_LF) && (nnx->cty==BTEWS_CTY_DGTWIRE))dgt=1;
	if((self->wfl&BTEWS_WFL_CN_UP) && (npy->cty==BTEWS_CTY_DGTWIRE))dgt=1;
	if((self->wfl&BTEWS_WFL_CN_DN) && (nny->cty==BTEWS_CTY_DGTWIRE))dgt=1;
	
	if(dgt)
	{
		btews_wire_UpdateDigital(self, dt);
	}else
	{
		btews_wire_Update(self, dt);
	}
}

void btews_resistor_Update(BTEWS_WireNode *self, double dt)
{
	BTEWS_WireNode *npx, *nnx, *npy, *nny;
	double f, g, d, v, c, im, in, il, vm, vn;
	double w, wm, wn, w0, w1;
	double v0, v1, v2, v3;
	double c0, c1, c2, c3;

	if((self->wfl&BTEWS_WFL_CN_LFRT)==BTEWS_WFL_CN_LFRT)
	{
		npx=BTEWS_GetNode(self->grid, self->x+2, self->y);
		nnx=BTEWS_GetNode(self->grid, self->x-2, self->y);

		vm=999999; vn=-999999;
		wm=999999; wn=-999999;

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
		npy=BTEWS_GetNode(self->grid, self->x  , self->y+2);
		nny=BTEWS_GetNode(self->grid, self->x  , self->y-2);

		vm=999999; vn=-999999;
		wm=999999; wn=-999999;

		v0=nny->vpy;
		v1=npy->vny;
		c0=nny->ipy;
		c1=npy->iny;
		w0=nny->pwpy;
		w1=npy->pwny;

		if(v0<vm)vm=v0;
		if(v0>vn)vn=v0;
		if(v1<vm)vm=v1;
		if(v1>vn)vn=v1;

		if(w0<wm)wm=w0;
		if(w0>wn)wn=w0;
		if(w1<wm)wm=w1;
		if(w1>wn)wn=w1;

		v=(v0+v1)*0.5;
		c=(c0+c1)*0.5;
		w=(w0+w1)*0.5;
		self->vnx=v;	self->vpx=v;
		self->vny=v;	self->vpy=v;

//		il=wn/(vn-vm);
		il=(vn>vm)?(wn/(vn-vm)):0;

		c2=(vn-vm)/self->ohm;
//		c3=(c2+c)*0.5;
		c3=c2;
		
		if(c3>il)c3=il;
		
		self->icc=c3;

		self->inx=c3;	self->ipx=c3;
		self->iny=c3;	self->ipy=c3;

//		self->icc=(vn-vm)/self->ohm;
		self->watt=(vn-vm)*self->icc;

		if(w0>w1)
		{
			w=self->watt;
			self->pwnx=w;	self->pwpx=w;
			self->pwny=w0;	self->pwpy=w;
		}else
		{
			w=self->watt;
			self->pwnx=w;	self->pwpx=w;
			self->pwny=w;	self->pwpy=w1;
		}

#if 1
		d=self->icc*self->ohm;

		v2=v+d*0.5;
		v3=v-d*0.5;
				
		if(self->icc>c)
		{
			f=(self->icc-c)/self->icc;
			f=(f+1);
			f=f*f*f-1;
			g=0.5-(1000*f);
			if(g<0.35)g=0.35;

			self->vc3=(self->vc3*0.999)+(g*0.001);
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

	self->temp=self->temp+((1.0/0.004)*self->watt*dt*dt)-
		dt*((self->temp-25)/25.0);

	btews_wire_UpdateAdjacent(self);
}

void btews_resistor_Draw(BTEWS_WireNode *self,
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

	s="images/resistor0.png";

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

BTEIFGL_API void btews_resistor_Place(BTEWS_Component *self,
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
		ncur->Draw=btews_resistor_Draw;
//		ncur->Update=btews_resistor_Update;
//		ncur->Update=btews_wire_Update;
		ncur->Update=btews_resistor_UpdatePin;
		ncur->wfl=3;
//		ncur->ohm=1000/5;
//		ncur->ohm=0.00022;
//		ncur->ohm=0.001;
		ncur->ohm=BTEWS_WIREOHM;
		ncur->temp=25;

		if((i==0) || (i==4))
			{ ncur->wfl|=BTEWS_WFL_JOIN; }
		else
			{ ncur->wfl=BTEWS_WFL_VERT|BTEWS_WFL_CN_UPDN; }

		if(i==2)
		{
			ncur->Update=btews_resistor_Update;
			ncur->ohm=1000;
		}

		ncur->cx=x;
		ncur->cy=y;
		
		ncur->cc_x=0;
		ncur->cc_y=i;
		ncur->cc_r=0;
	}
}
