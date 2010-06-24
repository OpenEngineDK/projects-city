uniform sampler2D ao;
varying vec2 uv;
 
const float blurSize = 1.0/(800.0);
 
void main(void)
{
    /* gl_FragColor = vec4(texture2D(ao, uv).x); */
    /* return; */
    vec4 sum = vec4(0.0);
 
   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture2D(ao, vec2(uv.x - 4.0*blurSize, uv.y)) * 0.05;
   sum += texture2D(ao, vec2(uv.x - 3.0*blurSize, uv.y)) * 0.09;
   sum += texture2D(ao, vec2(uv.x - 2.0*blurSize, uv.y)) * 0.12;
   sum += texture2D(ao, vec2(uv.x - blurSize, uv.y)) * 0.15;
   sum += texture2D(ao, vec2(uv.x, uv.y)) * 0.16;
   sum += texture2D(ao, vec2(uv.x + blurSize, uv.y)) * 0.15;
   sum += texture2D(ao, vec2(uv.x + 2.0*blurSize, uv.y)) * 0.12;
   sum += texture2D(ao, vec2(uv.x + 3.0*blurSize, uv.y)) * 0.09;
   sum += texture2D(ao, vec2(uv.x + 4.0*blurSize, uv.y)) * 0.05;
 
   gl_FragColor = vec4(sum.x);
}
