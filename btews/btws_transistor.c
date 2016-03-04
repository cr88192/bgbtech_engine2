#include <bteifgl.h>

BTEIFGL_API void btews_transistor_Update(BTEWS_WireNode *self, double dt)
{
	BTEWS_WireNode *npx, *nnx, *npy, *nny;
	double f, g, d, v, vm, vn, v0, v1;
	double ic, im, in;

	npx=BTEWS_GetNode(self->grid, self->x+1, self->y  );
	nnx=BTEWS_GetNode(self->grid, self->x-1, self->y  );
	npy=BTEWS_GetNode(self->grid, self->x  , self->y+1);
	nny=BTEWS_GetNode(self->grid, self->x  , self->y-1);

	v=0; d=0; ic=0;
	vm=999999; vn=-999999;
	im=999999; in=-999999;
	if(self->wfl&BTEWS_WFL_CN_LF)
	{
		f=nnx->vpx; v+=f; d+=1;
		g=nnx->ipx; ic+=g;
		if(f<vm)vm=f;
		if(f>vn)vn=f;
		if(g<im)im=g;
		if(g>in)in=g;
	}

	if(self->wfl&BTEWS_WFL_CN_RT)
	{
		f=npx->vnx; v+=f; d+=1;
		g=npx->inx; ic+=g;
		if(f<vm)vm=f;
		if(f>vn)vn=f;
		if(g<im)im=g;
		if(g>in)in=g;
	}

	if(d>0)
		{ v/=d; ic/=d; }
		
	self->vnx=v;	self->vpx=v;
	self->vny=v;	self->vpy=v;
	self->inx=ic;	self->ipx=ic;
	self->iny=ic;	self->ipy=ic;
	
	if( ((self->cty==BTEWS_CTY_NPN) ||
			(self->cty==BTEWS_CTY_NPNDAR) ||
			(self->cty==BTEWS_CTY_NMOS)) &&
			(npy->vny>(nny->vpy+0.8)) ||
		((self->cty==BTEWS_CTY_PNP) ||
			(self->cty==BTEWS_CTY_PNPDAR) ||
			(self->cty==BTEWS_CTY_PMOS)) &&
			(npy->vny<(nny->vpy-0.8))
			)
	{
		v0=(v+nny->vpy)*0.5;

#if 1
		if((self->cty!=BTEWS_CTY_NMOS) &&
			(self->cty!=BTEWS_CTY_PMOS))
		{
			v1=npy->vny*0.999+nny->vpy*0.001;
//			v1=npy->vny*0.99+nny->vpy*0.01;
//			v1=0;

			npy->vnx=v1;	npy->vpx=v1;
			npy->vny=v1;	npy->vpy=v1;
			npy->inx=0.1;	npy->ipx=0.1;
			npy->iny=0.1;	npy->ipy=0.1;
		}
#endif	

		switch(self->cty)
		{
		case BTEWS_CTY_NPN:
			g=100*(npy->vny-(nny->vpy+0.8));
			break;
		case BTEWS_CTY_PNP:
			g=100*(-(npy->vny-(nny->vpy-0.8)));
			break;
		case BTEWS_CTY_NPNDAR:
		case BTEWS_CTY_PNPDAR:
			g=1000;
			break;
		case BTEWS_CTY_NMOS:
		case BTEWS_CTY_PMOS:
			g=10000;
			break;
		}

		if(g<0.0)g=0.0;
		if(g>1.0)g=1.0;

		v1=(1.0-g)*v+v0*g;
		self->vnx=v1;	self->vpx=v1;
		self->vny=v1;	self->vpy=v1;

		v1=(1.0-g)*nny->vpy+v0*g;
		nny->vnx=v1;	nny->vpx=v1;
		nny->vny=v1;	nny->vpy=v1;

//		self->vnx=v0;	self->vpx=v0;
//		self->vny=v0;	self->vpy=v0;

//		nny->vnx=v0;	nny->vpx=v0;
//		nny->vny=v0;	nny->vpy=v0;
	}else
	{
		if((self->cty!=BTEWS_CTY_NMOS) &&
			(self->cty!=BTEWS_CTY_PMOS))
		{
//			v1=npy->vny*0.9999+nny->vpy*0.0001;
			v1=npy->vny*0.999+nny->vpy*0.001;

			npy->vnx=v1;	npy->vpx=v1;
			npy->vny=v1;	npy->vpy=v1;
			npy->inx=0.1;	npy->ipx=0.1;
			npy->iny=0.1;	npy->ipy=0.1;
		}
	}
	
	if(vn>=vm)
	{
		self->icc=(vn-vm)/self->ohm;
		
//		self->icc=(self->icc+ic)*0.5;
		self->inx=self->icc;
		self->ipx=self->icc;
		self->iny=self->icc;
		self->ipy=self->icc;
			
		self->watt=(vn-vm)*self->icc;
	}else
	{
		self->icc=0;
		self->watt=0;
		self->temp=25;
	}

	btews_wire_UpdateAdjacent(self);
}

