#include "SpriteManager.h"
#include <iostream>
#include <fstream>
#include <vector>

// i really should look into more save solution... Oh well!
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

// below are some logic for reading bmp files into vector<vector<unsigned int>>
// it's quite a low-tech solution, so i extract all info by hands
// as sprite rendering was my last-priority please note that it's a half-baked mess
int bint(std::vector<unsigned char> bytes) {
    int decimal = 0;
    for (int i = 0; i < bytes.size(); i++)
        decimal += bytes[i] << (i * 8);
    return decimal;
}

std::vector<unsigned char> ibytes(int decimal, int count) {
    std::vector<unsigned char> bytes(count);
    memcpy(&bytes[0], &decimal, sizeof decimal);
    return bytes;
}

int fdread(int count, FILE* file) {
    std::vector<unsigned char> bytes(count);
    fread(&bytes[0], 1, count, file);
    return bint(bytes);
}

// sprite constructor that reads a bmp file and creates a sprite structure
spr::sprite::sprite(std::string filename, unsigned int spr_width, unsigned int spr_height)
    : spr_width(spr_width), spr_height(spr_height) {

    FILE* file = fopen(filename.c_str(), "rb");

    // reading header
    fseek(file, 18, SEEK_SET); // seek to width / height
    width = fdread(4, file); // reading width
    height = fdread(4, file); // reading height

    // reading pixels
    fseek(file, 54, SEEK_SET); // seek to pixel data
    pixels = std::vector<std::vector<unsigned int>>();
    pixels.resize(height);

    for (int y = 0; y < height; y++) {
        std::vector<unsigned int> row(width);
        for (int x = 0; x < width; x++)
            row[x] = fdread(3, file);
        pixels[y] = row;
    }

    reverse(pixels.begin(), pixels.end());
    fclose(file);
}

// default empty sprite
spr::sprite::sprite() {
    width = 0; // reading width
    height = 0; // reading height
    pixels = std::vector<std::vector<unsigned int>>();
    spr_width = 0;
    spr_height = 0;
}