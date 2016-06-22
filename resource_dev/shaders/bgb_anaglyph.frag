uniform sampler2D texBase;
uniform sampler2D texScreen;
// uniform sampler2DShadow texScreenDepth;
uniform sampler2D texScreenDepth;

uniform vec2 screenSize;
uniform vec2 screenTexSize;

uniform float time;
// uniform float floatParm0;
// uniform float floatParm1;

uniform vec4 leftColor;
uniform vec4 rightColor;

invariant varying vec4 colorGamma;
invariant varying vec4 leftColorGammaWeight;
invariant varying vec4 rightColorGammaWeight;
invariant varying vec4 leftColorGammaAdjust;
invariant varying vec4 rightColorGammaAdjust;

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
	float ht0, ht1, ht2, hte;
	float hta, htb, htc, htd;
	float sc, rt, y0, y1, gam;
	float fi, fj, fk;

//	sc=floatParm0 * gl_Color.a;
//	rt=floatParm1;

//	gam=(gl_Color.a+0.5);
	gam=(gl_Color.a+(gl_Color.r-gl_Color.g));
	gam=gam*gam;

	st=vec2(gl_FragCoord.x/screenTexSize.x,
		gl_FragCoord.y/screenTexSize.y);

	ht0=texture2D(texScreenDepth, st).r;
	
//	ht0 = 1.0 / (1.0 - ht0*0.99999);
//	ht0 = 1.0 / (100000.0 - ht0 * 99999.0);
//	ht0 = 4.0 / (10.0 - ht0 * 9.0);
//	ht0 = 1.0 / (10.0 - ht0 * 9.5);
//	ht0 = 4.0 / (100000.0 - ht0 * 99999.0);

//	ht0 = ht0 * 2.0;
//	ht0 = ht0 * ht0 * 2.0;

//	ht1 = 100000.0 / (100000.0 - ht0 * 99999.0);
	ht1 = (100000.0 - 100000.0 / 256.0) / 99999.0;
	
//	ht0=ht0+(1.0-ht1);
//	ht0=ht0-(1.0-ht1);
//	ht0=ht0+0.5*(1.0-ht1);

	ht0=ht0+0.75*(1.0-ht1);
//	ht0=ht0-0.75*(1.0-ht1);

//	hte=0.1*(1.0-ht0);
//	hte=1.0-ht0*ht0;
//	hte=0.5*(1.0-0.5*ht0);
//	hte=2.0*(1.0-ht0);
//	hte=3.0*(1.0-ht0);
//	hte=1.5*(1.0-ht0);
//	hte=1.0*(1.0-ht0);
//	hte=0.5*(1.0-ht0);
//	hte=0.25*(1.0-ht0);
//	hte=0.33*(1.0-ht0);
//	hte=0.75*(1.0-ht0);

//	hte=0.002*ht0;
	hte=0.003;

//	hte=0.01*ht0;
//	hte=0.01*((2.0/(ht0+1.0))-1.0);
//	hte=0.1/ht1;

//	hte=-hte;

#if 1
	st1=st-vec2(hte, 0); ht1=2.0;
	st2=st+vec2(hte, 0); ht2=2.0;
//	for(fi=1.0; fi>=0.0; fi-=0.1)
	for(fi=1.0; fi>=0.0; fi-=0.125)
