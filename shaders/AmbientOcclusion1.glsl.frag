varying vec2 uv;
varying vec2 p;
uniform sampler2D normals;
//uniform sampler2D depths; 

const float win_w = 800.0;
const float win_h = 600.0;

vec2 uv2win(vec2 uv) {
    return vec2(floor(uv.x * win_w), floor(uv.y * win_h));
}

vec2 win2uv(vec2 win) {
    return vec2(floor(win.x) / win_w, floor(win.y) / win_h);
}

vec3 unproject(vec2 pos, float depth) {
    //return vec3(pos, depth);
    return vec3((gl_ModelViewProjectionMatrixInverse * vec4(floor(pos),0,0)).xy, depth);
}

void main(void)
{
    const float rad = 5.0;
    const int steps = 6;
    const int rays = 6;
    const float PI = 3.14159265;
    const float distAngle = (2.0*PI) / float(rays);
    const float offsetAngle = PI/6.0;

    vec4 sample = texture2D(normals,uv);
    // if there is no no geometry in this fragment then we do nothing
    if (length(sample.xyz) == 0.0) {
        gl_FragColor = vec4(1.0);
        return;
    }

    vec2 winPos  = uv2win(uv);     // window coordinates
    float depth  = sample.a;
    vec3 normal  = sample.xyz;
    vec3 fragPos = unproject(winPos, depth);

    float ao = 0.0; 
    for (int i = 0; i < rays; ++i) {
        float angle = offsetAngle + distAngle * float(i);
        vec2 stepRay = vec2(cos(angle), sin(angle));
        float stepSize = rad / float(steps);
        stepRay = stepRay * stepSize;
        vec3 horizon = fragPos;
        vec2 pos = winPos;        
        for (int j = 0; j < steps; j = ++j) {
            pos += stepRay;
            float d = texture2D(normals, win2uv(pos)).a;
            if (d < horizon.z) {
                horizon = unproject(pos, d);
            }
        }
        horizon = horizon - fragPos;
        if (length(horizon) == 0.0) {
            continue;
        }
        horizon = normalize(horizon);
        float hAngle = atan(horizon.z, length(horizon.xy));
        vec3 rotvec = normalize(cross(normal, horizon));

        float x = rotvec.x;
        float y = rotvec.y;
        float z = rotvec.z;
        const float w = PI * 0.5;// + PI/6;

        /* mat3 rotmat = mat3(w*w+x*x-y*y-z*z, 2*x*y+2*w*z,    2*x*z-2*w*y, */
        /*                    2*x*y-2*w*z,     w*w-x*x+y*y-z*z, 2*y*z+2*w*x, */
        /*                    2*x*z+2*w*y,     2*y*z-2*w*x,    w*w-x*x-y*y+z*z); */
                           
        mat3 rotmat = mat3(w*w+x*x-y*y-z*z, 2.0*x*y-2.0*w*z, 2.0*x*z+2.0*w*y,
                           2.0*x*y+2.0*w*z, w*w-x*x+y*y-z*z, 2.0*y*z-2.0*w*x,
                           2.0*x*z-2.0*w*y, 2.0*y*z+2.0*w*x, w*w-x*x-y*y+z*z);

        // rotate normal 90+bias degrees to construct tangent
        vec3 t = normalize(rotmat * normal);
        float tAngle = atan(t.z, length(t.xy));
        //ao += dot(normal, horizon);
        ao += sin(hAngle) - sin(tAngle);
    }
    gl_FragColor = vec4(1.0 - (ao/float(rays)));
}
