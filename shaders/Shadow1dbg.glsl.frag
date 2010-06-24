uniform sampler2DShadow dtex;
//uniform sampler2D dtex;
uniform sampler2D ttex;

varying vec2 uv;
varying vec4 shadowCoord;

void main(void) {
 
    
    vec3 c;
    c.xy = uv;
    gl_FragColor.rgb = shadow2D(dtex, c).rrr;
    //gl_FragColor.rb = shadowCoord.xy;

    //gl_FragColor.gb = texture2D(ttex, uv).gb;
}