//	for(fi=1.0; fi>=0.0; fi-=0.0625)
	{
		fj=hte*fi;
//		fk=0.5*(fj+fj*fj);
		fk=fi*hte*hte;
		sta=st-vec2(fj, 0);
		stb=st+vec2(fj, 0);
		hta=texture2D(texScreenDepth, sta).r * (1.0/(1.0+fk));
		htb=texture2D(texScreenDepth, stb).r * (1.0/(1.0+fk));
//		hta=texture2D(texScreenDepth, sta).r * (1.0/(1.0-fk));
//		htb=texture2D(texScreenDepth, stb).r * (1.0/(1.0-fk));

//		hta=texture2D(texScreenDepth, sta).r;
//		htb=texture2D(texScreenDepth, stb).r;
//		hta=sqrt(fj*fj+hta*hta);
//		htb=sqrt(fj*fj+htb*htb);
//		hta=sqrt(hta*hta-fj*fj);
//		htb=sqrt(htb*htb-fj*fj);

		if(hta<ht1) { st1=sta; ht1=hta; }
		if(htb<ht2) { st2=stb; ht2=htb; }
	}

	pix1=texture2D(texScreen, st1);
	pix2=texture2D(texScreen, st2);
#endif

	y0=0.35*pix1.r+0.50*pix1.g+0.15*pix1.b;
	y1=0.35*pix2.r+0.50*pix2.g+0.15*pix2.b;
//	pix1=pix1*vec4(0.7, 0.7, 0.7, 1.0)+y0*vec4(0.3, 0.3, 0.3, 0.0);
//	pix2=pix2*vec4(0.7, 0.7, 0.7, 1.0)+y1*vec4(0.3, 0.3, 0.3, 0.0);
//	pix1=pix1*vec4(0.8, 0.8, 0.8, 1.0)+y0*vec4(0.2, 0.2, 0.2, 0.0);
//	pix2=pix2*vec4(0.8, 0.8, 0.8, 1.0)+y1*vec4(0.2, 0.2, 0.2, 0.0);

//	pix1=pix1*vec4(0.6, 0.6, 0.6, 1.0)+y0*vec4(0.4, 0.4, 0.4, 0.0);
//	pix2=pix2*vec4(0.6, 0.6, 0.6, 1.0)+y1*vec4(0.4, 0.4, 0.4, 0.0);

//	pix1=pix1*vec4(0.0, 0.0, 0.0, 1.0)+y0*vec4(1.0, 1.0, 1.0, 0.0);
//	pix2=pix2*vec4(0.0, 0.0, 0.0, 1.0)+y1*vec4(1.0, 1.0, 1.0, 0.0);

#if 0
//	ht1=(0.75*pix1.r+0.25*pix1.b)-pix1.g; ht1=0.7*ht1*ht1; hta=1.0-ht1;
//	ht2=(0.75*pix2.r+0.25*pix2.b)-pix2.g; ht2=0.7*ht2*ht2; htb=1.0-ht2;
	ht1=(0.9*pix1.r+0.1*pix1.b)-pix1.g;
	ht1=clamp(ht1, -1.0, 1.0);
//	ht1=0.8*ht1*ht1; hta=1.0-ht1;
	ht1=ht1*ht1; hta=1.0-ht1;

	ht2=(0.9*pix2.r+0.1*pix2.b)-pix2.g;
	ht2=clamp(ht2, -1.0, 1.0);
//	ht2=0.8*ht2*ht2; htb=1.0-ht2;
	ht2=ht2*ht2; htb=1.0-ht2;

	pix1=pix1*vec4(hta, hta, hta, 1.0)+y0*vec4(ht1, ht1, ht1, 0.0);
	pix2=pix2*vec4(htb, htb, htb, 1.0)+y1*vec4(ht2, ht2, ht2, 0.0);
#endif

#if 0
	ht1=(0.7*pix1.r+0.3*pix1.b)-pix1.g;
	ht1=clamp(ht1, -1.0, 1.0);
	ht1=0.7*ht1*ht1; hta=1.0-ht1;
	pix1=pix1*vec4(1.0, hta, 1.0, 1.0)+y0*vec4(0.0, ht1, 0.0, 0.0);

//	pix1=pix1*vec4(1.0, 0.1, 1.0, 1.0)+y0*vec4(0.0, 0.9, 0.0, 0.0);
//	pix1=pix1*vec4(1.0, 0.3, 1.0, 1.0)+y0*vec4(0.0, 0.7, 0.0, 0.0);
#endif

