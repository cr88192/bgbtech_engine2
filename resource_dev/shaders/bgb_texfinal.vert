uniform float time;

varying vec3 vecSdir;
varying vec3 vecTdir;

varying vec3 point, normal, normal_x, normal_y;
varying vec3 eyeVec;

void main() 
{ 
	mat3 eyeMat;
	vec4 mdlVec;
	vec3 eyeOrg;

	point = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);
//	eyeOrg = (gl_ModelViewProjectionMatrix * vec4(0,0,0,0)).xyz;
//	eyeOrg = (gl_ModelViewProjectionMatrixInverse * vec4(0,0,0,0)).xyz;
//	eyeOrg = (vec4(0,0,0,0) * gl_ModelViewProjectionMatrixInverse).xyz;
//	eyeOrg = (vec4(0,0,0,0) * gl_ProjectionMatrixInverse).xyz;
	eyeOrg = (gl_ProjectionMatrixInverse * vec4(0,0,0,1)).xyz;

//	mdlVec = vec4(64*sin(time), 64*cos(time), 0, 1);
//	eyeOrg = (gl_ProjectionMatrixInverse * mdlVec).xyz;

//	normal_x=vec3(normal.z, normal.x, normal.y);
//	normal_y=vec3(normal.y, normal.z, normal.x);

#if 1
//	if(length(vecSdir)>0.1)
	if(0)
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

	eyeMat[0]=-normal_x;
	eyeMat[1]=-normal_y;
	eyeMat[2]=normal;

//	mdlVec = gl_Vertex;
	mdlVec = gl_ModelViewMatrix * gl_Vertex;
//	mdlVec = mdlVec + vec4(64 * sin(10*time) * normal_x, 0);	
//	mdlVec = gl_ModelViewProjectionMatrix * gl_Vertex;

//	mdlVec = mdlVec - vec4(gl_ProjectionMatrixInverse * vec4(0,0,-1,0));
	mdlVec = mdlVec + vec4(gl_ProjectionMatrixInverse * vec4(0,0,-1,0));

//	eyeVec = eyeMat * vec3(-mdlVec);
	eyeVec = vec3(-mdlVec) * eyeMat;
//	eyeVec = vec3(mdlVec) * eyeMat;

//	eyeVec = mdlVec.xyz;

//	eyeVec = eyeVec + vec3(gl_ProjectionMatrixInverse * vec4(0,0,-1,0));

//	eyeVec = vec3(point-eyeOrg) * eyeMat;      
//	eyeVec = vec3(eyeOrg-point) * eyeMat;      

	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0; 
	gl_Position = ftransform(); 
}
