//
// Created by faliszewskii on 17.06.24.
//

#ifndef MATERIAL_PAINTER_TEXTURE_H
#define MATERIAL_PAINTER_TEXTURE_H

#include <string>

class Texture {


    unsigned int id{};
    int nrChannels;
    int target, format, internalFormat;

public:
    explicit Texture(const std::string& path, bool isSRGB);
    explicit Texture(const std::string& path, int repeat);
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


#endif //MATERIAL_PAINTER_TEXTURE_H
