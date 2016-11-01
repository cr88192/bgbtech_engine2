/**
 * Stuff for floating-point vector types.
 */

// #include <bteifgl.h>

BGBDTC_X128 *bgbdt_xv128_freev3f;
BGBDTC_X128 *bgbdt_xv128_freev4f;
BGBDTC_X128 *bgbdt_xv128_freeqf;

static int objty_v3f=-1;
static int objty_v4f=-1;
static int objty_qf=-1;

BGBDTC_X128 *BGBDT_XV128_AllocVec3f(void)
{
	BGBDTC_X128 *xp;
	int i;
	
	xp=bgbdt_xv128_freev3f;
	if(xp)
	{
		bgbdt_xv128_freev3f=*(BGBDTC_X128 **)xp;
		return(xp);
	}

	if(objty_v3f<0)
		{ objty_v3f=BGBDT_MM_GetIndexObjTypeName("_vec3f_t"); }

	xp=dtmAlloc("_vec3f_t", 128*sizeof(BGBDTC_X128));
	
	for(i=0; i<127; i++)
	{
		*(BGBDTC_X128 **)xp=bgbdt_xv128_freev3f;
		bgbdt_xv128_freev3f=xp;
		xp++;
	}
	return(xp);
}

BGBDTC_X128 *BGBDT_XV128_AllocVec4f(void)
{
	BGBDTC_X128 *xp;
	int i;
	
	xp=bgbdt_xv128_freev4f;
	if(xp)
	{
		bgbdt_xv128_freev4f=*(BGBDTC_X128 **)xp;
		return(xp);
	}

	if(objty_v4f<0)
		{ objty_v4f=BGBDT_MM_GetIndexObjTypeName("_vec4f_t"); }

	xp=dtmAlloc("_vec4f_t", 128*sizeof(BGBDTC_X128));
	
	for(i=0; i<127; i++)
	{
		*(BGBDTC_X128 **)xp=bgbdt_xv128_freev4f;
		bgbdt_xv128_freev4f=xp;
		xp++;
	}
	return(xp);
}

BGBDTC_X128 *BGBDT_XV128_AllocQuatf(void)
{
	BGBDTC_X128 *xp;
	int i;
	
	xp=bgbdt_xv128_freeqf;
	if(xp)
	{
		bgbdt_xv128_freeqf=*(BGBDTC_X128 **)xp;
		return(xp);
	}

	if(objty_qf<0)
		{ objty_qf=BGBDT_MM_GetIndexObjTypeName("_quatf_t"); }

	xp=dtmAlloc("_quatf_t", 128*sizeof(BGBDTC_X128));
	
	for(i=0; i<127; i++)
	{
		*(BGBDTC_X128 **)xp=bgbdt_xv128_freeqf;
		bgbdt_xv128_freeqf=xp;
		xp++;
	}
	return(xp);
}

void BGBDT_XV128_FreeVec3f(BGBDTC_X128 *xp)
{
	*(BGBDTC_X128 **)xp=bgbdt_xv128_freev3f;
	bgbdt_xv128_freev3f=xp;
}

void BGBDT_XV128_FreeVec4f(BGBDTC_X128 *xp)
{
	*(BGBDTC_X128 **)xp=bgbdt_xv128_freev4f;
	bgbdt_xv128_freev4f=xp;
}

void BGBDT_XV128_FreeQuatf(BGBDTC_X128 *xp)
{
	*(BGBDTC_X128 **)xp=bgbdt_xv128_freeqf;
	bgbdt_xv128_freeqf=xp;
}


BS2VM_API dtVal BGBDT_XV128_WrapVec3fv(float *fv)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XV128_AllocVec3f();
	xp->x=fv[0];	xp->y=fv[1];
	xp->z=fv[2];	xp->w=0;
	return(dtvWrapTagPtrF(xp, objty_v3f));
}

