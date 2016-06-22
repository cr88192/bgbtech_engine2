uniform float time;

// varying vec3 vecSdir;
// varying vec3 vecTdir;

varying vec3 point, normal;
// varying vec3 eyeVec;

void main() 
{ 
	mat3 eyeMat;
	vec4 mdlVec;
	vec3 eyeOrg;

	point = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);

	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0; 
	gl_Position = ftransform(); 
}
