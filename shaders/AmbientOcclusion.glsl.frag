varying vec2 uv;

uniform sampler2D normals;
uniform sampler2DShadow d;
uniform mat4 proj;
uniform mat4 unproj;

uniform float sphereRad;
//uniform float linearAtt;
uniform float contrast;
uniform float rays;
uniform float steps;
uniform float bias;

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

void main(void)
{
    const float offsetAngle = PI/10.0;
    float distAngle = (2.0*PI) / rays;
    vec4 sample = texture2D(normals,uv);
    
    vec2 winPos  = uv2win(uv); // window coordinates [-1;1]
    float depth  = shadow2D(d, vec3(uv,0.0)).x;
    vec3 normal  = normalize(sample.xyz);
    vec3 fragPos = unproject(winPos, depth);

    float circleRad = length(winPos - project(fragPos + vec3(sphereRad, 0.0, 0.0))); // project the sphere radius onto the screen
    float stepSize = circleRad / steps;
    
    float ao = 0.0; 
    // calculate the horizon for each ray
    for (float i = 0.0; i < rays; i = i + 1.0) {
        float angle = offsetAngle + distAngle * i;
        vec2 stepRay = vec2(cos(angle), sin(angle));
        stepRay = stepRay * stepSize;
        vec3 horizon = fragPos;
        vec2 pos = winPos;        
        for (float j = 0.0; j < steps; j = j + 1.0) {
            pos += stepRay;
            vec3 new = unproject(pos, shadow2D(d, vec3(win2uv(pos), 0.0)).x);
            if (new.z < horizon.z) {
                horizon = new;
            }
        }
        horizon = horizon - fragPos;
        float r = length(horizon) / sphereRad; 

        // calculate tangent to the surface normal
        horizon = normalize(horizon);
        vec3 tan   = vec3(1,0,0);
        vec3 bitan = normalize(cross(normal, tan));
        tan        = normalize(cross(bitan, normal));

        // calculate angles in [-PI; PI]
        float hAngle = atan(horizon.z / length(horizon.xy));
        float tAngle = atan(tan.z / length(tan.xy)) + bias;
        
        // final ao contribution of this direction
        ao += (sin(hAngle) - sin(tAngle));// * linearAtt * (1.0 - r * r);
    }

    // average ao and multiply with contrast
    gl_FragColor = vec4(1.0 - (ao/rays) * contrast);
}
