uniform mat4 mvp;
uniform mat4 mv;

attribute vec4 vPosition;
attribute vec2 vTexCoord0;
attribute vec4 vNormal;

varying vec2 texCoord0;
varying float lightDot;

void main()
{
    gl_Position = mvp * vPosition;
    texCoord0 = vTexCoord0;
    vec4 normal = vNormal;
    normal.w = 0.0;
    lightDot = max(dot(normalize(mv * normal).xyz, normalize(vec3(1, 1, 10))), 0.0) * 2.0;
}
