varying vec3 normal;
varying vec3 eyePos;

void main(void)
{
    eyePos = (gl_ModelViewMatrix * gl_Vertex).xyz;
    gl_Position = ftransform();
    normal = normalize(gl_NormalMatrix * gl_Normal);
}
