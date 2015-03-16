varying vec4 color;
varying vec2 texCoord0;

uniform sampler2D texUnit0;

void main()
{
    gl_FragColor = color * texture2D(texUnit0, texCoord0);
}
