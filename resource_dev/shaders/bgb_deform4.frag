uniform sampler2D texBase;
uniform sampler2D texDeform;

uniform float time;
uniform float scale;
uniform float rate;

void main()
{
	vec4 pix0, pix1, pix2;
	vec2 st0, st2, st3;
	float y, th;
//	float sc=scale;
//	float rt=rate;

	float sc0=0.05;
	float sc1=0.01;
	float rt=0.15;

	st0=gl_TexCoord[0].st;

	st2=0.1*st0 + vec2(sin(time)*sc0, cos(time)*sc0);
//	st2=0.1*st0 + vec2(time*rt*0.1, -time*rt);

	pix0=texture2D(texDeform, st2);

	st2=st0 - vec2((2.0*pix0.r-1.0)*sc1, (2.0*pix0.g-1.0)*sc1);
	pix1 = texture2D(texBase, st2);

	y=pix1.r*4.0; th=pix1.r-0.5;
//	gl_FragColor = vec4(y, y, y, 1.0);

	gl_FragColor = vec4(
		y*cos(th),
		y*cos(th-0.89),
		y*cos(th-1.57), 1.0);
}
