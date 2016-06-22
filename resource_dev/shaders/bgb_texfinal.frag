// uniform vec2 scaleBias;
uniform sampler2D texBase;
uniform sampler2D texNorm;
uniform sampler2D texGloss;
uniform sampler2D texGlow;

uniform bool texIsUVAY;

varying vec3 point, normal, normal_x, normal_y;
varying vec3 eyeVec;

vec4 ConvTex2RGBA(vec4 clr)
{
	vec4 rgba;
	float g, u, v, a, sc;

	if(texIsUVAY)
	{
#if 1
		a=2.0*clr.z;
		if(a>1.0)
//			{ sc=2.0-(4.0*a-2.0); a=1.0; }
			{ sc=1.0-(a-1.0); a=1.0; }
		else
			{ sc=1.0; }
		u=(2.0*clr.x-1.0)*sc;
		v=(2.0*clr.y-1.0)*sc;
		g=clr.w+v*0.5;
		rgba=vec4(g-v-u*0.5, g, g-v+u*0.5, a);
		return rgba;
#endif

#if 0
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
#endif
	}else
	{
		return clr;
	}
}

void main()
{
	vec2 srcST, texST;
	vec2 texST1, texST2;
	vec3 eye;
	vec4 rgba, glow;
	float height;
	float v;
	float h1, h2, v1, v2;

	srcST = gl_TexCoord[0].xy;
//	height = 1-texture2D(texNorm, srcST).a;
//	height = texture2D(texNorm, srcST).a;

	rgba = texture2D(texNorm, srcST);
	rgba = ConvTex2RGBA(rgba);

	height = rgba.a;
//	height = 1-rgba.a;

//	v = height * scaleBias.x - scaleBias.y;
	v = height * 0.04 - 0.03;
//	v = height * 0.4 - 0.3;
	eye = normalize(eyeVec);
//	v = 0.0;
	texST = srcST + (eye.xy * v);

//	texST = srcST;

#if 0
	texST1 = srcST + (eye.xy * v * 0.33);
	texST2 = srcST + (eye.xy * v * 0.66);
	h1 = texture2D(texNorm, texST1).a;
	h2 = texture2D(texNorm, texST2).a;

	v1=eye.z*0.66;
	v2=eye.z*0.33;
	if(height>=eye.z)	{ texST=srcST; }
	else if(h1>=v1)		{ texST=texST1; }
	else if(h2>=v2)		{ texST=texST2; }
#endif		

	rgba = texture2D(texBase, texST);
	glow = texture2D(texGlow, texST);

	rgba=ConvTex2RGBA(rgba);
	glow=ConvTex2RGBA(glow);

//	if(rgba.a<0.5)
//		discard;

#if 0
//	if(rgba.a<rgba.b)
//	if(1)
	if(texIsUVAY)
	{
		v=rgba.w-(rgba.x+rgba.y-1)*0.5;
//		v=rgba.w;
		rgba=vec4(
			v+2.0*(rgba.y-0.5),
			v,
			v+2.0*(rgba.x-0.5),
			rgba.z
		);

		v=rgba.w-(rgba.x+rgba.y-1)*0.5;
//		v=rgba.w;
		rgba=vec4(
			v+2.0*(rgba.y-0.5),
			v,
			v+2.0*(rgba.x-0.5),
			rgba.z
		);
	}
#endif

	// output final color
//	gl_FragColor = vec4(rgb, 1.0);
//	gl_FragColor = gl_Color * rgba;
//	gl_FragColor = gl_Color * rgba + glow*(rgba.a*gl_Color.a);
	//gl_FragColor = vec4(vec3(rgb)*height, 1.0);
	gl_FragColor = gl_Color * rgba + vec4(glow.rgb, 0.0);
}

