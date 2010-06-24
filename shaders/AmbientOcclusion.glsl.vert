varying vec2 uv;

void main(void)
{
    gl_Position = ftransform();
    //uv = gl_MultiTexCoord0.xy;
    uv = (gl_Position.xy + vec2(1.0)) * 0.5; 
}
