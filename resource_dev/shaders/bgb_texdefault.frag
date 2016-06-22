// uniform vec2 scaleBias;
uniform sampler2D texBase;
// uniform sampler2D texNorm;
// uniform sampler2D texGloss;
// uniform sampler2D texGlow;

uniform bool texIsUVAY;

varying vec3 point, normal;
// varying vec3 eyeVec;

float spow(float x, float y)
{
	if(x<0)
		return -pow(-x, y);
	return(pow(x, y));
}

vec4 ConvTex2RGBA(vec4 clr)
{
	vec4 rgba;
	float g, u, v, a, sc;

	if(texIsUVAY)
	{
#if 1
		a=2.0*clr.z;
		if(a>1.0)
			{ sc=1.0-(a-1.0); a=1.0; }
//			{ sc=2.0-a; a=1.0; }
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

	rgba = texture2D(texBase, srcST);
//	glow = texture2D(texGlow, texST);

	rgba = ConvTex2RGBA(rgba);

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
	}
#endif

	// output final color
	gl_FragColor = gl_Color * rgba;
	//gl_FragColor = vec4(vec3(rgb)*height, 1.0);
}

