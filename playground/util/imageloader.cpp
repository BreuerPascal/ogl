#include "imageloader.h"
#include <filesystem>
#include <fstream>
#include <stdio.h>


GLuint util::loadBMP(const std::string_view imagepath)
{
    BMPFileHeader bmp_file_header{};

    std::ifstream ifs(imagepath.data(), std::ios::binary|std::ios::ate);
    if(!ifs) {
        printf("Image could not be opende\n");
        return 0;
    }

    std::ifstream::pos_type end = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::size_t size = static_cast<std::size_t>(end - ifs.tellg());
    if(size == 0) {
        printf("File has length 0\n");
        return 0;
    }
    bmp_file_header.fileSize = size;

    ifs.read((char*) bmp_file_header.header.data(), bmp_file_header.header.size());
    std::streamsize read_for_header = ifs.gcount();
    if(read_for_header != bmpFileHeaderSize) {
        printf("Not a correct BMP file\n");
        return 0;
    }

    if(bmp_file_header.header.at(0) != static_cast<std::byte>('B') || bmp_file_header.header.at(1) != static_cast<std::byte>('M')){
        printf("Not a correct BMP file\n");
        return 0;
    }

    bmp_file_header.dataPos = static_cast<unsigned int>(bmp_file_header.header.at(dataPos_headerPosition));
    bmp_file_header.imageSize = static_cast<unsigned int>(bmp_file_header.header.at(imageSize_headerPosition));
    bmp_file_header.width = static_cast<unsigned int>(bmp_file_header.header.at(width_headerPosition));
    bmp_file_header.height = static_cast<unsigned int>(bmp_file_header.header.at(height_headerPosition));

    if(bmp_file_header.imageSize == 0) {
        bmp_file_header.imageSize = bmp_file_header.width * bmp_file_header.height * 3;
    }
    if(bmp_file_header.dataPos == 0) {
        bmp_file_header.dataPos = bmpFileHeaderSize;
    }
    bmp_file_header.data.reserve(bmp_file_header.imageSize);
    ifs.read((char*)bmp_file_header.data.data(), bmp_file_header.imageSize);
    ifs.close();

    // create one OpenGL texture
    GLuint texture_id;
    glGenTextures(1, &texture_id);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return texture_id;
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

#include <string.h>
GLuint loadDDS(const char * imagepath){

    unsigned char header[124];

    FILE *fp;

    /* try to open the file */
    fp = fopen(imagepath, "rb");
    if (fp == NULL){
        printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
        return 0;
    }

    /* verify the type of file */
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        return 0;
    }

    /* get the surface desc */
    fread(&header, 124, 1, fp);

    unsigned int height      = *(unsigned int*)&(header[8 ]);
    unsigned int width	     = *(unsigned int*)&(header[12]);
    unsigned int linearSize	 = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC      = *(unsigned int*)&(header[80]);


    unsigned char * buffer;
    unsigned int bufsize;
    /* how big is it going to be including all mipmaps? */
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    /* close the file pointer */
    fclose(fp);

    unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch(fourCC)
    {
    case FOURCC_DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case FOURCC_DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case FOURCC_DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    default:
        free(buffer);
        return 0;
    }

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    /* load the mipmaps */
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
            0, size, buffer + offset);

        offset += size;
        width  /= 2;
        height /= 2;

        // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
        if(width < 1) width = 1;
        if(height < 1) height = 1;

    }

    free(buffer);

    return textureID;


}
