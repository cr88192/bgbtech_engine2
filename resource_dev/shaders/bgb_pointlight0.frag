varying vec4 diffuse, ambient, specular, global;
varying vec3 lorg, halfVector;

varying vec3 point, normal;

void main()
{
	float dist;

	vec3 aux;

	vec3 ldir, norm;
	float NdotL,NdotHV;
	vec4 color;
	float att;

	aux = lorg - point;
	ldir = normalize(aux);
	norm = normalize(normal);
	
	dist = length(aux);
	color = global;

//	NdotL = max(dot(norm, ldir), 0.0);
	NdotL = dot(norm, ldir);
	if (NdotL>0.0)
	{
		if(gl_LightSource[0].constantAttenuation>=10.0)
		{
			att = (gl_LightSource[0].constantAttenuation-dist) / 256.0;
			color += att * (diffuse * NdotL + ambient);
		}else
		{
			att = gl_LightSource[0].constantAttenuation +
				gl_LightSource[0].linearAttenuation*dist +
				gl_LightSource[0].quadraticAttenuation*dist*dist;
			if(att != 0.0) color += (1.0 / att) *
					(diffuse * NdotL + ambient);
		}
	
		NdotHV = max(dot(norm, halfVector), 0.0);
		color += att * specular * pow(NdotHV,gl_FrontMaterial.shininess);
	}

	gl_FragColor = color;

}

