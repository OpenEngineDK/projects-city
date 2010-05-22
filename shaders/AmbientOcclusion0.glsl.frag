varying vec3 normal;
//varying float depth;
void main(void)
{
    gl_FragColor = vec4(normalize(normal), gl_FragDepth);
    //gl_FragColor = vec4(gl_FragDepth);
}
