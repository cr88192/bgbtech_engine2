uniform float time;
uniform float exposure;

uniform vec3 camOrg;
uniform vec3 camRotX;
uniform vec3 camRotY;
uniform vec3 camRotZ;

// varying vec3 vecSdir;
// varying vec3 vecTdir;
varying vec3 lorg, eyeOrg, eyeVec;
varying vec4 extColor;
// varying vec4 position;

varying vec3 point, normal, normal_x, normal_y;
varying vec3 eye_x, eye_y, eye_z;
// varying vec3 eyeVec;
varying mat3 eyeMat;

vec4 checkExtendColor(vec4 clr)
{
	float sc;
	int e;
	
	e=int(clr.a*255.0);
	if((e<0x70) || (e>0x8F))
		return(clr);
	sc=pow(2.0, float(e-0x7F));
	return(vec4(clr.rgb*sc, 1.0));
}

void main() 
{ 
//	mat3 eyeMat;
	vec4 mdlVec;
//	vec3 eyeOrg;
//	vec3 eye_x, eye_y, eye_z;
	vec3 lcl_z;

	point=vec3(gl_ModelViewMatrix * gl_Vertex);
	normal=normalize(gl_NormalMatrix * gl_Normal);

	if(true)
	{
		if(abs(normal.x)>abs(normal.y))
		{
			if(abs(normal.x)>abs(normal.z))
			{
				normal_x=vec3(0.0, 1.0, 0.0);
				normal_y=vec3(0.0, 0.0, 1.0);
				if(normal.x<0.0)
					normal_x=-normal_x;
			}else
			{
				normal_x=vec3(1.0, 0.0, 0.0);
				normal_y=vec3(0.0, 1.0, 0.0);
				if(normal.z<0.0)
					normal_x=-normal_x;
			}
		}else
		{
			if(abs(normal.y)>abs(normal.z))
			{
				normal_x=vec3(1.0, 0.0, 0.0);
				normal_y=vec3(0.0, 0.0, 1.0);
//				if(normal.y>0.0)
				if(normal.y<0.0)
					normal_x=-normal_x;
			}else
			{
				normal_x=vec3(1.0, 0.0, 0.0);
				normal_y=vec3(0.0, 1.0, 0.0);
				if(normal.z<0.0)
					normal_x=-normal_x;
			}
		}

		normal_y=-normal_y;

		//clamp
		normal_x=normalize(normal_x-(normal*dot(normal_x, normal)));
		normal_y=normalize(normal_y-(normal*dot(normal_y, normal)));

		eyeMat[0]=-normal_x;
		eyeMat[1]=-normal_y;
		eyeMat[2]=normal;
	}

//	eyeOrg=vec3(gl_ProjectionMatrixInverse * vec4(0.0, 0.0, 0.0, 1.0));
//	eyeOrg=vec3(gl_ProjectionMatrixInverse * vec4(0.0, 0.0, -0.1, 0.0));
//	eye_x=normalize(vec3(gl_ProjectionMatrixInverse * vec4(1.0, 0.0, 0.0, 0.0)));
//	eye_y=normalize(vec3(gl_ProjectionMatrixInverse * vec4(0.0, 1.0, 0.0, 0.0)));
//	eye_z=normalize(vec3(gl_ProjectionMatrixInverse * vec4(0.0, 0.0, 1.0, 0.0)));
//	eye_z=normalize(cross(eye_x, eye_y));
//	eye_z=normalize(cross(eye_y, eye_x));

	eyeOrg=camOrg;
	eye_x=camRotX;
	eye_y=camRotZ;
	eye_z=normalize(cross(eye_x, eye_y));

	lcl_z=normalize(vec3(0.0, 0.0, eye_y.z+0.01));

//	mdlVec=gl_ModelViewMatrix * gl_Vertex;
//	mdlVec=vec4(point, 0.0) +
//		vec4(gl_ProjectionMatrixInverse * vec4(0.0,0.0,-1.0,0.0));
//	mdlVec=vec4(point-eyeOrg, 0.0) +
//		vec4(gl_ProjectionMatrixInverse * vec4(0.0,0.0,-1.0,0.0));
//	mdlVec=vec4(point, 0.0) +
//		vec4(gl_ProjectionMatrixInverse * vec4(0.0,0.0,-1.0,1.0));

//	mdlVec=vec4(point-eyeOrg, 0.0)+vec4(-eye_z, 0.0);
//	eyeVec=vec3(-mdlVec) * eyeMat;      
//	eyeVec=(-mdlVec).xyz * eyeMat;      

//	eyeVec=point-eyeOrg;
//	eyeVec=(point-eyeOrg)*eyeMat;
//	eyeVec=(eyeOrg-point)*eyeMat;
//	eyeVec=((eyeOrg+eye_z)-point)*eyeMat;

	lorg=eyeOrg + vec3(0.0, 0.0, 1.0);
//	lorg=eyeOrg + 3.0*lcl_z;

	extColor=checkExtendColor(gl_Color);
	extColor=vec4(extColor.rgb*exposure, extColor.a);

//	gl_FrontColor=gl_Color;
	gl_TexCoord[0]=gl_MultiTexCoord0; 
	gl_Position=ftransform(); 
//	position=ftransform();
//	gl_Position=position;
}
