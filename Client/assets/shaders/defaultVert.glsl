attribute vec4 vertexBuf;
varying vec2 pos;
void main(){
    gl_Position=vertexBuf;
    pos = vec2(vertexBuf.x,vertexBuf.y);
}