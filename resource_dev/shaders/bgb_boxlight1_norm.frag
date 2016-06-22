uniform vec3 light_mins;
uniform vec3 light_maxs;
uniform vec3 light_value;

uniform sampler2D texBase;
uniform sampler2D texNorm;
// uniform sampler2D texSpec;
uniform sampler2D texGloss;
// uniform samplerCube texEnvMap;
// uniform float alphaEnvMap;

uniform bool texIsUVAY;

uniform sampler2D texFalloffXZ;
uniform sampler2D texFalloffYZ;

varying vec4 diffuse, ambient, specular;
varying vec3 lorg, halfVector;

varying vec3 point, normal, normal_x, normal_y;
varying vec3 boxCoord;

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
		g=clr.w-(u+v)*0.25;
		rgba=vec4(g+v, g, g+u, a);
		return rgba;
	}else
	{
		return clr;
	}
}

void main()
{
	vec4 pix0, pix1, pix2, spec, env, foff;
	vec3 ldir, norm, norm1, aux, attv;
	float NdotL,NdotHV;
	float dist, att, att2, shexp;

	//verify that point is within box
//	if(any(lessThan(point, light_mins)))
//		discard;
//	if(any(greaterThan(point, light_maxs)))
//		discard;

	aux = lorg - point;
	ldir = normalize(aux);
//	norm = normalize(normal);

	pix0=texture2D(texNorm, gl_TexCoord[0].st);
	pix0=ConvTex2RGBA(pix0);
//	norm1=vec3(pix0.r-0.5, pix0.g-0.5, pix0.b-0.5);

//	if((pix0.r!=pix0.b) || (pix0.g!=pix0.b))
//	if(dot(pix0, pix0)!=4.0)
	if(1==1)
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

	spec = specular;

//	pix1 = texture2D(texGloss, gl_TexCoord[0].st);
//	shexp = (0.30*pix1.r) + (0.59*pix1.g) + (0.11*pix1.b);
//	spec = specular * pix1*(1.0/shexp);

	pix1 = texture2D(texGloss, gl_TexCoord[0].st);
	pix1 = ConvTex2RGBA(pix1);
//	pix1 = texture2D(texSpec, gl_TexCoord[0].st);
	spec = specular * pix1;
	shexp = pix1.a;

//	pix2 = texture2D(texGloss, gl_TexCoord[0].st);
//	shexp = (0.30*pix2.r) + (0.59*pix2.g) + (0.11*pix2.b);

	pix1 = texture2D(texFalloffXZ, boxCoord.xz);
	pix1 = ConvTex2RGBA(pix1);
	pix2 = texture2D(texFalloffYZ, boxCoord.yz);
	pix1 = ConvTex2RGBA(pix1);

	foff = pix1 * pix2;

	env = vec4(0, 0, 0, 0);

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

	dist = length(aux);
	att2 = (gl_LightSource[0].constantAttenuation-dist)/256.0;

//	if(att<=0) discard;
//	attv = (light_value-abs(aux))*(1.0/256.0);
//	attv = (light_value-abs(aux))*(1.0/light_value);
//	attv = vec3(1,1,1)-(abs(aux)*(1.0/light_value));
//	attv = clamp(attv, vec3(0,0,0), vec3(1,1,1));
//	att = (attv.x+attv.y+attv.z)/3;
//	att = attv.x*attv.y*attv.z;
//	att = min(attv.x, min(attv.y, attv.z));

//	attv = abs(aux)*(1.0/light_value);
//	att = 1.0 - length(attv);
//	att = max(att, att2);
//	att = clamp(att, 0, 1);

	NdotHV = max(dot(norm, halfVector), 0.0);
//	gl_FragColor = att * (diffuse * NdotL + ambient +
//		spec * pow(NdotHV, shexp*gl_FrontMaterial.shininess)) + env;

	gl_FragColor = foff * (diffuse * NdotL + ambient +
		spec * pow(NdotHV, shexp*gl_FrontMaterial.shininess)) + env;
}

