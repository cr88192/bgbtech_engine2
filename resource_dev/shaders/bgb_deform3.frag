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
//	float sc=scale;
//	float rt=rate;

	float sc0=0.15;
	float sc1=0.03;
	float rt=0.25;

	st0=gl_TexCoord[0].st;

//	st2=vec2(sin(time)*sc0, cos(time)*sc0);
	st2=0.1*st0 + vec2(time*rt*0.1, -time*rt);

	pix0=texture2D(texDeform, st0+st2);

	st2=st0 - vec2((2.0*pix0.r-1.0)*sc1, (2.0*pix0.g-1.0)*sc1+0.05);
//	st2=vec2(clamp(st2.s, 0, 1), clamp(st2.t, 0, 1));

	if(st2.t<0.0)st2=vec2(st2.s, 0.0);
	if(st2.t>1.0)st2=vec2(st2.s, 1.0);

	pix1=texture2D(texBase, st2);

	gl_FragColor = pix1*vec4(0.8, 0.8, 0.8, 1.0);
}
