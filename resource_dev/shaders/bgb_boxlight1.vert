varying vec4 diffuse, ambient, specular;
varying vec3 lorg, halfVector;

varying vec3 point, normal;

void main()
{
	point = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);
	lorg = vec3(gl_LightSource[0].position);

	halfVector = normalize(gl_LightSource[0].halfVector.xyz);

	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	specular = gl_FrontMaterial.specular * gl_LightSource[0].specular;

	gl_Position = ftransform();
}
