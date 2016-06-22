varying vec4 color;
varying vec2 st0, st1;

uniform sampler2D texScreen;
uniform sampler2D texBase;
uniform sampler2D texDeform;

uniform float time;
uniform float scale;
uniform float rate;


void main(void)
{
	vec3 dir;
	vec4 pix0, pix1, pix2;
	vec2 st2, st3;
	float y;
	float sc=scale;
	float rt=rate;

//	st2=vec2(sin(time)*sc, cos(time)*sc);
	st2=vec2(0.5*st0.s+time*rt*0.1, 0.5*st0.t-time*rt);

	pix0=texture2D(texDeform, st2);

	dir=vec3((2.0*pix0.r-1.0)*color.r*sc,
		(2.0*pix0.g-1.0)*color.g*sc,
		(2.0*pix0.b-1.0)*color.b*sc);
//	dir=normalize(dir);

	pix1=texture2D(texBase, st0+dir.st);


	y=0.70*pix1.g + 0.20*pix1.r + 0.10*pix1.b;

	st2=vec2((2*pix0.r-1)*sc*y, (2*pix0.g-1)*sc*y);
	st3=vec2(gl_FragCoord.x/1024.0,
		(600.0/1024.0)-gl_FragCoord.y/1024.0);

	pix2=texture2D(texScreen, st3+st2);

//	gl_FragColor = gl_Color;
//	gl_FragColor = vec4(pix1.rgb, color.a);
//	gl_FragColor = vec4(pix1.rgb, sin(time));

	gl_FragColor = pix2*(1.0-y)+pix1;
}
