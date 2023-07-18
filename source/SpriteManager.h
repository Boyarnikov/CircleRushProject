#ifndef BITMAP_H
#define BITMAP_H
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include <vector>
#include <string>

namespace spr {
    struct sprite {
        // NOTE: have no time to finish sprite structure, used only to store font values
        unsigned int width, height;
        unsigned int spr_width, spr_height;
        std::vector<std::vector<unsigned int>> pixels;

        sprite(std::string filename, unsigned int spr_width, unsigned int spr_height);
        sprite();
        // TODO: add functions to get sprite frame position on the spritesheet by frame index.
        // TODO: add sprite frame count
    };
}

#endif