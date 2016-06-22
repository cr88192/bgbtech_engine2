#version 120

uniform bool texIsNorm;

//Light: Shadow Map
uniform bool envShadowMap;
uniform vec3 envOrigin;
uniform float zNear, zFar;

//Light: Box Light
uniform bool lightIsBox;

uniform vec3 light_mins;
uniform vec3 light_maxs;
uniform vec3 light_value;

//Varying
varying vec3 vecSdir;
varying vec3 vecTdir;

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

void main()
{
	mat3 eyeMat;
	vec4 mdlVec;
	vec3 eyeOrg;

	point = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);
	lorg = vec3(gl_LightSource[0].position);


	if(lightIsBox)
	{
		boxCoord=(point-lorg)*(0.5/light_value) + vec3(0.5, 0.5, 0.5);
	}else
	{
		boxCoord = vec3(0.5, 0.5, 0.5);
	}

	if(texIsNorm)
//	if(false)
	{
		eyeOrg = (gl_ProjectionMatrixInverse * vec4(0,0,0,1)).xyz;

#if 1
		if(length(vecSdir)>0.1)
		{
			//ok, we were given some vectors
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
					normal_x=vec3(1, 0, 0);
					normal_y=vec3(0, 1, 0);
					if(normal.z<0.0)
					{
						normal_x=-normal_x;
//						normal_y=-normal_y;
					}
				}
			}else
			{
				if(abs(normal.y)>abs(normal.z))
				{
					normal_x=vec3(1, 0, 0);
					normal_y=vec3(0, 0, 1);
//					if(normal.y<0.0)
//						normal_x=-normal_x;
					if(normal.y>0.0)
						normal_x=-normal_x;
				}else
				{
					normal_x=vec3(1, 0, 0);
					normal_y=vec3(0, 1, 0);
					if(normal.z<0.0)
					{
						normal_x=-normal_x;
//						normal_y=-normal_y;
					}
				}
			}
		}
#endif

//		normal_x=-normal_x;
		normal_y=-normal_y;

		//clamp
		normal_x=normalize(normal_x-(normal*dot(normal_x, normal)));
		normal_y=normalize(normal_y-(normal*dot(normal_y, normal)));

		eyeMat[0]=-normal_x;
		eyeMat[1]=-normal_y;
		eyeMat[2]=normal;

		mdlVec = gl_ModelViewMatrix * gl_Vertex;
//		mdlVec = mdlVec - vec4(gl_ProjectionMatrixInverse * vec4(0,0,-1,0));
		mdlVec = mdlVec + vec4(gl_ProjectionMatrixInverse * vec4(0,0,-1,0));

		eyeVec = vec3(-mdlVec) * eyeMat;      
	}else
	{
		normal_x = vec3(0, 0, 0);
		normal_y = vec3(0, 0, 0);
		eyeVec = vec3(0, 0, 0);
	}

	halfVector = normalize(gl_LightSource[0].halfVector.xyz);

	diffuse = gl_Color * gl_FrontMaterial.diffuse *
		gl_LightSource[0].diffuse;
	ambient = gl_Color * gl_FrontMaterial.ambient *
		gl_LightSource[0].ambient;
	specular = gl_Color * gl_FrontMaterial.specular *
		gl_LightSource[0].specular;

	value = gl_LightSource[0].constantAttenuation;
	hivalue = gl_LightSource[0].quadraticAttenuation;
	specAdd = hivalue / (1.0+value);

	zNearFar = zNear*zFar;
	zFarPNear = zFar+zNear;
	zFarNNear = zFar-zNear;

//	zNearFar = 1.0*100000.0;
//	zFarPNear = 100000.0+1.0;
//	zFarNNear = 100000.0-1.0;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
