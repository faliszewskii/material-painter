//
// Created by Bartek Jadczak on 31/05/2024.
//
#ifndef DUCKONWATER_TEXTURE_COMMON_H
#define DUCKONWATER_TEXTURE_COMMON_H

class TextureCommon
{
public:
    static unsigned char* loadImage(char const *filename, int *x, int *y, int *comp, int req_comp);

    static void freeImage(void *retval_from_stbi_load);
};



#endif