BS2VM_API dtVal BGBDT_XV128_WrapVec3f(float x, float y, float z)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XV128_AllocVec3f();
	xp->x=x;	xp->y=y;
	xp->z=z;	xp->w=0;
	return(dtvWrapTagPtrF(xp, objty_v3f));
}

BS2VM_API dtVal BGBDT_XV128_WrapVec4fv(float *fv)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XV128_AllocVec4f();
	xp->x=fv[0];	xp->y=fv[1];
	xp->z=fv[2];	xp->w=fv[4];
	return(dtvWrapTagPtrF(xp, objty_v4f));
}

BS2VM_API dtVal BGBDT_XV128_WrapVec4f(float x, float y, float z, float w)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XV128_AllocVec4f();
	xp->x=x;	xp->y=y;
	xp->z=z;	xp->w=w;
	return(dtvWrapTagPtrF(xp, objty_v4f));
}

BS2VM_API dtVal BGBDT_XV128_WrapQuatfv(float *fv)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XV128_AllocQuatf();
	xp->x=fv[0];	xp->y=fv[1];
	xp->z=fv[2];	xp->w=fv[4];
	return(dtvWrapTagPtrF(xp, objty_qf));
}

BS2VM_API dtVal BGBDT_XV128_WrapQuatf(float x, float y, float z, float w)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XV128_AllocQuatf();
	xp->x=x;	xp->y=y;
	xp->z=z;	xp->w=w;
	return(dtvWrapTagPtrF(xp, objty_qf));
}

BS2VM_API int BGBDT_XV128_IsVec3fP(dtVal objv)
	{ return(dtvCheckPtrTagP(objv, objty_v3f)); }
BS2VM_API int BGBDT_XV128_IsVec4fP(dtVal objv)
	{ return(dtvCheckPtrTagP(objv, objty_v4f)); }
BS2VM_API int BGBDT_XV128_IsQuatfP(dtVal objv)
	{ return(dtvCheckPtrTagP(objv, objty_qf)); }

BS2VM_API void BGBDT_XV128_GetVec3fv(dtVal vec, float *fv)
{
	BGBDTC_X128 *xp;
	
	xp=dtvUnwrapPtrF(vec);
	fv[0]=xp->x; fv[1]=xp->y;
	fv[2]=xp->z;
}

BS2VM_API void BGBDT_XV128_GetVec4fv(dtVal vec, float *fv)
{
	BGBDTC_X128 *xp;
	
	xp=dtvUnwrapPtrF(vec);
	fv[0]=xp->x; fv[1]=xp->y;
	fv[2]=xp->z; fv[3]=xp->w;
}

BS2VM_API void BGBDT_XV128_GetQuatfv(dtVal vec, float *fv)
{
	BGBDTC_X128 *xp;
	
	xp=dtvUnwrapPtrF(vec);
	fv[0]=xp->x; fv[1]=xp->y;
	fv[2]=xp->z; fv[3]=xp->w;
}


