#version 330 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iColor;
layout(location = 2) in vec2 iTexCoord;

out vec3 oColor;
out vec2 oTexCoord;

void main() {
    gl_Position = vec4(iPos, 1.0f);
    oColor = iColor;
    oTexCoord = iTexCoord;
}