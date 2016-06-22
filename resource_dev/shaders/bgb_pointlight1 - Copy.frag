varying vec4 diffuse, ambient, specular;
varying vec3 lorg, halfVector;

varying vec3 point, normal;

void main()
{
	vec3 ldir, norm, aux;
	float NdotL,NdotHV;
	float dist, att;

	aux = lorg - point;
	dist = length(aux);
	ldir = aux / dist;
//	ldir = normalize(aux);
//	norm = normalize(normal);
	norm = normal;

	NdotL = dot(norm, ldir);
//	if(NdotL<0.0) discard;

//	dist = length(aux);
	att = (gl_LightSource[0].constantAttenuation-dist)/256.0;
//	if(att<=0) discard;
	att = clamp(att, 0, 1);

//	att = clamp((gl_LightSource[0].constantAttenuation-dist)/256.0, 0, 1);

	NdotHV = max(dot(norm, halfVector), 0.0);
	gl_FragColor = att * (diffuse * NdotL + ambient +
		specular * pow(NdotHV,gl_FrontMaterial.shininess));
}

