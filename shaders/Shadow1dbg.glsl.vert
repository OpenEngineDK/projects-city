varying vec2 uv;

varying vec4 shadowCoord;

void main(void) {
    uv = gl_MultiTexCoord0.xy;

    vec4 texCoord = gl_TextureMatrix[1] * gl_Vertex;
    shadowCoord = texCoord / texCoord.w;

    gl_Position = ftransform();    
}
