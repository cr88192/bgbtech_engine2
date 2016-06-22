varying vec4 color;
varying vec2 st0, st1;

uniform sampler2D basetex;
uniform sampler2D deformtex;

//uniform sampler2D deform, base;
uniform float time;
uniform float scale;


void main(void)
{
//	vec3 dir;
	vec4 pix0, pix1;
	vec2 st2, st3;
	float sc=scale;

//	st2=vec2(sin(time), cos(time));

	pix0=texture2D(deformtex, st1);

//	pix0=texture2D(deformtex, st1+st2);
//	dir=vec3((2*pix0.r-1)*color.r*sc,
//		(2*pix0.g-1)*color.g*sc,
//		(2*pix0.b-1)*color.b*sc);

//	st2=noise2(time);

	st2=vec2((2.0*pix0.r-1.0)*sc, (2.0*pix0.g-1.0)*sc);

//	st2=vec2(sin(time), cos(time));

//	pix1=texture2D(basetex, st0);
//	pix1=texture2D(basetex, st0+dir.st);
//	pix1=texture2D(basetex, st0+st2);

	st3=vec2(gl_FragCoord.x/1024.0,
		(600.0/1024.0)-gl_FragCoord.y/1024.0);

	pix1=texture2D(basetex, st3+st2);

//	gl_FragColor = gl_Color;
//	gl_FragColor = vec4(pix1.rgb, color.a);
//	gl_FragColor = vec4(pix1.rgb, sin(time));

	gl_FragColor = vec4(pix1.rgb, 1.0);
}
