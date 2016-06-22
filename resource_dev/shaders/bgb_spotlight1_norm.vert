// uniform vec3 vecSdir;
// uniform vec3 vecTdir;
varying vec3 vecSdir;
varying vec3 vecTdir;

varying vec4 diffuse, ambient, specular;
varying vec3 lorg, halfVector, eyeVec;

varying vec3 point, normal, normal_x, normal_y;
varying float value, hivalue, specAdd;

void main()
{
	mat3 eyeMat;
	vec4 mdlVec;
	vec3 eyeOrg;

	point = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);
//	normal = gl_Normal;
	lorg = vec3(gl_LightSource[0].position);

//	eyeOrg = (vec3(0,0,0) * gl_ModelViewProjectionMatrix).xyz;
//	eyeOrg = (vec3(0,0,0) * gl_ModelViewProjectionMatrixInverse).xyz;
//	eyeOrg = (gl_ModelViewProjectionMatrixInverse * vec4(0,0,0,0)).xyz;
//	eyeOrg = (vec4(0,0,0,0) * gl_ModelViewProjectionMatrixInverse).xyz;
//	eyeOrg = (vec4(0,0,0,0) * gl_ProjectionMatrixInverse).xyz;
	eyeOrg = (gl_ProjectionMatrixInverse * vec4(0,0,0,1)).xyz;

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
				if(normal.z<0)
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
				if(normal.z<0)
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

	eyeMat[0]=normal_x;
	eyeMat[1]=normal_y;
	eyeMat[2]=normal;

	mdlVec = gl_ModelViewMatrix * gl_Vertex;
	eyeVec = vec3(-mdlVec) * eyeMat;      
//	eyeVec = vec3(mdlVec) * eyeMat;      

//	eyeVec = vec3(eyeOrg-point) * eyeMat;      
//	eyeVec = vec3(point-eyeOrg) * eyeMat;      

	halfVector = normalize(gl_LightSource[0].halfVector.xyz);

	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	specular = gl_FrontMaterial.specular * gl_LightSource[0].specular;

	value = gl_LightSource[0].constantAttenuation;
	hivalue = gl_LightSource[0].quadraticAttenuation;
	specAdd = hivalue / (1.0+value);

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
