varying vec2 uv;

void main(void)
{
    //gl_Position = ftransform();
    uv = gl_MultiTexCoord0.xy;
    gl_Position = vec4((uv * 2.0) - vec2(1.0,1.0), 0.0, 1.0);

}
