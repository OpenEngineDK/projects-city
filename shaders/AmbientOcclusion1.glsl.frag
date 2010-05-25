varying vec2 uv;

uniform sampler2D normals;
uniform sampler2D scene; 

uniform float near, far;

const float win_w = 800.0;
const float win_h = 600.0;

vec2 uv2win(vec2 uv) {
    return vec2(floor(uv.x * win_w), floor((uv.y) * win_h));
}

vec2 win2uv(vec2 win) {
    return vec2(floor(win.x) / win_w, (floor(win.y) / win_h));
}

vec3 unproject(vec2 pos, float depth) {
    // @todo: how to do correct unprojection?
    return vec3(pos, depth);

    /* vec3 dir = vec3(pos, near); */
    /* vec3 p = dir + normalize(dir) * (far-near)*depth; */
    //return p;
}

void main(void)
{
    const float PI = 3.14159265;

    const float linearAtt = .89;
    const float contrast = 0.785; 
    const float bias = PI/7.0;
    const float rad = 20;
    const float wrad = 30;
    const int steps = 10;
    const int rays = 20;

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
    vec3 normal  = normalize(sample.xyz);
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
            vec3 new = unproject(pos, d);
            if (length(fragPos-new) > wrad) break;
            if (new.z < horizon.z) {
                horizon = new;//unproject(pos, d);
            }
        }
        horizon = horizon - fragPos;
        float r = length(horizon)/wrad; 
        if (r == 0.0) {
            continue;
        }
        horizon = normalize(horizon);
        float hAngle = atan(horizon.z / length(horizon.xy));
        vec3 rotvec = normalize(cross(normal, horizon));

        float x = rotvec.x;
        float y = rotvec.y;
        float z = rotvec.z;
        const float w = (PI * 0.5); // tangent angle
                           
        mat3 rotmat = mat3(w*w+x*x-y*y-z*z, 2.0*x*y-2.0*w*z, 2.0*x*z+2.0*w*y,
                           2.0*x*y+2.0*w*z, w*w-x*x+y*y-z*z, 2.0*y*z-2.0*w*x,
                           2.0*x*z-2.0*w*y, 2.0*y*z+2.0*w*x, w*w-x*x-y*y+z*z);
        // rotate normal 90+bias degrees to construct tangent
        vec3 tan   = vec3(1,0,0);
        vec3 bitan = normalize(cross(normal, tan));
        tan        = normalize(cross(bitan, normal));

        vec3 t = tan; //normalize(rotmat * normal);
        float tAngle = atan(t.z / length(t.xy)) + bias;
        //ao += dot(normal, horizon);
        ao += (sin(hAngle) - sin(tAngle)) * linearAtt * (1.0 - r*r);
    }
    //gl_FragColor = vec4(uv.x, uv.y,0,1);
    gl_FragColor = (1-(ao/float(rays))) * contrast;
    
    //gl_FragColor = texture2D(scene,uv) * (1.0 - (ao/float(rays)));
    //gl_FragColor *= texture2D(scene,uv);
    //gl_FragColor = texture2D(normals,uv);
}
