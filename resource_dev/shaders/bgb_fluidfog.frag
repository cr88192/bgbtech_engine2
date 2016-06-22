varying vec4 color;

void main(void)
{
	float att;

//	att=gl_FragCoord.z/4;
//	att=1;
	att=(1.0/gl_FragCoord.w)*color.a;
	gl_FragColor = vec4(color.rgb, att);
}
