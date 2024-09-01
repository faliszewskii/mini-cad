//
// Created by faliszewskii on 06.05.24.
//

#include <vector>
#include "Texture.h"
#include "../entities/shader/Shader.h"

Texture::Texture(int x, int y, int nrChannels, int internalFormat, int format, int type, int target) {
    width = x;
    height = y;
    this->format = format;
    this->internalFormat = internalFormat;
    this->target = target;
    this->nrChannels = nrChannels;
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(target, id);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // TODO DEBUG
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    switch (target) {
        case GL_TEXTURE_2D:
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, format, type, nullptr);
            break;
        default:
            break;
    }
    glBindImageTexture(0, id, 0, GL_FALSE, 0, GL_READ_WRITE, format);

    // glGenerateMipmap(id);
}

void Texture::bind(int slot) const {
    glBindTextureUnit(slot, id);
}

void Texture::bindImage(int slot) const {
    glBindTextureUnit(slot, id);
    glBindImageTexture(slot, id, 0, GL_FALSE, 0, GL_READ_WRITE, internalFormat);
}

void Texture::update2D(void *data) const {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexSubImage2D(
        target,
        0,
        0,
        0,
        width,
        height,
        format,
        GL_FLOAT,
        data
    );
}

void Texture::update3D(void *data) const {
    glBindTexture(target, id);
    glTexSubImage3D(
            target,
            0,
            0,
            0,
            0,
            width,
            height,
            depth,
            format,
            GL_FLOAT,
            data
    );
}

Texture::Texture(int x, int y, int z, int nrChannels, int internalFormat, int format, int type, int target) {
    width = x;
    height = y;
    depth = z;
    this->format = format;
    this->internalFormat = internalFormat;
    this->target = target;
    this->nrChannels = nrChannels;
    glGenTextures(1, &id);
    glBindTexture(target, id);

    switch (target) {
        case GL_TEXTURE_3D:
            glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, x, y, z, 0, format, type, nullptr);
            break;
        default:
            break;
    }

    glGenerateMipmap(id);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_REPEAT);
}

void Texture::updatePixel(int x, int y, void *data) const {
    glBindTexture(target, id);
    glTexSubImage2D(
            target,
            0,
            x,
            y,
            1,
            1,
            format,
            GL_FLOAT,
            data
    );
}