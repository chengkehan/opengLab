attribute vec4 vPosition;
attribute vec2 vTexCoord0;

varying vec2 texCoord0;

void main()
{
    gl_Position = vPosition;
    texCoord0 = vTexCoord0;
}
