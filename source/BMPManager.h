#ifndef BITMAP_H
#define BITMAP_H
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include <vector>
#include <string>

namespace bmp {
    std::vector<std::vector<int>> readBitmap(std::string filename);
    void writeBitmap(std::vector<std::vector<int>> pixels, std::string filename);
}

#endif