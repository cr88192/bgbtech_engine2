//varying vec2 st0;
varying vec3 point;

void main()
{
	point = vec3(gl_ModelViewMatrix * gl_Vertex);

//	st0 = gl_MultiTexCoord0.st;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
