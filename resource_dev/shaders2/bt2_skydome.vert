uniform float time;
uniform float exposure;

uniform vec3 camOrg;
uniform vec3 camRotX;
uniform vec3 camRotY;
uniform vec3 camRotZ;

// varying vec3 vecSdir;
// varying vec3 vecTdir;

varying vec3 point, normal;
varying vec3 relPoint;
varying vec3 relPvec;
varying	vec2 relST;
// varying vec3 eyeVec;
varying vec4 extColor;

vec4 checkExtendColor(vec4 clr)
{
	float sc;
	int e;
	
	e=int(clr.a*255.0);
	if((e<0x70) || (e>0x8F))
		return(clr);
	sc=pow(2.0, float(e-0x7F));
	return(vec4(clr.rgb*sc, 1.0));
}

void main() 
{ 
	mat3 eyeMat;
	vec4 mdlVec;
	vec3 eyeOrg;
	float d;

	point = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);

//	eyeOrg = (gl_ProjectionMatrixInverse * vec4(0.0,0.0,0.0,1.0)).xyz;
	eyeOrg=camOrg;
	relPoint=point-eyeOrg;
	
	d=length(relPoint);
	relPvec=vec3(atan(relPoint.y, relPoint.x), acos(relPoint.z/d), d);

	relST=vec2(
		cos(relPvec.x)*sin(relPvec.y)*0.5+0.5,
		sin(relPvec.x)*sin(relPvec.y)*0.5+0.5);

	extColor=checkExtendColor(gl_Color);
	extColor=vec4(extColor.rgb*exposure, extColor.a);

//	gl_FrontColor = gl_Color;
//	gl_TexCoord[0] = gl_MultiTexCoord0; 
//	gl_TexCoord[0] = relST; 
	gl_Position = ftransform(); 
}
