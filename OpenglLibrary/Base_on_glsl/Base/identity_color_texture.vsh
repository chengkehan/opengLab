attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec2 vTexCoord0;

varying vec4 color;
varying vec2 texCoord0;

void main()
{
    gl_Position = vPosition;
    color = vColor;
    texCoord0 = vTexCoord0;
}
