uniform float time;

uniform sampler2D texBase;
uniform sampler2D texScreen;
uniform sampler2D texScreenDepth;

uniform vec2 screenOrigin;
uniform vec2 screenSize;
uniform vec2 screenTexSize;

// uniform float time;
uniform float floatParm0;
uniform float floatParm1;

uniform bool texIsUVAY;

varying vec3 point;

//varying vec2 st0;

vec4 ConvTex2RGBA(vec4 clr)
{
	vec4 rgba;
	float g, u, v, a, sc;

	if(texIsUVAY)
	{
		a=2.0*clr.z;
		if(a>1.0)
			{ sc=1.0-(a-1.0); a=1.0; }
		else
			{ sc=1.0; }
		u=(2.0*clr.x-1.0)*sc;
		v=(2.0*clr.y-1.0)*sc;
		g=clr.w-(u+v)*0.25;
		rgba=vec4(g+v, g, g+u, a);
		return rgba;
	}else
	{
		return clr;
	}
}

void main()
{
	vec4 pix0, pix1, pix2;
	vec2 st2, st3;
	float sc, rt, ds, rz, sdist;

	sc=floatParm0 * gl_Color.a;
	rt=floatParm1;

	sc=0.05;
	rt=0.1;

	st2=gl_TexCoord[0].st+vec2(sin(time*rt), cos(time*rt));
	pix0=texture2D(texBase, st2);
	pix0=ConvTex2RGBA(pix0);
	st2=vec2((2.0*pix0.r-1.0)*sc, (2.0*pix0.g-1.0)*sc);

//	st3=vec2(gl_FragCoord.x/1024.0,
//		(600.0/1024.0)-gl_FragCoord.y/1024.0);
//	st3=vec2(gl_FragCoord.x/1024.0,
//		gl_FragCoord.y/1024.0);
	st3=vec2(gl_FragCoord.x/screenTexSize.x,
		gl_FragCoord.y/screenTexSize.y);
//	st3=st3+vec2(screenOrigin.x/screenTexSize.x,
//		screenOrigin.y/screenTexSize.y);

	pix1=texture2D(texScreen, st3-st2);
//	pix1=ConvTex2RGBA(pix1);

//	pix2=texture2D(texScreenDepth, st3-st2);
//	sdist = 100000.0 / (100000.0 - pix2.x * 99999.0);
//	z = 100000.0 / (100000.0 - gl_FragCoord.z * 99999.0);

//	ds=(sdist-z)*0.1;
//	ds=clamp(ds, 0.0, 1.0);

//	ds=gl_FragCoord.w-pix2.x;
//	ds=(pix2.x-gl_FragCoord.z)*10.0;
//	ds=pix2.x*0.1;
//	ds=pix2.x*0.5;
//	ds=(pix2.x-gl_FragCoord.z);
//	ds=gl_FragCoord.z/gl_FragCoord.w;
//	ds=1.0/gl_FragCoord.w;
//	ds=1.0-8.0*gl_FragCoord.w;
//	ds=(pix2.x-gl_FragCoord.w);

//	rz=1.0-((gl_FragCoord.z/gl_FragCoord.w)/100000.0);
//	rz=(gl_FragCoord.z/gl_FragCoord.w)/100000.0;
//	ds=rz;

//	z=(gl_DepthRange.diff*(gl_FragCoord.z/gl_FragCoord.w)+
//		(gl_DepthRange.near+gl_DepthRange.far))*0.5;
//	z=gl_FragCoord.z;

//	ds=(pix2.x-gl_FragCoord.z)*10.0;
//	ds=clamp(ds, 0.0, 1.0);

//	gl_FragColor = vec4(
//		pix1.r*gl_Color.r, pix1.g*gl_Color.g,
//		pix1.b*gl_Color.b, 1);

//	gl_FragColor = vec4(pix1.rgb * (1.0-ds), 1.0);

	gl_FragColor = vec4(pix1.rgb, 1.0);
//	gl_FragColor = vec4(pix0.rgb, 1.0);
}

