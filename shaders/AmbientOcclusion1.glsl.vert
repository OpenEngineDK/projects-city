varying vec2 uv;
/* uniform vec3 camPos; */

/* varying vec2 p;  */
void main(void)
{
    gl_Position = ftransform();
    uv = gl_MultiTexCoord0.xy;//(gl_Position.xy + vec2(1.0)) * 0.5; */

    /* camPos = (gl_ModelViewProjectionMatrix * vec4(camPos,1.0)).xyz; */
}
