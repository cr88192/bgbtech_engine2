uniform vec4 leftColor;
uniform vec4 rightColor;

invariant varying float colorGammaK;
invariant varying vec4 colorGamma;
invariant varying vec4 leftColorGammaWeight;
invariant varying vec4 rightColorGammaWeight;
invariant varying vec4 leftColorGammaAdjust;
invariant varying vec4 rightColorGammaAdjust;

//varying vec2 st0;

void main()
{
	colorGamma=vec4(0.35, 0.50, 0.15, 0.00);
	colorGammaK=dot(colorGamma, vec4(1.0, 1.0, 1.0, 1.0));

	leftColorGammaWeight=leftColor*colorGamma;
	rightColorGammaWeight=rightColor*colorGamma;
	leftColorGammaAdjust=leftColor*
		(colorGammaK*0.10/dot(leftColor, colorGamma));
	rightColorGammaAdjust=rightColor*
		(colorGammaK*0.10/dot(rightColor, colorGamma));
//	st0 = gl_MultiTexCoord0.st;
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
