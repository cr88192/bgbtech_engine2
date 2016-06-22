uniform sampler2D texBase;
uniform sampler2D texScreen;

uniform bool texIsUVAY;

uniform float time;
uniform float floatParm0;

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
	vec4 pix0, pix1;
	vec2 st2, st3;
	float sc;

	sc=floatParm0;
	pix0=texture2D(texBase, gl_TexCoord[0].st);
	pix0=ConvTex2RGBA(pix0);
	st2=vec2((2.0*pix0.r-1.0)*sc, (2.0*pix0.g-1.0)*sc);

//	st3=vec2(gl_FragCoord.x/1024.0,
//		(600.0/1024.0)-gl_FragCoord.y/1024.0);
	st3=vec2(gl_FragCoord.x/1024.0,
		gl_FragCoord.y/1024.0);

	pix1=texture2D(texScreen, st3-st2);
	gl_FragColor = vec4(pix1.rgb, 1.0);

//	gl_FragColor = vec4(pix0.rgb, 1.0);
}

