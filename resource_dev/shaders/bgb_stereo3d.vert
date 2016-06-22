//varying vec2 st0;

void main()
{
//	st0 = gl_MultiTexCoord0.st;
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