#if 0
//	ht1=dot(pix1, leftColor)-dot(pix1, rightColor);
	ht1=dot(pix1, leftColorGammaWeight)-dot(pix1, rightColorGammaWeight);
	ht1=clamp(ht1, -1.0, 1.0);
//	ht1=0.8*ht1*ht1; hta=1.0-ht1;
	ht1=ht1*ht1; hta=1.0-ht1;

//	ht2=(0.9*pix2.r+0.1*pix2.b)-pix2.g;
//	ht2=dot(pix2, leftColor)-dot(pix2, rightColor);
	ht2=dot(pix2, leftColorGammaWeight)-dot(pix2, rightColorGammaWeight);
	ht2=clamp(ht2, -1.0, 1.0);
//	ht2=0.8*ht2*ht2; htb=1.0-ht2;
	ht2=ht2*ht2; htb=1.0-ht2;

	pix1=pix1*vec4(hta, hta, hta, 1.0)+y0*vec4(ht1, ht1, ht1, 0.0);
	pix2=pix2*vec4(htb, htb, htb, 1.0)+y1*vec4(ht2, ht2, ht2, 0.0);
#endif

#if 1
	ht1=dot(pix1, leftColorGammaWeight)-dot(pix1, rightColorGammaWeight);
	ht2=dot(pix2, rightColorGammaWeight)-dot(pix2, leftColorGammaWeight);
//	pix1=pix1-ht1*leftColorGammaWeight;
//	pix2=pix2-ht2*rightColorGammaWeight;
//	pix1=pix1-0.33*ht1*leftColor;
//	pix2=pix2-0.33*ht2*rightColor;

	if(ht1>=0) { ht1=ht1*ht1; } else { ht1=-ht1*ht1; }
	if(ht2>=0) { ht2=ht2*ht2; } else { ht2=-ht2*ht2; }

	pix1=pix1-ht1*leftColorGammaAdjust;
	pix2=pix2-ht2*rightColorGammaAdjust;
#endif

//	pix0=
//		pix1*vec4(0.4, 0.1, 0.5, 0.5)+
//		pix2*vec4(0.1, 0.4, 0.5, 0.5);
//	pix0=
//		pix1*vec4(0.8, 0.1, 0.25, 0.25)+
//		pix2*vec4(0.1, 0.8, 0.25, 0.25)+
//		pix3*vec4(0.1, 0.1, 0.5, 0.5);

//	pix0=
//		pix1*vec4(0.9, 0.1, 0.9, 0.5)+
//		pix2*vec4(0.1, 0.9, 0.1, 0.5);

//	pix0=
//		pix1*vec4(1.0, 0.0, 1.0, 0.5)+
//		pix2*vec4(0.0, 1.0, 0.0, 0.5);

//	pix0=
//		pix1*vec4(0.0, 1.0, 0.2, 0.5)+
//		pix2*vec4(1.0, 0.0, 0.8, 0.5);

	pix0=
		pix1*leftColor+
		pix2*rightColor;
		
	pix0=1.5*pix0;

//	pix1=pix1*0.75+0.25;

//	pix0=vec4(
//		floor(pix1.r*10.0)/10.0,
//		floor(pix1.g*10.0)/10.0,
//		floor(pix1.b*10.0)/10.0,
//		1);

//	pix0=pix0*(1-floor(hte*1000));

//	if(hte>0.001)pix0=vec4(0,0,0,1);
//	if(hte>0.01)pix0=vec4(0,0,0,1);
//	if(hte>0.0001)pix0=vec4(0,0,0,1);

//	gl_FragColor = vec4(
//		pix1.r*gl_Color.r, pix1.g*gl_Color.g,
//		pix1.b*gl_Color.b, 1);

	pix0=pix0*gam;

//	gl_FragColor = vec4(pix1.rgb, 1);
	gl_FragColor = vec4(pix0.rgb, 1);
}

