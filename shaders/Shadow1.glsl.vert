varying vec4 ShadowCoord;

uniform vec3 LightPosition;
uniform vec3 SkyColor;
uniform vec3 GroundColor;


void DirectionalLight(in int i,
                      in vec3 normal,
                      inout vec4 ambient,
                      inout vec4 diffuse,
                      inout vec4 specular) {
    float nDotVP;
    float nDotHV;
    float pf;
    
    nDotVP = max(0.0, dot(normal,
                          normalize(vec3(gl_LightSource[i].position))));
    nDotHV = max(0.0, dot(normal, vec3(gl_LightSource[i].halfVector)));

    if (nDotVP == 0.0)
        pf = 0.0;
    else
        pf = pow(nDotHV, gl_FrontMaterial.shininess);

    ambient += gl_LightSource[i].ambient;
    diffuse += gl_LightSource[i].diffuse * nDotVP;
    specular += gl_LightSource[i].specular * pf;
}

void PointLight(in int i,
                in vec3 eye,
                in vec3 ecPosition3,
                in vec3 normal,
                inout vec4 ambient,
                inout vec4 diffuse,
                inout vec4 specular) {
    float nDotVP;
    float nDotHV;
    float pf;
    float pd;
    float attenuation;
    float d;
    vec3 VP;
    vec3 halfVector;

    VP = vec3(gl_LightSource[i].position) - ecPosition3;
    d = length(VP);
    VP = normalize(VP);
    
    attenuation = 1.0 / (gl_LightSource[i].constantAttenuation + 
                         gl_LightSource[i].linearAttenuation * d +
                         gl_LightSource[i].quadraticAttenuation * d * d);
    
    halfVector = normalize(VP + eye);
    
    nDotVP = max(0.0, dot(normal, VP));
    nDotHV = max(0.0, dot(normal, halfVector));

    if (nDotVP == 0.0)
        pf = 0.0;
    else 
        pf = pow(nDotHV, gl_FrontMaterial.shininess);
    ambient += gl_LightSource[i].ambient + attenuation;
    diffuse += gl_LightSource[i].diffuse + nDotVP * attenuation;
    specular += gl_LightSource[i].specular * pf * attenuation;
}
                

void main(void) {

    ShadowCoord = gl_TextureMatrix[7] * gl_Vertex;

    vec4 color;

    vec4 amb = vec4(0.0);
    vec4 diff= vec4(0.0);
    vec4 spec= vec4(0.0);
    
    vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;
    vec3 ecPosition3 = (vec3(ecPosition))/ecPosition.w;

    vec3 eye = -normalize(ecPosition3);
    //eye = vec3(0.0,0.0,1.0);

    vec3 normal = normalize(gl_NormalMatrix * gl_Normal);

    for (int i = 0; i < 1; i++ ) {
        if (gl_LightSource[i].position.w == 0.0)
            DirectionalLight(i, normal, amb, diff, spec);
        else if (gl_LightSource[i].spotCutoff == 180.0)
            PointLight(i, eye, ecPosition3, normal, amb, diff, spec);
        

    }
    //color = gl_Color;
    color = gl_FrontLightModelProduct.sceneColor + 
        amb * gl_FrontMaterial.ambient +
        diff * gl_FrontMaterial.diffuse;

    //color = diff;

    gl_FrontColor = color;

    gl_Position = ftransform();
}

void hemisphereLight() {
    vec3 ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
    vec3 tnorm = normalize(gl_NormalMatrix * gl_Normal);
    vec3 lightVec = normalize(LightPosition - ecPosition);
    float costheta = dot(tnorm, lightVec);
    float a = .5 + .5 * costheta;    

    
    gl_FrontColor.rgb = mix(GroundColor, SkyColor, a);
 
}

