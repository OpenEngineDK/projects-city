varying vec3 normal;
/* uniform vec3 camPos, camDir; */
/* uniform float depthNear, depthFar; */

/* varying float camDepth; */

void main(void)
{
    gl_Position = ftransform();

    /* vec3 offset = (gl_Vertex.xyz / gl_Vertex.w) - camPos; */
    /* float z = -dot(offset, camDir); */
    /* camDepth = (z - depthNear) / (depthFar - depthNear); */

    normal = gl_NormalMatrix * gl_Normal;
}
