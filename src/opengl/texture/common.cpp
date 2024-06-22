//
// Created by Bartek Jadczak on 31/05/2024.
//
#include "common.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../dep/stb/stb_image.h"

unsigned char *TextureCommon::loadImage (const char *filename, int *x, int *y, int *comp, int req_comp) {
    return stbi_load(filename, x, y, comp, req_comp);
}

void TextureCommon::freeImage (void *retval_from_stbi_load) {
    stbi_image_free(retval_from_stbi_load);
}