BTEIFGL_API void btews_transistor_Draw(BTEWS_WireNode *self,
	float x0, float y0, float x1, float y1)
{
	float s0, s1, t0, t1;
	float sf, tf;
	float v;
	char *s;
	int tex, txs, tys;

	self->wfl|=BTEWS_WFL_JOIN;

//	if(self->wfl&BTEWS_WFL_JOIN)
	if(1)
	{
		btews_wire_Draw(self, x0, y0, x1, y1);
	}else
	{
		v=(self->vnx+self->vpx+self->vny+self->vpy)*0.25;
		BTEWS_Wire_ColorForVoltage(self, v);
	}

	switch(self->cc_y)
	{
	case 0:
		self->wfl&=~BTEWS_WFL_CN_UP;
		break;
	case 1:
		self->wfl&=~BTEWS_WFL_CN_UP;
		self->wfl&=~BTEWS_WFL_CN_DN;
		break;
	case 2:
		self->wfl&=~BTEWS_WFL_CN_DN;
		break;
	}

	switch(self->cty)
	{
	case BTEWS_CTY_NPN:
		s="images/bjt_to220_npn1.png";
		break;
	case BTEWS_CTY_PNP:
		s="images/bjt_to220_pnp1.png";
		break;
	case BTEWS_CTY_NPNDAR:
		s="images/bjt_to220_npndar1.png";
		break;
	case BTEWS_CTY_PNPDAR:
		s="images/bjt_to220_pnpdar1.png";
		break;
	case BTEWS_CTY_NMOS:
		s="images/bjt_to220_nmos1.png";
		break;
	case BTEWS_CTY_PMOS:
		s="images/bjt_to220_pmos1.png";
		break;

	default:
		s="images/bjt_to220_npn1.png";
		break;
	}

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

BTEIFGL_API void btews_transistor_Place(BTEWS_Component *self,
	BTEWS_WireGrid *grid, int x, int y)
{
	BTEWS_WireNode *ncur;
	int i, j;

	for(i=0; i<3; i++)
	{
		j=BTEWS_CheckEmptyNode(grid, x, y+(i-1));
		if(!j)
			break;
	}
	
	if(i<3)return;

	for(i=0; i<3; i++)
	{
		ncur=BTEWS_GetAllocNode(grid, x, y+(i-1));
		ncur->Draw=btews_transistor_Draw;
//		ncur->Update=btews_resistor_Update;
		ncur->Update=btews_wire_Update;
		ncur->wfl=BTEWS_WFL_HORZ|BTEWS_WFL_VERT;
//		ncur->ohm=1000/5;
//		ncur->ohm=0.00022;
//		ncur->ohm=0.001;
		ncur->ohm=BTEWS_WIREOHM;
		ncur->temp=25;

		if(i==1)
		{
			ncur->wfl=BTEWS_WFL_HORZ;
			ncur->Update=btews_transistor_Update;
//			ncur->ohm=0.001;
			ncur->ohm=BTEWS_WIREOHM;
		}

		ncur->cx=x;
		ncur->cy=y;
		
		ncur->cc_x=0;
		ncur->cc_y=i;
		ncur->cc_r=0;
		ncur->cty=self->cty;
	}
}
