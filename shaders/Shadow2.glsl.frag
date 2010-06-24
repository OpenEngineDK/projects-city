
uniform sampler2DShadow ShadowMap;
uniform float ShadowAmount;

varying vec4 ShadowCoord;

varying vec3 Normal, LightDir, HalfVector;
varying float LightDistance;
varying vec4 Diffuse, Ambient, GlobalAmbient;

float lookup(float x, float y) {
    return shadow2DProj(ShadowMap,ShadowCoord + vec4(x,y,0,0)).r < 1.0 ? ShadowAmount : 1.0;
}


void main() {
    vec4 color;

    float sd=0.017;

    float shadow = lookup(0.0,0.0);
    shadow += lookup( sd,  sd);
    shadow += lookup( sd, -sd);
    shadow += lookup(-sd,  sd);
    shadow += lookup(-sd, -sd);

    shadow += lookup(0.0, -sd);
    shadow += lookup(0.0,  sd);
    shadow += lookup(-sd, 0.0);
    shadow += lookup( sd, 0.0);

    shadow /= 9.0;
   
    color = GlobalAmbient + Ambient;
    
    vec3 n = normalize(Normal); // it's interpolated, we need to normalize it!
    float NdotL = max(dot(n, normalize(LightDir)), 0.0);
    if (NdotL > 0.0) {
        float att = 1.0 / (gl_LightSource[0].constantAttenuation +
                           gl_LightSource[0].linearAttenuation * LightDistance + 
                           gl_LightSource[0].quadraticAttenuation * LightDistance * LightDistance);
        color += att * Diffuse * NdotL;
        vec3 halfV = normalize(HalfVector);
        float NdotHV = max(dot(n, halfV), 0.0);
        color += att * gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV, gl_FrontMaterial.shininess);
    }

    color *= shadow;
    color.a = 1.0;

  	gl_FragColor =	 color;
    
}
