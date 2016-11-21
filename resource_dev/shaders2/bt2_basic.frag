// uniform vec2 scaleBias;
uniform sampler2D texBase;
// uniform sampler2D texNorm;
// uniform sampler2D texGloss;
// uniform sampler2D texGlow;

// uniform bool texIsUVAY;

varying vec3 point, normal;
// varying vec3 eyeVec;

float spow(float x, float y)
{
	if(x<0)
		return -pow(-x, y);
	return(pow(x, y));
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

//	rgba = ConvTex2RGBA(rgba);

	// output final color
	gl_FragColor = gl_Color * rgba;
	//gl_FragColor = vec4(vec3(rgb)*height, 1.0);
}

