#version 120

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

void main()
{
	point = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);
	lorg = vec3(gl_LightSource[0].position);

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

//	gl_Position = (gl_ProjectionMatrix * gl_ModelViewMatrix) * gl_Vertex; 
//	gl_Position = (gl_ModelViewMatrix * gl_ProjectionMatrix) * gl_Vertex; 
//	v = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
//	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	gl_Position = ftransform();
}
