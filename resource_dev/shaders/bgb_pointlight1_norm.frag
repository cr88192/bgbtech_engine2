uniform sampler2D texBase;
uniform sampler2D texNorm;
// uniform sampler2D texSpec;
uniform sampler2D texGloss;
// uniform float alphaEnvMap;

uniform bool texIsUVAY;

uniform samplerCube texEnvMap;
uniform samplerCube texEnvDepthMap;
uniform bool envShadowMap;
uniform vec3 envOrigin;
uniform float zNear, zFar;

varying vec4 diffuse, ambient, specular;
varying vec3 lorg, halfVector, eyeVec;

varying vec3 point, normal, normal_x, normal_y;
varying float value, hivalue, specAdd;

vec4 ConvTex2RGBA(vec4 clr)
{
	vec4 rgba;
	float g, u, v, a, sc;

	if(texIsUVAY)
	{
#if 1
		a=2.0*clr.z;
		if(a>1.0)
			{ sc=1.0-(a-1.0); a=1.0; }
		else
			{ sc=1.0; }
		u=(2.0*clr.x-1.0)*sc;
		v=(2.0*clr.y-1.0)*sc;
		g=clr.w+v*0.5;
		rgba=vec4(g-v-u*0.5, g, g-v+u*0.5, a);
		return rgba;
#endif

#if 0
		a=2.0*clr.z;
		if(a>1.0)
			{ sc=1.0-(a-1.0); a=1.0; }
		else
			{ sc=1.0; }
		u=(2.0*clr.x-1.0)*sc;
		v=(2.0*clr.y-1.0)*sc;
		g=clr.w-(u+v)*0.25;
		rgba=vec4(g+v, g, g+u, a);
		return rgba;
#endif
	}else
	{
		return clr;
	}
}

float vecToDepthValue(vec3 vec)
{
    vec3 avec;
    float lz, nz;

	avec = abs(vec);
    lz = max(avec.x, max(avec.y, avec.z));
    return lz;
    
//	nz = (zFar+zNear) / (zFar-zNear) -
//		(2.0*zFar*zNear)/(zFar-zNear)/lz;
//	return (nz + 1.0) * 0.5;
}

