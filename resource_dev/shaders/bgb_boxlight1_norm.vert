uniform vec3 light_mins;
uniform vec3 light_maxs;
uniform vec3 light_value;

uniform vec3 vecSdir;
uniform vec3 vecTdir;

varying vec4 diffuse, ambient, specular;
varying vec3 lorg, halfVector;

varying vec3 point, normal, normal_x, normal_y;
varying vec3 boxCoord;

void main()
{
	point = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);
//	normal = gl_Normal;
	lorg = vec3(gl_LightSource[0].position);

//	boxCoord=(point-light_mins)*(1.0/(light_maxs-light_mins));

//	boxCoord=(point-lorg)*(1.0/light_value) + vec3(0.5, 0.5, 0.5);
//	boxCoord=(point-lorg)*(1.0/light_value);
	boxCoord=(point-lorg)*(0.5/light_value) + vec3(0.5, 0.5, 0.5);
//	boxCoord = clamp(boxCoord, vec3(0,0,0), vec3(1,1,1));

//	normal_x=vec3(normal.z, normal.x, normal.y);
//	normal_y=vec3(normal.y, normal.z, normal.x);

#if 1
	if(length(vecSdir)>0.1)
	{
		//ok, we were given some vectors

//		normal_x=vecSdir;
//		normal_y=vecTdir;
		normal_x = normalize(gl_NormalMatrix * vecSdir);
		normal_y = normalize(gl_NormalMatrix * vecTdir);
	}else
	{
		//figure out some good adjacent normals
		if(abs(normal.x)>abs(normal.y))
		{
			if(abs(normal.x)>abs(normal.z))
			{
				normal_x=vec3(0, 1, 0);
				normal_y=vec3(0, 0, 1);
				if(normal.x<0.0)
					normal_x=-normal_x;
			}else
			{
//				normal_x=vec3(0, 1, 0);
//				normal_y=vec3(1, 0, 0);
				normal_x=vec3(1, 0, 0);
				normal_y=vec3(0, 1, 0);
				if(normal.z<0.0)
				{
					normal_x=-normal_x;
//					normal_y=-normal_y;
				}
			}
		}else
		{
			if(abs(normal.y)>abs(normal.z))
			{
				normal_x=vec3(1, 0, 0);
				normal_y=vec3(0, 0, 1);
//				if(normal.y<0.0)
//					normal_x=-normal_x;
				if(normal.y>0.0)
					normal_x=-normal_x;
			}else
			{
//				normal_x=vec3(0, 1, 0);
//				normal_y=vec3(1, 0, 0);
				normal_x=vec3(1, 0, 0);
				normal_y=vec3(0, 1, 0);
				if(normal.z<0.0)
				{
					normal_x=-normal_x;
//					normal_y=-normal_y;
				}
			}
		}
	}
#endif

//	normal_x=vecSdir;
//	normal_y=vecTdir;

//	normal_x=-normal_x;
	normal_y=-normal_y;

	//clamp
	normal_x=normalize(normal_x-(normal*dot(normal_x, normal)));
	normal_y=normalize(normal_y-(normal*dot(normal_y, normal)));

//	normal = normalize(gl_NormalMatrix * gl_Normal);
//	normal_x = normalize(gl_NormalMatrix * normal_x);
//	normal_y = normalize(gl_NormalMatrix * normal_y);

	halfVector = normalize(gl_LightSource[0].halfVector.xyz);

	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	specular = gl_FrontMaterial.specular * gl_LightSource[0].specular;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
