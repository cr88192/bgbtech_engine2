varying vec4 diffuse, ambient, specular, global;
varying vec3 lorg, halfVector;

varying vec3 point, normal;

void main()
{
	float dist;

	vec3 aux;
	vec3 ldir, norm;
	vec3 spdir;

	float ndl,ndhv;
	vec4 color;
	float att, speff;

	aux = lorg - point;
	ldir = normalize(aux);
	norm = normalize(normal);
	
	dist = length(aux);
	color = global;

//	ndl = max(dot(norm, ldir), 0.0);
	ndl = dot(norm, ldir);
	if (ndl>0.0)
	{
		spdir = normalize(gl_LightSource[0].spotDirection);
		speff = dot(spdir, -ldir);
		if (speff > gl_LightSource[0].spotCosCutoff)
		{
			speff = pow(speff, gl_LightSource[0].spotExponent);
		}

		if(gl_LightSource[0].constantAttenuation>=10.0)
		{
			att = (gl_LightSource[0].constantAttenuation-dist) / 256.0;
			color += (speff * att) * (diffuse * ndl + ambient);
		}else
		{
			att = gl_LightSource[0].constantAttenuation +
				gl_LightSource[0].linearAttenuation*dist +
				gl_LightSource[0].quadraticAttenuation*dist*dist;
			if(att != 0.0) color += (speff/att) *
					(diffuse * ndl + ambient);
		}
	
		ndhv = max(dot(norm, halfVector), 0.0);
		color += att * specular * pow(ndhv, gl_FrontMaterial.shininess);
	}

	gl_FragColor = color;
}

