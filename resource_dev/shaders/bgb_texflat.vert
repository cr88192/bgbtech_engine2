uniform float time;

varying vec3 point, normal;

void main() 
{ 
	point = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);

	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0; 
	gl_Position = ftransform(); 
}
