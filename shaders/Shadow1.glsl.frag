uniform sampler2DShadow ttex;
//uniform sampler2D ttex;

varying vec2 uv;
varying vec4 shadowCoord;

void main(void) {
 
    float depth = shadow2DProj(ttex, shadowCoord).x;
    if (depth < 1.0)
        gl_FragColor.r = 1.0;

    //gl_FragColor.r = texture2D(ttex, uv).x;

}
