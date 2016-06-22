varying vec4 color;
varying vec2 st0, st1;
// uniform sampler2D base, deform;
//uniform sampler2D deform, base;

void main()
{
	color=gl_Color;
	st0=gl_MultiTexCoord0.st;
	st1=gl_MultiTexCoord1.st;

	gl_Position = ftransform();
}
