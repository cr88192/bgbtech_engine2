uniform sampler2D texBase;
uniform sampler2D texScreen;
uniform sampler2D texScreenDepth;

uniform vec2 screenOrigin;
uniform vec2 screenSize;
uniform vec2 screenTexSize;

//uniform sampler2D deform, base;
uniform float time;
uniform float scale;
uniform float splitOffset;

uniform vec4 distortK;


void main(void)
{
	vec4 pix0, pix1;
	vec2 st, stc, stv;
	float rt, sc, gam, scc, sto;
//	float sc=scale;

	gam=(gl_Color.a+(gl_Color.r-gl_Color.g));
	gam=gam*gam;

	st=vec2(gl_FragCoord.x/screenTexSize.x,
		gl_FragCoord.y/screenTexSize.y);

//	scc=0.5*(screenSize.x/screenTexSize.x);
	scc=1.0*(screenSize.x/screenTexSize.x);
	if(st.x>=scc)
		{ sto=1.0; }
	else
		{ sto=-1.0; }
		
	stc=screenOrigin+0.5*screenSize;
	stc=vec2(stc.x/screenTexSize.x, stc.y/screenTexSize.y);
	stc=stc-1.0*sto*vec2(splitOffset, 0.0);

	stv=st-stc;
	stv=stv*vec2(2.0, 2.0);
	rt=dot(stv, stv);
//	sc=0.9 + 3.2*rt + 9.6*rt*rt + 9.6*rt*rt*rt;
	sc=distortK.x + distortK.y*rt +
		distortK.z*rt*rt + distortK.w*rt*rt*rt;
//	st=stv*sc+stc;
	st=stc+sc*stv*vec2(0.5, 0.5);

//	st=st+sto*vec2(0.020, 0.0);
//	st=st+sto*vec2(0.025, 0.0);
//	st=st+sto*vec2(0.026, 0.0);
//	st=st+sto*vec2(0.028, 0.0);
//	st=st+sto*vec2(0.032, 0.0);

	st=st+sto*vec2(splitOffset, 0.0);

//	st=gl_TexCoord[0].st;
//	pix0=texture2D(texScreen, st);

	if((st.x<0.0) || (st.y<0.0))
	{
		pix0=vec4(0.0, 0.0, 0.0, 1.0);
	}else
	{
		pix0=texture2D(texScreen, st);
	}

//	st3=vec2(gl_FragCoord.x/1024.0,
//		(600.0/1024.0)-gl_FragCoord.y/1024.0);

//	pix1=texture2D(basetex, st3+st2);

//	gl_FragColor = gl_Color;
//	gl_FragColor = vec4(pix1.rgb, color.a);
//	gl_FragColor = vec4(pix1.rgb, sin(time));

	pix0=pix0*gam;

	gl_FragColor = vec4(pix0.rgb, 1.0);
}