void main()
{
	vec4 pix0, pix1, pix2, spec, env, amb, diff, rgba;
	vec3 ldir, norm, norm1, aux, eye;
	vec3 eldir, eaux;
	vec2 baseST, srcST, texST1, texST2;
	float NdotL,NdotHV, spexp, height, v;
	float dist, att, att2, shexp;
	float h1, h2, v1, v2;
	float sc, sdist, lndist, sf;
	float sdz, sdz2;

	aux = lorg - point;
	dist = length(aux);
	ldir = aux / dist;
//	ldir = normalize(aux);
//	norm = normalize(normal);

#if 0
	pix0 = texture2D(texBase, gl_TexCoord[0].st);
	pix0=ConvTex2RGBA(pix0);
	if(pix0.a<0.5)
		discard;
#endif

#if 0
	//base pixel
	pix0 = texture2D(texBase, gl_TexCoord[0].st);
//	diff = diffuse * pix0*2 + vec4(0.002, 0.002, 0.002, 0.000);

	amb = (ambient*pix0)*0.85 + (ambient+pix0)*0.15;
	diff = amb;
//	diff = diffuse * pix0;
#endif

	pix0=texture2D(texNorm, gl_TexCoord[0].st);
	pix0=ConvTex2RGBA(pix0);
//	norm1=vec3(pix0.r-0.5, pix0.g-0.5, pix0.b-0.5);

	//texNorm.a == bump map
//	height = 1-pix0.a;
	height = pix0.a;
	v = height * 0.04 - 0.03;
//	v = height * 0.4 - 0.3;

	//calculate new base ST coords
	eye = normalize(eyeVec);
	srcST = gl_TexCoord[0].st;
//	v = 0.0;
	baseST = srcST + (eye.xy * v);
//	baseST = gl_TexCoord[0].st;

#if 0
	texST1 = srcST + (eye.xy * v * 0.33);
	texST2 = srcST + (eye.xy * v * 0.66);
	h1 = texture2D(texNorm, texST1).a;
	h2 = texture2D(texNorm, texST2).a;

	v1=eye.z*0.66;
	v2=eye.z*0.33;
	if(height>=eye.z)	{ baseST=srcST; }
	else if(h1>=v1)		{ baseST=texST1; }
	else if(h2>=v2)		{ baseST=texST2; }
#endif		


	//get new pixel
	pix0=texture2D(texNorm, baseST);
	pix0=ConvTex2RGBA(pix0);
//	pix0=vec4(0.5, 1, 0.5, 1);

#if 0
//	if((pix0.r!=pix0.b) || (pix0.g!=pix0.b))
//	if(dot(pix0, pix0)!=4.0)
	if(1)
	{
		norm1=
			normal_x* (pix0.r-0.5) +
			normal_y* (pix0.g-0.5) +
			normal  * (pix0.b-0.5);
		norm=normalize(norm1);
	}else
	{
		norm = normalize(normal);
	}
#endif

#if 1
	norm=
		normal_x* (2.0*pix0.r-1.0) +
		normal_y* (2.0*pix0.g-1.0) +
		normal  * (2.0*pix0.b-1.0);
#endif

	spec = specular;

//	pix1 = texture2D(texGloss, gl_TexCoord[0].st);
//	shexp = (0.30*pix1.r) + (0.59*pix1.g) + (0.11*pix1.b);
//	spec = specular * pix1*(1.0/shexp);

//	pix1 = texture2D(texGloss, gl_TexCoord[0].st);
	pix1 = texture2D(texGloss, baseST);
	pix1 = ConvTex2RGBA(pix1);
//	pix1 = texture2D(texSpec, gl_TexCoord[0].st);
	spec = specular * pix1;
	shexp = pix1.a;

//	pix2 = texture2D(texGloss, gl_TexCoord[0].st);
//	shexp = (0.30*pix2.r) + (0.59*pix2.g) + (0.11*pix2.b);

//	env = vec4(0, 0, 0, 0);

	rgba = texture2D(texBase, baseST);
	rgba = ConvTex2RGBA(rgba);

//	if(rgba.a<0.5)
//		discard;

#if 0
	if(alphaEnvMap>0)
	{
		pix2 = textureCube(texEnvMap, reflect(point, normal));
		env = pix2 * alphaEnvMap;
	}else
	{
		env = vec4(0, 0, 0, 0);
	}
#endif

	NdotL = dot(norm, ldir);
//	if(NdotL<0.0) discard;
//	if(NdotL<0.0)
//		NdotL=0;

//	dist = length(aux);
//	att = (gl_LightSource[0].constantAttenuation-dist)/256.0;
//	att = (value-dist)/256.0;
//	if(att<=0) discard;
//	att = clamp(att, 0, 1);

//	att = max((value-dist)/256.0, 0);

	att2 = clamp((value-dist)/256.0, 0.0, 1.0);
	att = max((hivalue-dist)/256.0, att2);
	att = att * rgba.a;

	if(envShadowMap)
//	if(false)
	{
		eaux = envOrigin - point;
		lndist = vecToDepthValue(eaux);
//		eldir = normalize(eaux);
//		eldir = eaux / lndist;

//		clr = textureCube(texEnvMap, -eldir);
//		sdz = textureCube(texEnvDepthMap, -eldir).x;
		sdz = textureCube(texEnvDepthMap, -eaux).x;

//		sdz2 = 2.0 * sdz - 1.0;
//		sdist = (2.0 * zNear * zFar) / (zFar + zNear - sdz2 * (zFar - zNear));

		sdist = (zNear * zFar) / (zFar - sdz * (zFar - zNear));
//		sdist = zNearFar / (zFar - sdz * zFarNNear);

//		sdist = sdist * 0.9;
//		sdist = sdist * 0.75;
//		sdist = sdist * 0.515;
//		sdist = sdist * 0.52;
//		sdist = sdist * 0.5125;
		sdist = sdist * 0.51;

		if(lndist>sdist)
			att=att-att*0.75;

//		sf=clamp((lndist-sdist)*0.125, 0.0, 1.0);
//		sf=clamp((lndist-sdist)*0.25, 0.0, 1.0);
//		att=att-(att*sf)*0.5;
//		att=att-(att*sf)*0.75;
	}

	NdotHV = max(dot(norm, halfVector), 0.0);
	spexp = pow(NdotHV, shexp*gl_FrontMaterial.shininess) + specAdd;
//	gl_FragColor = att * (amb + diff * NdotL + spec * spexp);
//	gl_FragColor = att * (diffuse * NdotL + ambient + spec * spexp);
//	gl_FragColor = att * (diffuse * NdotL + ambient +
//		spec * pow(NdotHV, shexp*gl_FrontMaterial.shininess)) + env;

//	gl_FragColor = att * (diffuse * NdotL + ambient + spec * spexp);
//	gl_FragColor = vec4(
//		(att * (diffuse * NdotL + ambient + spec * spexp)).xyz,
//		att*rgba.a);

	gl_FragColor = att * (diffuse * NdotL + ambient + spec * spexp);
//	gl_FragColor = (att*att) * (diffuse * NdotL + ambient + spec * spexp);
}

