#version 330 core

in vec3 iColor;
in vec3 iTexCoord;

out vec4 oFragColor;

uniform sampler2D uTexture;

void main() {
    oFragColor = texture(uTexture, iTexCoord);
}