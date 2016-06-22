uniform sampler2D colorMap;
uniform sampler2D noiseMap;
uniform float time;

void main()
{
	vec3 noiseVec;
	vec2 displacement;
	float scaledTimer;

	displacement = gl_TexCoord[0].st;

	scaledTimer = time*0.1;

	displacement.x += scaledTimer;
	displacement.y -= scaledTimer;

	noiseVec = normalize(texture2D(noiseMap, displacement.xy)).xyz;
	noiseVec = (noiseVec * 2.0 - 1.0) * 0.035;
	
	gl_FragColor = texture2D(colorMap, gl_TexCoord[0].st + noiseVec.xy);
}

