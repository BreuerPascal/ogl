#ifndef IMAGELOADER_H
#define IMAGELOADER_H


#include <GL/glew.h>
#include <string_view>
#include <array>
#include <vector>

namespace util {

    const size_t dataPos_headerPosition = 0x0A;
    const size_t imageSize_headerPosition = 0x22;
    const size_t width_headerPosition = 0x12;
    const size_t height_headerPosition = 0x16;
    const size_t bmpFileHeaderSize = 54;
    struct BMPFileHeader {
        std::array<std::byte, bmpFileHeaderSize> header{}; // Each BMP file begins by a 54-bytes header
        unsigned int dataPos; // Position in the file wehre the actual data begins
        unsigned int width;
        unsigned int height;
        unsigned int imageSize; // = width * height * 3
        std::size_t fileSize;
        std::vector<std::byte> data;
    };

    GLuint loadBMP(const std::string_view imagepath);


    GLuint loadDDs(const char* imagepath);
}

//class ImageLoader
//{
//public:
//    ImageLoader();
//};

#endif // IMAGELOADER_H
