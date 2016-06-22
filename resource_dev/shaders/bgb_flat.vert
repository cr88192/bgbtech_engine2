uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

// attribute vec2 texCoord; 
attribute vec4 color; 
attribute vec4 vertex; 

varying  vec4 v_color; 
// varying  vec2 v_texCoord; 

void main() 
{
	v_color = gl_Color;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex; 

//	v_color = color;
//	v_texCoord = texCoord;
//	gl_Position = projectionMatrix * modelViewMatrix * vertex; 
}