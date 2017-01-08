// uniform vec2 scaleBias;
uniform sampler2D texBase;
uniform sampler2D texNorm;
// uniform sampler2D texGloss;
// uniform sampler2D texGlow;

// uniform bool texIsUVAY;

varying vec3 point, normal;
varying vec3 relPoint;
varying vec3 relPvec;
varying	vec2 relST;
// varying vec3 eyeVec;
varying vec4 extColor;

float spow(float x, float y)
{
	if(x<0.0)
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

//	srcST=gl_TexCoord[0].xy;
	srcST=relST;

//	srcST=vec2(
//		(relPoint.x/relPoint.z)+0.5,
//		(relPoint.y/relPoint.z)+0.5);

//	srcST=vec2(
//		cos(relPvec.x)*sin(relPvec.y)*0.5+0.5,
//		sin(relPvec.x)*sin(relPvec.y)*0.5+0.5);

//	eyeOrg = (gl_ProjectionMatrixInverse * vec4(0,0,0,1)).xyz;

	rgba=texture2D(texBase, srcST);
//	glow=texture2D(texGlow, texST);

//	rgba=ConvTex2RGBA(rgba);
//	rgba=vec4(1,0,1,1);

	// output final color
//	gl_FragColor=gl_Color*rgba;
//	gl_FragColor=rgba;
	gl_FragColor=extColor*rgba;
	//gl_FragColor=vec4(vec3(rgb)*height, 1.0);
}

