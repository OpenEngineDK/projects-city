varying vec4 ShadowCoord;

varying vec3 Normal, LightDir, HalfVector;
varying float LightDistance;
varying vec4 Diffuse, Ambient, GlobalAmbient;

                

void main(void) {

    ShadowCoord = gl_TextureMatrix[7] * gl_Vertex;

    
    vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;
    //vec3 ecPosition3 = (vec3(ecPosition))/ecPosition.w;

    vec3 aux = vec3(gl_LightSource[0].position - ecPosition);


    Normal = normalize(gl_NormalMatrix * gl_Normal);
    
    LightDir = normalize(aux);
    LightDistance = length(aux);
    HalfVector = normalize(gl_LightSource[0].halfVector.xyz);
    
    Diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
    Ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    GlobalAmbient = gl_LightModel.ambient * gl_FrontMaterial.ambient;


    gl_Position = ftransform();
}

