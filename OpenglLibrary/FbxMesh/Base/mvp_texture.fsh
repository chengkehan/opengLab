varying vec2 texCoord0;
varying float lightDot;

uniform sampler2D texUnit;

void main()
{
    vec4 c = texture2D(texUnit, texCoord0);
    gl_FragColor = c * lightDot;
}
