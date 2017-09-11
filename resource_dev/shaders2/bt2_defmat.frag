// uniform mat4 pgl_ModelViewMatrix;
// uniform mat4 pgl_ProjectionMatrix;

// uniform vec2 scaleBias;
uniform sampler2D texBase;
uniform sampler2D texNorm;
// uniform sampler2D texGloss;
// uniform sampler2D texGlow;

uniform float time;
uniform float exposure;

// uniform bool texIsUVAY;
varying vec3 lorg, eyeOrg, eyeVec;
varying vec4 extColor;
// varying vec4 position;
varying vec2 texCoord;

varying vec3 point, normal, normal_x, normal_y;
varying vec3 eye_x, eye_y, eye_z;
// varying vec3 eyeVec;
varying mat3 eyeMat;

//   1.0/17.0,  9.0/17.0,  3.0/17.0, 11.0/17.0,
//   3.0/17.0,  5.0/17.0, 15.0/17.0,  7.0/17.0,
//   4.0/17.0, 12.0/17.0,  2.0/17.0, 10.0/17.0,
//  16.0/17.0,  8.0/17.0, 14.0/17.0,  6.0/17.0
      
//const mat4 bayer4m=mat4(
//	vec4( 1.0/17.0,  9.0/17.0,  3.0/17.0, 11.0/17.0),
//	vec4( 3.0/17.0,  5.0/17.0, 15.0/17.0,  7.0/17.0),
//	vec4( 4.0/17.0, 12.0/17.0,  2.0/17.0, 10.0/17.0),
//	vec4(16.0/17.0,  8.0/17.0, 14.0/17.0,  6.0/17.0));

const mat4 bayer4m=mat4(
	vec4( 0.06, 0.53, 0.18, 0.65),
	vec4( 0.18, 0.29, 0.88, 0.41),
	vec4( 0.24, 0.71, 0.12, 0.59),
	vec4( 0.94, 0.47, 0.82, 0.35));

//const mat4 bayer4m=mat4(
//	vec4(  1*0.06,  6*0.06, 10*0.06, 3*0.06),
//	vec4( 11*0.06, 15*0.06, 14*0.06, 9*0.06),
//	vec4(  7*0.06, 16*0.06, 13*0.06, 5*0.06),
//	vec4(  4*0.06, 12*0.06,  8*0.06, 2*0.06));

//const mat4 bayer4m=mat4(
//	vec4( 1*0.06, 1*0.06, 1*0.06, 1*0.06),
//	vec4( 1*0.06, 1*0.06, 1*0.06, 1*0.06),
//	vec4( 1*0.06, 1*0.06, 1*0.06, 1*0.06),
//	vec4( 1*0.06, 1*0.06, 1*0.06, 1*0.06));

float spow(float x, float y)
{
	if(x<0.0)
		return -pow(-x, y);
	return(pow(x, y));
}

/*
float hfbayer4(vec4 v)
{
	int ix, iy;
	ix=(int(v.x))&3;
	iy=(int(v.y))&3;
	return(bayer4m[iy][ix]);
}
*/

/*
float hfnoise(vec4 v)
{
	int nx, ny, nz, nt, h;
	int i;
	float f;
//	nx=int(v.x*1024.0);
//	ny=int(v.y*1024.0);
//	nz=int(v.z*1024.0);
//	nx=int(v.x*4194304.0);
//	ny=int(v.y*4194304.0);
//	nz=int(v.z*4194304.0);
	nx=int(v.x*65521.0);
	ny=int(v.y*65521.0);
	nz=int(v.z*65521.0);
	
//	nt=int(time*1021.0);
//	nt=nt*251^nt;
	
//	h=(((nx*251+ny)*251+nz)*251+nx);
	h=((nx*251^ny)*251^nz)*251;
//	h=(((nx*251^ny)*251^nz)*251^nt)*251;
	i=(h>>8)&4095;
	f=float(i)/4096.0;
//	return(2*f-1);
	return(f);
}
*/

float hfign(vec4 v)
{
	return(fract(52.9829189*fract(0.06711056*v.x+0.00583715*v.y)));
}


float btge_scurve(float a)
	{ return(a*a*(3.0-2.0*a)); }

