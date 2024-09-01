//
// Created by faliszewskii on 06.05.24.
//

#ifndef PUMAROBOT_TEXTURE_H
#define PUMAROBOT_TEXTURE_H


#include <string>

class Texture {


    int nrChannels;
    int target, format, internalFormat;

public:
    unsigned int id{};
    Texture(int x, int y, int nrChannels, int internalFormat, int format, int type, int target);
    Texture(int x, int y, int z, int nrChannels, int internalFormat, int format, int type, int target);


    void bind(int slot) const;
    void bindImage(int slot) const;
    void update2D(void* data) const;
    void updatePixel(int x, int y, void* data) const;
    void update3D(void *data) const; // TODO refactor to be abstract texture
    int width;
    int height;
    int depth;
};


#endif //PUMAROBOT_TEXTURE_H