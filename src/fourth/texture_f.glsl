#version 330 core

in vec3 oColor;
in vec2 oTexCoord;

out vec4 oFragColor;

uniform sampler2D uTexture;

void main() {
    oFragColor = texture(uTexture, oTexCoord);
}