uniform sampler2D texBase;
uniform sampler2D texDeform;

uniform float time;
uniform float scale;
uniform float rate;


void main(void)
{
	vec3 dir;
	vec4 pix0, pix1, pix2;
	vec2 st0, st2, st3;
	float y;
	float sc=scale;
	float rt=rate;

	st0=gl_TexCoord[0].st;

//	st2=vec2(sin(time)*sc, cos(time)*sc);
	st2=0.5*st0 + vec2(time*rt*0.1, -time*rt);

	pix0=texture2D(texDeform, st2);

	st2=vec2((2.0*pix0.r-1.0)*sc*y, (2.0*pix0.g-1.0)*sc*y);
	pix1=texture2D(texBase, st0-st2);

	y=0.70*pix1.g + 0.20*pix1.r + 0.10*pix1.b;

	gl_FragColor = pix1 * vec4(1.0, 1.0, 1.0, (1.0-y));
}
