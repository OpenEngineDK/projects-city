varying vec2 uv;
varying vec2 p; 
void main(void)
{
    gl_Position = ftransform();
    p = gl_Vertex.xy;
    /* gl_Position = sign( gl_Position ); */
    // Texture coordinate for screen aligned (in correct range):
    //    gl_TexCoord[0] = gl_MultiTexCoord0;
    uv = gl_MultiTexCoord0.xy;//(gl_Position.xy + vec2(1.0)) * 0.5; */
}
