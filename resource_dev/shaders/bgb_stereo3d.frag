uniform sampler2D texBase;
uniform sampler2D texScreen;
// uniform sampler2DShadow texScreenDepth;
uniform sampler2D texScreenDepth;

uniform float time;
uniform float floatParm0;
uniform float floatParm1;

uniform vec2 screenSize;
uniform vec2 screenTexSize;

//varying vec2 st0;

void main()
{
	vec4 pix0, pix1, pix2, pix3;
	vec2 st1, st2, st3, st;
	vec2 sta, stb, stc;
	vec3 str;
	float h00, h01, h02;
	float h10, h11, h12;
	float h20, h21, h22;
	float ht0, ht1, ht2, hte, hto;
	float hta, htb, htc, htd;
	float sc, rt, y0, y1, gam;
	float fi, fj, fk, sto, scc, scv;

	sc=floatParm0 * gl_Color.a;
	rt=floatParm1;

//	gam=(gl_Color.a+0.5);
	gam=(gl_Color.a+(gl_Color.r-gl_Color.g));
	gam=gam*gam;

//	st2=gl_TexCoord[0].st+vec2(sin(time*rt), cos(time*rt));
//	pix0=texture2D(texBase, st2);
//	st2=vec2((2*pix0.r-1)*sc, (2*pix0.g-1)*sc);

//	st3=vec2(gl_FragCoord.x/1024.0,
//		(600.0/1024.0)-gl_FragCoord.y/1024.0);
//	st3=vec2(gl_FragCoord.x/1024.0,
//		gl_FragCoord.y/1024.0);

//	pix1=texture2D(texScreen, st3-st2);

//	st=gl_TexCoord[0].st;
//	st=vec2(gl_FragCoord.x/1024.0,
//		gl_FragCoord.y/1024.0);
	st=vec2(gl_FragCoord.x/screenTexSize.x,
		gl_FragCoord.y/screenTexSize.y);

//	scc=0.5*(800.0/1024.0);
//	scc=0.5*(1024.0/1024.0);
//	scc=0.5*(1600.0/1024.0);
	scc=0.5*(screenSize.x/screenTexSize.x);
	scv=0.5*(screenSize.y/screenTexSize.y);

	if(st.x>=scc)
	{
		st=vec2(2.0*(st.x-scc), st.y);
		sto=1.0;
	}else
	{
		st=vec2(2.0*(st.x-0.0), st.y);
		sto=-1.0;
	}

//	st=(st-vec2(0.0, scv))*vec2(1.0, 1.5)+vec2(0.0, scv);

#if 1
	st1=st-vec2(scc, scv);
	rt=dot(st1, st1);
//	sc=1.0+0.8*rt+0.4*rt*rt;
//	sc=1.0+3.2*rt+1.2*rt*rt;
//	sc=0.75+3.2*rt+1.2*rt*rt;
	sc=0.9 + 3.2*rt + 9.6*rt*rt + 9.6*rt*rt*rt;
	st=st1*sc+vec2(scc, scv);

//	st=(st-vec2(0.0, scv))*vec2(1.0, 1.6)+vec2(0.0, scv);
	st=(st-vec2(0.0, scv))*vec2(1.0, 1.75)+vec2(0.0, scv);
#endif

	ht0=texture2D(texScreenDepth, st).r;

//	ht1 = 100000.0 / (100000.0 - ht0 * 99999.0);
	ht1 = (100000.0 - 100000.0 / 256.0) / 99999.0;
	
//	ht0=ht0+(1.0-ht1);
//	ht0=ht0-(1.0-ht1);
//	ht0=ht0+0.5*(1.0-ht1);

	ht0=ht0+0.75*(1.0-ht1);

//	hte=0.1*(1.0-ht0);
//	hte=1.0-ht0*ht0;
//	hte=0.5*(1.0-0.5*ht0);
//	hte=2.0*(1.0-ht0);
//	hte=3.0*(1.0-ht0);
//	hte=0.33*(1.0-ht0);
	hte=0.75*(1.0-ht0);

//	hte=0.01*ht0;
//	hte=0.01*((2.0/(ht0+1.0))-1.0);
//	hte=0.1/ht1;

//	hte=0;
//	hte=0.003;
//	hte=0.005;
//	hte=0.007;
//	hte=-0.005;
	hte=-0.003;
//	hte=-0.010;
//	hte=-0.05;
//	hte=0.0;
	hto=0.05;
//	hto=0.04;
//	hto=0.06;

//	hte=-hte;

#if 1
	st1=st+sto*vec2(hte, 0)+sto*vec2(hto, 0); ht1=2.0;
//	st2=st+vec2(hte, 0); ht2=2.0;
//	for(fi=1.0; fi>=0.0; fi-=0.1)
//	for(fi=1.0; fi>=0.0; fi-=0.125)
	for(fi=1.0; fi>=0.0; fi-=0.0625)
	{
		fj=hte*fi;
//		fk=0.5*(fj+fj*fj);
		fk=fi*hte*hte;
		sta=st+sto*vec2(fj, 0)+sto*vec2(hto, 0);
//		stb=st+vec2(fj, 0);
		hta=texture2D(texScreenDepth, sta).r * (1.0/(1.0+fk));
//		htb=texture2D(texScreenDepth, stb).r * (1.0/(1.0+fk));

		if(hta<ht1) { st1=sta; ht1=hta; }
//		if(htb<ht2) { st2=stb; ht2=htb; }
	}

#if 0
//	st1=(st1-vec2(0.0, scv))*vec2(1.0, 1.6)+vec2(0.0, scv);
//	st1=(st1-vec2(0.0, scv))*vec2(1.0, 1.75)+vec2(0.0, scv);

	st2=st1-vec2(scc, scv);
	rt=dot(st2, st2);
//	sc=1.0+0.8*rt+0.4*rt*rt;
//	sc=1.0+3.2*rt+1.2*rt*rt;
//	sc=0.75+3.2*rt+1.2*rt*rt;
//	sc=0.9+3.2*rt+1.2*rt*rt;
//	sc=0.9+3.2*rt+4.8*rt*rt+9.6*rt*rt*rt;
//	sc=0.9 + 3.2*rt + 9.6*rt*rt + 19.2*rt*rt*rt;
	sc=0.9 + 3.2*rt + 9.6*rt*rt + 9.6*rt*rt*rt;
	st1=st2*sc+vec2(scc, scv);

//	st1=(st1-vec2(0.0, scv))*vec2(1.0, 1.6)+vec2(0.0, scv);
	st1=(st1-vec2(0.0, scv))*vec2(1.0, 1.75)+vec2(0.0, scv);
#endif

	pix1=texture2D(texScreen, st1);
//	pix2=texture2D(texScreen, st2);
#endif

	if((st1.x<0.0) || (st1.y<0.0))
		pix1=vec4(0.0, 0.0, 0.0, 1.0);

#if 0
	y0=0.35*pix1.r+0.50*pix1.g+0.15*pix1.b;
	y1=0.35*pix2.r+0.50*pix2.g+0.15*pix2.b;

	ht1=(0.9*pix1.r+0.1*pix1.b)-pix1.g;
	ht1=clamp(ht1, -1.0, 1.0);
	ht1=0.6*ht1*ht1; hta=1.0-ht1;

	ht2=(0.9*pix2.r+0.1*pix2.b)-pix2.g;
	ht2=clamp(ht2, -1.0, 1.0);
	ht2=0.6*ht2*ht2; htb=1.0-ht2;

	pix1=pix1*vec4(hta, hta, hta, 1.0)+y0*vec4(ht1, ht1, ht1, 0.0);
	pix2=pix2*vec4(htb, htb, htb, 1.0)+y1*vec4(ht2, ht2, ht2, 0.0);
#endif

//	pix0=
//		pix1*vec4(0.0, 1.0, 0.2, 0.5)+
//		pix2*vec4(1.0, 0.0, 0.8, 0.5);
	
	pix0=pix1;
		
//	pix0=1.5*pix0;

	pix0=pix0*gam;

//	gl_FragColor = vec4(pix1.rgb, 1.0);
	gl_FragColor = vec4(pix0.rgb, 1.0);
}

