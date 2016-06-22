#version 120

//Light: Material
uniform sampler2D texBase;
uniform sampler2D texNorm;
// uniform sampler2D texSpec;
uniform sampler2D texGloss;
// uniform float alphaEnvMap;

uniform bool texIsUVAY;
uniform bool texIsNorm;

//Light: Shadow Map
uniform samplerCube texEnvMap;
uniform samplerCube texEnvDepthMap;
uniform bool envShadowMap;
uniform vec3 envOrigin;
uniform float zNear, zFar;

//Light: Box Light
uniform bool lightIsBox;

uniform vec3 light_mins;
uniform vec3 light_maxs;
uniform vec3 light_value;

uniform sampler2D texFalloffXZ;
uniform sampler2D texFalloffYZ;


varying vec4 diffuse, ambient, specular;
varying vec3 lorg, halfVector, eyeVec;

varying vec3 point, normal, normal_x, normal_y;
varying float value, hivalue, specAdd;

varying vec3 boxCoord;

invariant varying float zNearFar;
invariant varying float zFarPNear;
invariant varying float zFarNNear;

// varying float zNearFar;
// varying float zFarPNear;
// varying float zFarNNear;

vec4 ConvTex2RGBA(vec4 clr)
{
	vec4 rgba;
	float g, u, v, a, sc;

	if(texIsUVAY)
	{
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
}

void main()
{
	vec4 pix0, pix1, pix2, spec, env, amb, diff, rgba, foff;
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

	att2 = clamp((value-dist)/256.0, 0.0, 1.0);
	att = max((hivalue-dist)/256.0, att2);

	if(envShadowMap)
//	if(false)
	{
//		eaux = lorg - point;
		eaux = envOrigin - point;
		lndist = vecToDepthValue(eaux);
//		eldir = normalize(eaux);

//		clr = textureCube(texEnvMap, -eaux);
		sdz = textureCube(texEnvDepthMap, -eaux).x;
//		sdz = textureCube(texEnvDepthMap, -eldir).x;

//		sdz = clamp(sdz, 0.75, 1.0);
//		sdz = 0.5;

//		sdz2 = 2.0 * sdz - 1.0;
//		sdist = (2.0 * zNear * zFar) / (zFar + zNear - sdz2 * (zFar - zNear));

//		sdist = (zNear * zFar) / (zFar - sdz * (zFar - zNear));
		sdist = zNearFar / (zFar - sdz * zFarNNear);

//		sdz = clamp(sdz, 0.0, 1.0);
//		sdist = 100000.0 / (100000.0 - sdz * 99999.0);
//		sdist = 1.0 + sdz * 99999.0;

//		sdist = sdist * 1.10;
		sdist = sdist * 0.51;
//		sdist = sdist * 0.52;

//		sdist = sdist * 0.1;
//		lndist = -1.0;

//		sdist = 999999.0;
//		lndist = -1.0;

		if(lndist>sdist)
			att=att-att*0.75;
	}

	if(texIsNorm)
//	if(false)
	{
		pix0=texture2D(texNorm, gl_TexCoord[0].st);
		pix0=ConvTex2RGBA(pix0);

		//texNorm.a == bump map
//		height = 1-pix0.a;
		height = pix0.a;
		v = height * 0.04 - 0.03;
//		v = height * 0.4 - 0.3;

		//calculate new base ST coords
		eye = normalize(eyeVec);
		srcST = gl_TexCoord[0].st;
		baseST = srcST + (eye.xy * v);
//		baseST = gl_TexCoord[0].st;

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

		norm=
			normal_x* (2.0*pix0.r-1.0) +
			normal_y* (2.0*pix0.g-1.0) +
			normal  * (2.0*pix0.b-1.0);

		spec = specular;

//		pix1 = texture2D(texGloss, gl_TexCoord[0].st);
		pix1 = texture2D(texGloss, baseST);
		pix1 = ConvTex2RGBA(pix1);
//		pix1 = texture2D(texSpec, gl_TexCoord[0].st);
		spec = specular * pix1;
		shexp = pix1.a;

		rgba = texture2D(texBase, baseST);
		rgba = ConvTex2RGBA(rgba);

		att = att * rgba.a;

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

	}else
	{
		norm = normal;
		spec = specular;
		shexp = 1.0;

		NdotL = dot(norm, ldir);
	}

	if(lightIsBox)
//	if(false)
	{
		pix1 = texture2D(texFalloffXZ, boxCoord.xz);
		pix1 = ConvTex2RGBA(pix1);
		pix2 = texture2D(texFalloffYZ, boxCoord.yz);
		pix1 = ConvTex2RGBA(pix1);

		foff = pix1 * pix2;

		NdotHV = max(dot(norm, halfVector), 0.0);
		spexp = pow(NdotHV, shexp*gl_FrontMaterial.shininess) + specAdd;
		gl_FragColor = foff * (diffuse * NdotL + ambient + spec * spexp);
	}else
	{
		NdotHV = max(dot(norm, halfVector), 0.0);
		spexp = pow(NdotHV, shexp*gl_FrontMaterial.shininess) + specAdd;
		gl_FragColor = att * (diffuse * NdotL + ambient + spec * spexp);
	}
}

