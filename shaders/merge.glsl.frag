uniform sampler2D ao;
uniform sampler2D scene;
varying vec2 uv;
 
void main(void)
{
    gl_FragColor = texture2D(scene, vec2(uv.x, uv.y)) * texture2D(ao, vec2(uv.x, uv.y)).x;
    //gl_FragColor = vec4(texture2D(ao, vec2(uv.x, uv.y)).x);
}
