varying vec4 color;
varying vec2 st0, st1;

void main()
{
	color=gl_Color;
	st0=gl_MultiTexCoord0.st;
	st1=gl_MultiTexCoord1.st;

	gl_Position = ftransform();
}
