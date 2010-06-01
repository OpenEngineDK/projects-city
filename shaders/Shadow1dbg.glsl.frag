//uniform sampler2DShadow dtex;
uniform sampler2D dtex;
uniform sampler2D ttex;

varying vec2 uv;
// varying vec4 shadowCoord;

void main(void) {
 
    //gl_FragColor = texture2D(ttex, uv);
    gl_FragColor = texture2D(dtex, uv);

}
