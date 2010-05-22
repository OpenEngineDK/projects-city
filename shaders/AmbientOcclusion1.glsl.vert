varying vec2 uv;
 
void main(void)
{
    gl_Position = ftransform();
    gl_Position = sign( gl_Position );
    // Texture coordinate for screen aligned (in correct range):
    uv = (gl_Position.xy + vec2(1.0)) * 0.5;
}
