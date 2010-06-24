
uniform sampler2DShadow ShadowMap;
uniform float ShadowAmount;

varying vec4 ShadowCoord;


void main() {
	float distanceFromLight = shadow2DProj(ShadowMap,ShadowCoord).r;
    float shadow = 1.0;
    if (distanceFromLight < 1.0)
        shadow = ShadowAmount;



  	gl_FragColor =	 shadow * gl_Color;
    
}
