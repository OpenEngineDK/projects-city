//uniform vec3 eyepos;
varying vec3 normal;
//varying float depth;

void main(void)
{
    //  depth = distance(eyepos, gl_Vertex);
    gl_Position = ftransform();
    normal = gl_NormalMatrix * gl_Normal;
}
