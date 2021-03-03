varying vec2 pos;
uniform float time;
uniform sampler2D ourTexture;
void main(){
    float ntime = time*0.003;
    gl_FragColor=vec4(0.5*(pos.x*cos(ntime)-pos.y*sin(ntime))+0.5,0.5*(pos.x*sin(ntime)+pos.y*cos(ntime))+0.5,1,1)*texture2D(ourTexture, 0.5*pos+0.5);
}