varying vec2 uv;

uniform sampler2D normals;
uniform sampler2DShadow d;
uniform mat4 proj;
uniform mat4 unproj;

uniform float sphereRad;
uniform float linearAtt;
uniform float contrast;
uniform float rays;
uniform float steps;
uniform float bias;
uniform float rand;

const float PI = 3.14159265;

vec2 uv2win(vec2 uv) {
    return (uv * 2.0) - vec2(1.0);
}

vec2 win2uv(vec2 win) {
    return (win + vec2(1.0)) * 0.5;
}

// pos must be in [-1;1] and depth in [0;1]
vec3 unproject(vec2 pos, float depth) {
    vec4 res = unproj * vec4(pos, depth, 1.0);
    return res.xyz / res.w;
}

vec2 project(vec3 pos) {
    vec4 res = proj * vec4(pos, 1.0);
    return res.xy / res.w;
}

float att(float r) {
    /* if (r > 1.0) return 0.0; */
    return 1.0 / r*r*linearAtt;
}

void main(void)
{
    vec2 winPos  = uv2win(uv); // window coordinates [-1;1]
    float depth  = shadow2D(d, vec3(uv,0.0)).x;
    vec3 normal  = normalize(texture2D(normals,uv).xyz);
    vec3 fragPos = unproject(winPos, depth);

    float circleRad = length(winPos - project(fragPos + vec3(sphereRad, 0.0, 0.0))); // project the sphere radius onto the screen
    float stepSize = circleRad / steps; 
    
    float ao = 0.0; 
    // calculate the horizon for each ray
    float baseAngle = 2.0*PI*rand;
    float stepAngle = (2.0*PI) / rays;
    float angle = baseAngle;
    for (float i = 0.0; i < rays; i = i + 1.0) {
        vec2 stepRay = vec2(cos(angle)*stepSize, sin(angle)*stepSize);
        vec2 pos = winPos + stepRay;  
        vec2 maxPos = pos;
        float maxDepth = shadow2D(d, vec3(win2uv(pos), 0.0)).x;

        vec3 horizon = unproject(maxPos, maxDepth) - fragPos;
        float r = length(horizon) / sphereRad; 

        // calculate tangent to the surface normal
        vec3 tang  = normalize(horizon);
        vec3 bitan = cross(normal, tang);
        tang       = cross(bitan, normal);

        // calculate angles in [-PI; PI]
        float tAngle = atan(tang.z / length(tang.xy)) + bias;
        float hAngle = atan(horizon.z / length(horizon.xy));
        
        // final ao contribution of this direction
        float pao = sin(hAngle) - sin(tAngle);
        ao +=  pao * att(r);
                     
        for (float j = 1.0; j < steps; j = j + 1.0) {
            pos += stepRay;
            float d = shadow2D(d, vec3(win2uv(pos), 0.0)).x;
            vec3 s = unproject(pos, d) - fragPos;
            float a = atan(s.z / length(s.xy));
            if (a > hAngle) {
                maxDepth = d;
                maxPos = pos;
                horizon = unproject(maxPos, maxDepth) - fragPos;
                r = length(horizon) / sphereRad; 
                hAngle = atan(horizon.z / length(horizon.xy));
                float nao = sin(hAngle) - sin(tAngle);
                ao += (nao - pao) * min(att(r), 1.0);
                pao = nao;
            }
        }
        angle += stepAngle;
    }

    // average ao and multiply with contrast
    gl_FragColor.r = max(min(1.0 - (ao/rays) * contrast,1.0),0.0);
}
