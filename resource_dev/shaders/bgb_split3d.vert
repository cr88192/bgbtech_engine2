// varying vec4 color;
// varying vec2 st0, st1;
// uniform sampler2D base, deform;
//uniform sampler2D deform, base;

void main()
{
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