dtVal BGBDT_XV128_Add(dtVal a, dtVal b)
{
	float tva[4], tvb[4], tvc[4];
	dtVal c;

	if(dtvIsCplxfP(a) && dtvIsCplxfP(b))
	{
		dtvUnwrapCplxfv(a, tva);
		dtvUnwrapCplxfv(b, tvb);
//		V2F_ADD(tva, tvb, tvc);
		tvc[0]=tva[0]+tvb[0];
		tvc[1]=tva[1]+tvb[1];
		c=dtvWrapCplxfv(tvc);
		return(c);
	}

	if(dtvIsVec2fP(a) && dtvIsVec2fP(b))
	{
		dtvUnwrapVec2fv(a, tva);
		dtvUnwrapVec2fv(b, tvb);
//		V2F_ADD(tva, tvb, tvc);
		tvc[0]=tva[0]+tvb[0];
		tvc[1]=tva[1]+tvb[1];
		c=dtvWrapVec2fv(tvc);
		return(c);
	}

	if(BGBDT_XV128_IsVec3fP(a) && BGBDT_XV128_IsVec3fP(b))
	{
		BGBDT_XV128_GetVec3fv(a, tva);
		BGBDT_XV128_GetVec3fv(b, tvb);
//		V3F_ADD(tva, tvb, tvc);
		tvc[0]=tva[0]+tvb[0];
		tvc[1]=tva[1]+tvb[1];
		tvc[2]=tva[2]+tvb[2];
		c=BGBDT_XV128_WrapVec3fv(tvc);
		return(c);
	}

	if(BGBDT_XV128_IsVec4fP(a) && BGBDT_XV128_IsVec4fP(b))
	{
		BGBDT_XV128_GetVec4fv(a, tva);
		BGBDT_XV128_GetVec4fv(b, tvb);
//		V4F_ADD(tva, tvb, tvc);
		tvc[0]=tva[0]+tvb[0];	tvc[1]=tva[1]+tvb[1];
		tvc[2]=tva[2]+tvb[2];	tvc[3]=tva[3]+tvb[3];
		c=BGBDT_XV128_WrapVec4fv(tvc);
		return(c);
	}
	
	if(BGBDT_XV128_IsQuatfP(a) && BGBDT_XV128_IsQuatfP(b))
	{
		BGBDT_XV128_GetQuatfv(a, tva);
		BGBDT_XV128_GetQuatfv(b, tvb);
		tvc[0]=tva[0]+tvb[0];	tvc[1]=tva[1]+tvb[1];
		tvc[2]=tva[2]+tvb[2];	tvc[3]=tva[3]+tvb[3];
		c=BGBDT_XV128_WrapQuatfv(tvc);
		return(c);
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XV128_Sub(dtVal a, dtVal b)
{
	float tva[4], tvb[4], tvc[4];
	dtVal c;

	if(dtvIsCplxfP(a) && dtvIsCplxfP(b))
	{
		dtvUnwrapCplxfv(a, tva);
		dtvUnwrapCplxfv(b, tvb);
//		V2F_SUB(tva, tvb, tvc);
		tvc[0]=tva[0]-tvb[0];	tvc[1]=tva[1]-tvb[1];
//		tvc[2]=tva[2]-tvb[2];	tvc[3]=tva[3]-tvb[3];
		c=dtvWrapCplxfv(tvc);
		return(c);
	}

	if(dtvIsVec2fP(a) && dtvIsVec2fP(b))
	{
		dtvUnwrapVec2fv(a, tva);
		dtvUnwrapVec2fv(b, tvb);
//		V2F_SUB(tva, tvb, tvc);
		tvc[0]=tva[0]-tvb[0];	tvc[1]=tva[1]-tvb[1];
//		tvc[2]=tva[2]-tvb[2];	tvc[3]=tva[3]-tvb[3];
		c=dtvWrapVec2fv(tvc);
		return(c);
	}

	if(BGBDT_XV128_IsVec3fP(a) && BGBDT_XV128_IsVec3fP(b))
	{
		BGBDT_XV128_GetVec3fv(a, tva);
		BGBDT_XV128_GetVec3fv(b, tvb);
//		V3F_SUB(tva, tvb, tvc);
		tvc[0]=tva[0]-tvb[0];	tvc[1]=tva[1]-tvb[1];
		tvc[2]=tva[2]-tvb[2];	// tvc[3]=tva[3]-tvb[3];
		c=BGBDT_XV128_WrapVec3fv(tvc);
		return(c);
	}

	if(BGBDT_XV128_IsVec4fP(a) && BGBDT_XV128_IsVec4fP(b))
	{
		BGBDT_XV128_GetVec4fv(a, tva);
		BGBDT_XV128_GetVec4fv(b, tvb);
//		V4F_SUB(tva, tvb, tvc);
		tvc[0]=tva[0]-tvb[0];	tvc[1]=tva[1]-tvb[1];
		tvc[2]=tva[2]-tvb[2];	tvc[3]=tva[3]-tvb[3];
		c=BGBDT_XV128_WrapVec4fv(tvc);
		return(c);
	}
	
	if(BGBDT_XV128_IsQuatfP(a) && BGBDT_XV128_IsQuatfP(b))
	{
		BGBDT_XV128_GetQuatfv(a, tva);
		BGBDT_XV128_GetQuatfv(b, tvb);
//		V4F_SUB(tva, tvb, tvc);
		tvc[0]=tva[0]-tvb[0];	tvc[1]=tva[1]-tvb[1];
		tvc[2]=tva[2]-tvb[2];	tvc[3]=tva[3]-tvb[3];
		c=BGBDT_XV128_WrapQuatfv(tvc);
		return(c);
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XV128_Mul(dtVal a, dtVal b)
{
	float tva[4], tvb[4], tvc[4];
	dtVal c;

	if(dtvIsCplxfP(a) && dtvIsCplxfP(b))
	{
		dtvUnwrapCplxfv(a, tva);
		dtvUnwrapCplxfv(b, tvb);
//		V2F_SUB(tva, tvb, tvc);
		tvc[0]=(tva[0]*tvb[0])-(tva[1]*tvb[1]);
		tvc[1]=(tva[0]*tvb[1])+(tva[1]*tvb[0]);
		c=dtvWrapCplxfv(tvc);
		return(c);
	}

	if(dtvIsVec2fP(a) && dtvIsVec2fP(b))
	{
		dtvUnwrapVec2fv(a, tva);
		dtvUnwrapVec2fv(b, tvb);
//		V2F_MUL(tva, tvb, tvc);
		tvc[0]=tva[0]*tvb[0];	tvc[1]=tva[1]*tvb[1];
//		tvc[2]=tva[2]*tvb[2];	tvc[3]=tva[3]*tvb[3];
		c=dtvWrapVec2fv(tvc);
		return(c);
	}

	if(BGBDT_XV128_IsVec3fP(a) && BGBDT_XV128_IsVec3fP(b))
	{
		BGBDT_XV128_GetVec3fv(a, tva);
		BGBDT_XV128_GetVec3fv(b, tvb);
//		V3F_MUL(tva, tvb, tvc);
		tvc[0]=tva[0]*tvb[0];	tvc[1]=tva[1]*tvb[1];
		tvc[2]=tva[2]*tvb[2];	// tvc[3]=tva[3]*tvb[3];
		c=BGBDT_XV128_WrapVec3fv(tvc);
		return(c);
	}

	if(BGBDT_XV128_IsVec4fP(a) && BGBDT_XV128_IsVec4fP(b))
	{
		BGBDT_XV128_GetVec4fv(a, tva);
		BGBDT_XV128_GetVec4fv(b, tvb);
//		V4F_MUL(tva, tvb, tvc);
		tvc[0]=tva[0]*tvb[0];	tvc[1]=tva[1]*tvb[1];
		tvc[2]=tva[2]*tvb[2];	tvc[3]=tva[3]*tvb[3];
		c=BGBDT_XV128_WrapVec4fv(tvc);
		return(c);
	}
	
	if(BGBDT_XV128_IsQuatfP(a) && BGBDT_XV128_IsQuatfP(b))
	{
		BGBDT_XV128_GetQuatfv(a, tva);
		BGBDT_XV128_GetQuatfv(b, tvb);
		tvc[0]=	(tva[3]*tvb[0])+(tva[0]*tvb[3])+
				(tva[1]*tvb[2])-(tva[2]*tvb[1]);
		tvc[1]=	(tva[3]*tvb[1])+(tva[1]*tvb[3])+
				(tva[2]*tvb[0])-(tva[0]*tvb[2]);
		tvc[2]=	(tva[3]*tvb[2])+(tva[2]*tvb[3])+
				(tva[0]*tvb[1])-(tva[1]*tvb[0]);
		tvc[3]=	(tva[3]*tvb[3])-(tva[0]*tvb[0])-
				(tva[1]*tvb[1])-(tva[2]*tvb[2]);
		c=BGBDT_XV128_WrapQuatfv(tvc);
		return(c);
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XV128_Div(dtVal a, dtVal b)
{
	float tva[4], tvb[4], tvc[4], tvr[4];
	float f, g, h;
	dtVal c;

	if(dtvIsCplxfP(a) && dtvIsCplxfP(b))
	{
		dtvUnwrapCplxfv(a, tva);
		dtvUnwrapCplxfv(b, tvb);
		h=1.0/(tvb[0]*tvb[0]+tvb[1]*tvb[1]);
		tvc[0]=((tva[0]*tvb[0])+(tva[1]*tvb[1]))*h;
		tvc[1]=((tva[1]*tvb[0])-(tva[0]*tvb[1]))*h;
		c=dtvWrapCplxfv(tvc);
		return(c);
	}

	if(dtvIsVec2fP(a) && dtvIsVec2fP(b))
	{
		dtvUnwrapVec2fv(a, tva);
		dtvUnwrapVec2fv(b, tvb);
//		V2F_DIV(tva, tvb, tvc);
		tvc[0]=tva[0]/tvb[0];	tvc[1]=tva[1]/tvb[1];
//		tvc[2]=tva[2]/tvb[2];	tvc[3]=tva[3]/tvb[3];
		c=dtvWrapVec2fv(tvc);
		return(c);
	}

	if(BGBDT_XV128_IsVec3fP(a) && BGBDT_XV128_IsVec3fP(b))
	{
		BGBDT_XV128_GetVec3fv(a, tva);
		BGBDT_XV128_GetVec3fv(b, tvb);
//		V3F_DIV(tva, tvb, tvc);
		tvc[0]=tva[0]/tvb[0];	tvc[1]=tva[1]/tvb[1];
		tvc[2]=tva[2]/tvb[2];	// tvc[3]=tva[3]/tvb[3];
		c=BGBDT_XV128_WrapVec3fv(tvc);
		return(c);
	}

	if(BGBDT_XV128_IsVec4fP(a) && BGBDT_XV128_IsVec4fP(b))
	{
		BGBDT_XV128_GetVec4fv(a, tva);
		BGBDT_XV128_GetVec4fv(b, tvb);
//		V4F_DIV(tva, tvb, tvc);
		tvc[0]=tva[0]/tvb[0];	tvc[1]=tva[1]/tvb[1];
		tvc[2]=tva[2]/tvb[2];	tvc[3]=tva[3]/tvb[3];
		c=BGBDT_XV128_WrapVec4fv(tvc);
		return(c);
	}
	
	if(BGBDT_XV128_IsQuatfP(a) && BGBDT_XV128_IsQuatfP(b))
	{
		BGBDT_XV128_GetQuatfv(a, tva);
		BGBDT_XV128_GetQuatfv(b, tvb);

		f=	(tvb[0]*tvb[0])+(tvb[1]*tvb[1])+
			(tvb[2]*tvb[2])+(tvb[3]*tvb[3]);
		g=1.0/f;
		tvr[0]=-tvb[0]*g;	tvr[1]=-tvb[1]*g;
		tvr[2]=-tvb[2]*g;	tvr[3]= tvb[3]*g;
		tvc[0]=	(tva[3]*tvr[0])+(tva[0]*tvr[3])+
				(tva[1]*tvr[2])-(tva[2]*tvr[1]);
		tvc[1]=	(tva[3]*tvr[1])+(tva[1]*tvr[3])+
				(tva[2]*tvr[0])-(tva[0]*tvr[2]);
		tvc[2]=	(tva[3]*tvr[2])+(tva[2]*tvr[3])+
				(tva[0]*tvr[1])-(tva[1]*tvr[0]);
		tvc[3]=	(tva[3]*tvr[3])-(tva[0]*tvr[0])-
				(tva[1]*tvr[1])-(tva[2]*tvr[2]);

		c=BGBDT_XV128_WrapQuatfv(tvc);
		return(c);
	}
	
	return(DTV_UNDEFINED);
}
