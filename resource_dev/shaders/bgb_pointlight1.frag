#version 120

uniform samplerCube texEnvMap;
uniform samplerCube texEnvDepthMap;
uniform bool envShadowMap;
// uniform int envShadowMap;
uniform vec3 envOrigin;
uniform float zNear, zFar;

varying vec4 diffuse, ambient, specular;
varying vec3 lorg, halfVector;

varying vec3 point, normal;
varying float value, hivalue;
varying float specAdd;

invariant varying float zNearFar;
invariant varying float zFarPNear;
invariant varying float zFarNNear;

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
	vec4 clr, diff2;
	vec3 ldir, aux;
	vec3 eldir, eaux;
	float dist, att, att2, spexp;
	float sc, sdist, lndist, sf;
	float sdz, sdz2;

	aux = lorg - point;
	dist = length(aux);
	ldir = aux / dist;


//	att = clamp((value-dist)/256.0, 0, 1);
//	att = max((value-dist)/256.0, 0);
	att2 = clamp((value-dist)/256.0, 0.0, 1.0);
	att = max((hivalue-dist)/256.0, att2);

//	diff2 = diffuse;

	if(envShadowMap)
//	if(envShadowMap!=0)
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
//		sdist = (2.0 * zNearFar) / (zFarPNear - sdz2*zFarNNear);
//		sdist = (zNear * zFar) / (zFar - sdz * (zFar - zNear));
		sdist = zNearFar / (zFar - sdz * zFarNNear);

//		sdist = sdist * 0.50;
//		sdist = sdist * 0.515;
//		sdist = sdist * 0.52;
//		sdist = sdist * 0.5125;
//		sdist = sdist * 0.501;
		sdist = sdist * 0.51;

		if(lndist>sdist)
			att=att-att*0.75;

//		sf=clamp((lndist-sdist)*0.25, 0.0, 1.0);
//		sf=clamp((lndist-sdist)*0.0625, 0.0, 1.0);
//		att=att-(att*sf)*0.5;
//		att=att-(att*sf)*0.75;
//		att=att-(att*sf);

//		sdist=sdist * sc;
//		if(lndist>sdist)
//			att=0;
	}

	spexp = pow(max(dot(normal, halfVector), 0.0),
		gl_FrontMaterial.shininess) + specAdd;
//	gl_FragColor = att * (diffuse * dot(normal, ldir) +
//		ambient + specular * spexp);
//	gl_FragColor = vec4((att * (diffuse * dot(normal, ldir) +
//		ambient + specular * spexp)).rgb, att2);
//	gl_FragColor = att * (diffuse * dot(normal, ldir) +
//		ambient + specular * spexp + vec4(0,0,0,1));


//	gl_FragColor = att * (diff2 * dot(normal, ldir) +
//		ambient + specular * spexp);
	gl_FragColor = att * (diffuse * dot(normal, ldir) +
		ambient + specular * spexp);

//	gl_FragColor = (att * att) * (diffuse * dot(normal, ldir) +
//		ambient + specular * spexp);

//	clr = att * (diffuse * dot(normal, ldir) +
//		ambient + specular * spexp);
//	gl_FragColor = clr * clr;
}
