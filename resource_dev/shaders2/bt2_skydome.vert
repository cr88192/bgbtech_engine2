uniform mat4 pgl_ModelViewMatrix;
uniform mat4 pgl_ProjectionMatrix;

uniform float time;
uniform float exposure;

uniform vec3 camOrg;
uniform vec3 camRotX;
uniform vec3 camRotY;
uniform vec3 camRotZ;

// attribute vec4 gl_Color;
// attribute vec4 gl_SecondaryColor;
// attribute vec4 gl_Normal;
// attribute vec4 gl_Vertex;
// attribute vec4 gl_TexCoord;

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
	mat3 normalMatrix;
	mat3 eyeMat;
	vec4 mdlVec;
	vec3 eyeOrg;
	float d;

//	normalMatrix=transpose(inverse(mat3(gl_ModelViewMatrix)));
	normalMatrix=mat3(gl_ModelViewMatrix);

//	normalMatrix=mat3(
//		vec3(	gl_ModelViewMatrix[0][0], 
//				gl_ModelViewMatrix[0][1],
//				gl_ModelViewMatrix[0][2]),
//		vec3(	gl_ModelViewMatrix[1][0], 
//				gl_ModelViewMatrix[1][1],
//				gl_ModelViewMatrix[1][2]),
//		vec3(	gl_ModelViewMatrix[2][0], 
//				gl_ModelViewMatrix[2][1],
//				gl_ModelViewMatrix[2][2]));

//	point = vec3(pgl_ModelViewMatrix * gl_Vertex);
	point = vec3(gl_ModelViewMatrix * gl_Vertex);
//	normal = normalize(gl_NormalMatrix * gl_Normal);
//	normal = normalize(normalMatrix * vec3(gl_Normal));
	normal = normalize(normalMatrix * gl_Normal);

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
//	gl_Position = ftransform(); 
//	gl_Position=pgl_ProjectionMatrix*pgl_ModelViewMatrix*gl_Vertex;
	gl_Position=gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
}
