varying vec3 normal;
//varying float camDepth;
void main(void)
{
    gl_FragColor = vec4(normalize(normal), gl_FragCoord.z);
    //gl_FragColor = vec4(gl_FragDepth);
}
