uniform sampler2D texBase;
uniform sampler2D texScreen;
// uniform sampler2DShadow texScreenDepth;
uniform sampler2D texScreenDepth;

uniform vec2 screenSize;
uniform vec2 screenTexSize;

uniform float time;
uniform float floatParm0;
uniform float floatParm1;

//varying vec2 st0;

void main()
{
	vec4 pix0, pix1;
	vec2 st2, st3, st;
	vec3 str;
	float h00, h01, h02;
	float h10, h11, h12;
	float h20, h21, h22;
	float ht0, ht1, ht2, hte;
	float sc, rt, y, y1;

	sc=floatParm0 * gl_Color.a;
	rt=floatParm1;

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

#if 1
#if 0
	h00=texture2D(texScreenDepth, st+vec2(-1/1024.0,	-1/1024.0)).r;
	h01=texture2D(texScreenDepth, st+vec2(0,			-1/1024.0)).r;
//	h02=texture2D(texScreenDepth, st+vec2(1/1024.0,		-1/1024.0)).r;
	h10=texture2D(texScreenDepth, st+vec2(-1/1024.0,	0)).r;
//	h11=texture2D(texScreenDepth, st+vec2(0,			0)).r;
//	h12=texture2D(texScreenDepth, st+vec2(1/1024.0,		0)).r;
//	h20=texture2D(texScreenDepth, st+vec2(-1/1024.0,	1/1024.0)).r;
//	h21=texture2D(texScreenDepth, st+vec2(0,			1/1024.0)).r;
//	h22=texture2D(texScreenDepth, st+vec2(1/1024.0,		1/1024.0)).r;

	h11=texture2D(texScreenDepth, st).r;
#endif

	str=vec3(gl_FragCoord.x/1024.0,
		gl_FragCoord.y/1024.0, 0);

//	h00=texture(texScreenDepth, str+vec3(-1/1024.0,	-1/1024.0, 0));
//	h01=texture(texScreenDepth, str+vec3(0,			-1/1024.0, 0));
//	h10=texture(texScreenDepth, str+vec3(-1/1024.0,	0, 0));
//	h11=texture(texScreenDepth, str).r;

//	h00=texture(texScreenDepth, str+vec3(-5/1024.0,	-5/1024.0, 0));
//	h01=texture(texScreenDepth, str+vec3(0,			-5/1024.0, 0));
//	h10=texture(texScreenDepth, str+vec3(-5/1024.0,	0, 0));
//	h11=texture(texScreenDepth, str).r;

	h00=texture(texScreenDepth, str+vec3(-2/1024.0,	-2/1024.0, 0)).r;
	h01=texture(texScreenDepth, str+vec3( 2/1024.0,	-2/1024.0, 0)).r;
	h10=texture(texScreenDepth, str+vec3(-2/1024.0,	 2/1024.0, 0)).r;
	h11=texture(texScreenDepth, str+vec3( 2/1024.0,	 2/1024.0, 0)).r;

//	h00=texture(texScreenDepth, str+vec3(-1/1024.0,	-1/1024.0, 0)).r;
//	h01=texture(texScreenDepth, str+vec3( 1/1024.0,	-1/1024.0, 0)).r;
//	h10=texture(texScreenDepth, str+vec3(-1/1024.0,	 1/1024.0, 0)).r;
//	h11=texture(texScreenDepth, str+vec3( 1/1024.0,	 1/1024.0, 0)).r;

//	h00=texture(texScreenDepth, str+vec3(-3/1024.0,	-3/1024.0, 0)).r;
//	h01=texture(texScreenDepth, str+vec3( 3/1024.0,	-3/1024.0, 0)).r;
//	h10=texture(texScreenDepth, str+vec3(-3/1024.0,	 3/1024.0, 0)).r;
//	h11=texture(texScreenDepth, str+vec3( 3/1024.0,	 3/1024.0, 0)).r;

	ht0=(h10+h01)-h00;
//	ht1=(h21+h12)-h22;
//	hte=abs(h11-ht0)+abs(h11-ht1);
//	hte=(h11-ht0);
//	hte=abs(h11);
//	hte=abs(h11-ht0);
	hte=(abs(h11-h10)+abs(h01-h00)+
		abs(h11-h01)+abs(h10-h00))*0.25;
//	hte=(abs(h11-ht0)+ht2)*0.5;

#endif

	pix1=texture2D(texScreen, st);

//	pix1=pix1*0.75+0.25;

//	y=0.25*(pix1.r+pix1.b)+0.5*pix1.g;
	y=0.3333*(pix1.r+pix1.g+pix1.b);

	pix1=pix1-vec4(y, y, y, 0);
	pix1=2*pix1+vec4(y, y, y, 0);

	y1=floor(y*4.0+0.5)/4.0;

	pix0=vec4(
		y1+floor((pix1.r-y)*2.0+0.5)/2.0,
		y1+floor((pix1.g-y)*2.0+0.5)/2.0,
		y1+floor((pix1.b-y)*2.0+0.5)/2.0,
		1);

//	pix1=vec4(
//		floor(pix1.r*3.0+0.5)/3.0,
//		floor(pix1.g*3.0+0.5)/3.0,
//		floor(pix1.b*3.0+0.5)/3.0,
//		1);
//	pix0=0.5*pix0+0.5*pix1;

//	pix1=pix1*0.75+0.25;

//	pix0=vec4(
//		floor(pix1.r*10.0)/10.0,
//		floor(pix1.g*10.0)/10.0,
//		floor(pix1.b*10.0)/10.0,
//		1);

//	pix0=vec4(
//		floor(pix1.r*3.0)/3.0,
//		floor(pix1.g*3.0)/3.0,
//		floor(pix1.b*3.0)/3.0,
//		1);

//	pix1=pix1*0.75+0.25;

//	pix0=pix0*(1-floor(hte*1000));

//	if(hte>0.001)pix0=vec4(0,0,0,1);
//	if(hte>0.01)pix0=vec4(0,0,0,1);
//	if(hte>0.0001)pix0=vec4(0,0,0,1);
//	if(hte>0.00005)pix0=vec4(0,0,0,1);

//	if(hte>0.0001)pix0=vec4(0,0,0,1);

#if 1
//	if(hte>0.0001)pix0=pix0*(1.0-((hte-0.0001)*10000));
//	if(hte>0.00005)
	if(1)
	{
//		y1=(1.0-((hte-0.00005)*10000));
//		y1=1.0-(hte*1000);
		y1=(1.0-((hte-0.0001)*10000));
		y1=clamp(y1, 0, 1);
		pix0=pix0*y1;
	}
#endif

//	gl_FragColor = vec4(
//		pix1.r*gl_Color.r, pix1.g*gl_Color.g,
//		pix1.b*gl_Color.b, 1);

//	gl_FragColor = vec4(pix1.rgb, 1);
	gl_FragColor = vec4(pix0.rgb, 1);
}

