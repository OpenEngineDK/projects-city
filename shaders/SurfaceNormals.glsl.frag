varying vec3 normal;
varying vec3 eyePos;

void main(void)
{
    gl_FragColor.rgb = normalize(cross(dFdx(eyePos), dFdy(eyePos)));
    //gl_FragColor.rgb = normalize(normal); 
}
