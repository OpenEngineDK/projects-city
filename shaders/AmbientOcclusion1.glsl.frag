varying vec2 uv;
uniform sampler2D normals;

void main(void)
{
    const float rad = 0.3;
    const int steps = 3;
    const int rays = 4;
    const float PI = 3.14159265;
    const float distAngle = (2.0*PI) / rays;
    const float offsetAngle = PI/6.0;

    vec4 texel = texture2D(normals,uv);
    float depth = texel.a;
    vec3 normal = normalize(texel.xyz);

    float ao = 0.0; 
    for (int i = 0; i < rays; ++i) {
        float angle = offsetAngle + distAngle * i;
        vec2 stepRay = vec2(cos(angle), sin(angle));
        float step = length(normal) / steps;
        stepRay = normalize(stepRay) * step;
        vec3 horizon = vec3(uv,depth);
        vec2 pos = uv;        
        for (int j = 0; j < steps; j = ++j) {
            pos += stepRay;
            float d =  texture2D(normals,pos.xy).a;
            if (d > horizon.z) {
                horizon = vec3(pos,d);
            }
        }

        horizon = normalize(horizon - vec3(uv,depth));
        float hAngle = atan(horizon.z, length(horizon.xy));
        vec3 rotvec = normalize(cross(horizon, normal));

        const float x = rotvec.x;
        const float y = rotvec.y;
        const float z = rotvec.z;
        const float w = PI * 0.5;

        /* mat3 rotmat = mat3(w*w+x*x-y*y-z*z, 2*x*y+2*w*z,    2*x*z-2*w*y, */
        /*                    2*x*y-2*w*z,     w*w-x*x+y*y-z*z, 2*y*z+2*w*x, */
        /*                    2*x*z+2*w*y,     2*y*z-2*w*x,    w*w-x*x-y*y+z*z); */
                           
        const mat3 rotmat = mat3(w*w+x*x-y*y-z*z, 2*x*y-2*w*z,     2*x*z+2*w*y,
                           2*x*y+2*w*z,     w*w-x*x+y*y-z*z, 2*y*z-2*w*x,
                           2*x*z-2*w*y,     2*y*z+2*w*x,     w*w-x*x-y*y+z*z);

        vec3 t = normalize(rotmat * normal);
        float tAngle = atan(t.z, length(t.xy));
        ao += (sin(hAngle)-sin(tAngle));
    }
    gl_FragColor = 1-(ao/rays);
}
