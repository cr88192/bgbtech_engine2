uniform sampler2D texBase;
uniform sampler2D texScreen;

uniform vec2 screenSize;
uniform vec2 screenTexSize;

uniform float time;
uniform float floatParm0;
uniform float floatParm1;

uniform vec4 vecParm0;
uniform vec4 vecParm1;

//varying vec2 st0;

void main()
{
	vec4 pix0, pix1;
	vec2 st2, st3;
	float sc, rt;

	sc=floatParm0 * gl_Color.a;
	rt=floatParm1;

	st2=gl_TexCoord[0].st+vec2(sin(time*rt), cos(time*rt));
	pix0=texture2D(texBase, st2);
	st2=vec2((2.0*pix0.r-1.0)*sc, (2.0*pix0.g-1.0)*sc);

//	st3=vec2(gl_FragCoord.x/1024.0,
//		(600.0/1024.0)-gl_FragCoord.y/1024.0);
//	st3=vec2(gl_FragCoord.x/1024.0,
//		gl_FragCoord.y/1024.0);
	st3=vec2(gl_FragCoord.x/screenTexSize.x,
		gl_FragCoord.y/screenTexSize.y);

	pix1=texture2D(texScreen, st3-st2);

//	gl_FragColor = vec4(
//		pix1.r*gl_Color.r, pix1.g*gl_Color.g,
//		pix1.b*gl_Color.b, 1);

	gl_FragColor = pix1 * vecParm0 + vecParm1;

//	gl_FragColor = vec4(pix1.rgb, 1);
//	gl_FragColor = vec4(pix1.rgb, 1) * vecParm0 + vecParm1;
//	gl_FragColor = vec4(pix0.rgb, 1);
}