void main()
{
	vec2 srcST, texST, baseST;
	vec2 midST, dmidST;
	vec2 texST1, texST2, ofsST;
	vec3 eye, norm, aux, ldir;
	vec4 rgba, glow, npix, spec;
	vec4 hgofs;
	float height, dist, ndotl, ndoth, spe;
	float hgign;
	float eyedist;
	float v, a;
	float h1, h2, v1, v2;
	float tsx, tbx, tsy, tby;
	float tcx, tcy, tcw, mip;
	int xfval;

	aux=lorg-point;
	dist=length(aux);
	ldir=aux/dist;

//	srcST=gl_TexCoord[0].xy;
	srcST=texCoord;

	npix=texture2D(texNorm, srcST);
	xfval=0;
//	hgofs=vec4(0.0, 0.0, 0.0, 0.0);

	if((npix.a>=(240.0/255.0)) && (npix.a<=(252.0/255.0)))
	{
		xfval=int(npix.a*255.0+0.5);
		npix=vec4(0.5, 0.5, 1.0, 0.5);
		
//		if(xfval==248)
		if(true)
		{
			v=time+point.x+point.y+point.z;
//			h1=sin(time);
//			h2=cos(time);
			h1=sin(v);
			h2=cos(v);
			ofsST=vec2(h1, h2)*(0.2/16.0);

//			hgofs=vec4(h1*4, h2*4, 0.0, 0.0);
//			hgign=hfbayer4(gl_FragCoord+hgofs);
//			hgign=hfign(gl_FragCoord);
		}
	}else
	{
		ofsST=vec2(0.0, 0.0);
//		hgign=hfign(gl_FragCoord);
	}

	height=npix.a;
//	v=height*0.04-0.03;
//	v=height*0.0025-0.001875;
	v=height*0.0025-0.00125;

//	eye=normalize(eyeVec);
	eye=normalize((eyeOrg-point)*eyeMat);
//	eye=normalize(((eyeOrg+eye_z)-point)*eyeMat);
//	eye=normalize(((eyeOrg+(100*eye_z))-point)*eyeMat);

	eyedist=length(eyeOrg-point);

	baseST=srcST+(eye.xy*v)+ofsST;
	
//	if(int(baseST.x*16.0)!=int(srcST.x*16.0))
//		baseST=vec2(srcST.x, baseST.y);
//	if(int(baseST.y*16.0)!=int(srcST.y*16.0))
//		baseST=vec2(baseST.x, srcST.y);

	tsx=floor(srcST.x*16.0);
	tsy=floor(srcST.y*16.0);
	tbx=floor(baseST.x*16.0);
	tby=floor(baseST.y*16.0);
	baseST=baseST+(vec2(tsx-tbx, tsy-tby)*(1.0/16.0));

//	tcw=clamp(-gl_FragCoord.w, 0.0, 1.0);
//	tcw=clamp((gl_FragCoord.z-0.99)*100.0, 0.0, 1.0);
//	tcw=clamp((gl_FragCoord.z-0.98)*10.0, 0.0, 1.0);
//	midST=vec2((tsx+0.5)*(1.0/16.0), (tsy+0.5)*(1.0/16.0));
//	dmidST=midST-baseST;
//	baseST=baseST+(dmidST*tcw);

//	mip=4.0*log2(abs(eye.z));
//	mip=4.0;
//	mip=0.75*log2(eyedist);
//	mip=log2(eyedist*0.5);
	mip=log2(eyedist*0.75);
//	mip=log2(eyedist*2.0);
//	mip=log2(eyedist*eyedist*0.5);
//	mip=log2(eyedist);
	if(mip>6.0)mip=6.0;

	rgba=textureLod(texBase, baseST, mip);
//	rgba=texture2D(texBase, baseST);
//	rgba=texture2D(texBase, srcST);
//	glow=texture2D(texGlow, texST);

//	rgba=vec4(0.5, 0.5, 0.5, 1.0);

	norm=
		normal_x*(2.0*npix.r-1.0) +
		normal_y*(2.0*npix.g-1.0) +
		normal  *(2.0*npix.b-1.0);

	ndotl=dot(norm, ldir);
	
	ndotl=0.8+(0.4*ndotl);

//	ndotl=1.0;
	
//	ndotl=ndotl+(dot(norm, eye)*0.2)+norm.z*0.2;

//	rgba=ConvTex2RGBA(rgba);

//	spec=vec4(1.0, 1.0, 1.0, 0.0);
	spec=vec4(0.3, 0.3, 0.3, 0.0);

	ndoth=dot(norm, eye);
//	spe=pow(ndoth, 2.0);
	spe=ndoth*ndoth;

//	spe=0.0;

	// output final color
//	gl_FragColor=gl_Color*rgba;
//	gl_FragColor=extColor*rgba;
//	gl_FragColor=att*(diffuse*NdotL+ambient+spec*spexp);
//	gl_FragColor=extColor*(rgba*ndotl)+vec4(norm, 0.0);
//	gl_FragColor=extColor*(rgba*ndotl);
//	gl_FragColor=vec4((extColor*(rgba*ndotl+spec*spe)).xyz, rgba.a);

	a=1.0;
//	if((noise1(position*500.0)*0.5+0.5+rgba.a)<1.0)
//	if(rgba.a<0.95)
	if(true)
	{
//		if((hfnoise(position)*0.5+0.5+rgba.a)<1.0)
//		if((hfnoise(position)+rgba.a)<1.0)

		//gl_FragCoord
//		if((hfnoise(position)+btge_scurve(rgba.a))<1.0)
//		if(btge_scurve(hfnoise(position)+rgba.a)<1.0)
//		if((hfbayer4(gl_FragCoord)+btge_scurve(rgba.a))<1.0)
//		if((hfbayer4(gl_FragCoord)+rgba.a)<1.0)
//		if((hfign(gl_FragCoord)+rgba.a)<1.0)
//			a=0.0;

//		a=((hfign(gl_FragCoord)+rgba.a)-1.0)*1000.0;
//		a=((hfign(gl_FragCoord)+rgba.a)-0.998)*1000.0;
		a=((hfign(gl_FragCoord)+(rgba.a*1.05))-1.0)*1000.0;
//		a=((hfign(gl_FragCoord+hgofs)+(rgba.a*1.05))-1.0)*1000.0;
//		a=((hgign+(rgba.a*1.05))-1.0)*1000.0;
		a=clamp(a, 0.0, 1.0);
	}

	if(a<0.5)
		discard;

	gl_FragColor=vec4((extColor*(rgba*ndotl+spec*spe)).xyz, a);

	//gl_FragColor=vec4(vec3(rgb)*height, 1.0);
}

