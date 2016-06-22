uniform vec3 light_mins;
uniform vec3 light_maxs;
uniform vec3 light_value;

varying vec4 diffuse, ambient, specular;
varying vec3 lorg, halfVector;

varying vec3 point, normal;

void main()
{
	vec3 ldir, norm, aux, attv;
	float NdotL,NdotHV;
	float dist, att, att2;

	//verify that point is within box
//	if(any(lessThan(point, light_mins)))
//		discard;
//	if(any(greaterThan(point, light_maxs)))
//		discard;

	aux = lorg - point;
	ldir = normalize(aux);
	norm = normalize(normal);

	NdotL = dot(norm, ldir);
//	if(NdotL<0.0) discard;

//	dist = length(aux);
//	att2 = (gl_LightSource[0].constantAttenuation-dist)/256.0;

//	attv = (light_value-abs(aux))*(1.0/256.0);
//	attv = vec3(1,1,1)-(abs(aux)*(1.0/light_value));
//	attv = clamp(attv, vec3(0,0,0), vec3(1,1,1));
//	att = (attv.x+attv.y+attv.z)/3;
//	att = attv.x*attv.y*attv.z;
//	att = min(attv.x, min(attv.y, attv.z));
//	if(att<=0) discard;

	attv = abs(aux)*(1.0/light_value);
	att = 1.0 - length(attv);
//	att = max(att, att2);

	att = clamp(att, 0, 1);

	NdotHV = max(dot(norm, halfVector), 0.0);
	gl_FragColor = att * (diffuse * NdotL + ambient +
		specular * pow(NdotHV,gl_FrontMaterial.shininess));
}

