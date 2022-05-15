#include "FaceEngineCT/Texture2D.h"

namespace FaceEngineCT
{
    void Texture2D::Allocate(std::uint32_t w, std::uint32_t h)
    {
        if (w < 1 || h < 1)
        {
            throw std::string("no");
        }

        if (width > 0)
        {
            delete[] data;
        }

        width = w;
        height = h;
        data = new std::uint8_t[width * height * 4];
    }

    void Texture2D::FromPNGFile(const std::string& path)
    {
        std::FILE* fp = std::fopen(path.c_str(), "rb");

        if (!fp)
        {
            throw std::string("Couldn't open '" + path + "'.");
        }

        png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

        if (!png)
        {
            std::fclose(fp);
            throw std::string("Couldn't create PNG read struct.");
        }

        png_infop info = png_create_info_struct(png);

        if (!info)
        {
            std::fclose(fp);
            png_destroy_read_struct(&png, NULL, NULL);
            throw std::string("Couldn't create PNG info struct.");
        }

        if (setjmp(png_jmpbuf(png)))
        {
            std::fclose(fp);
            png_destroy_read_struct(&png, &info, NULL);
            throw std::string("An error occured while reading PNG file.");
        }

        png_init_io(png, fp);
        png_read_info(png, info);
        std::uint32_t width = png_get_image_width(png, info);
        std::uint32_t height = png_get_image_height(png, info);
        std::uint8_t colorType = png_get_color_type(png, info);
        std::uint8_t bitDepth = png_get_bit_depth(png, info);

        if (bitDepth < 8)
        {
            png_set_expand(png);
        }
        else if (bitDepth == 16)
        {
            png_set_strip_16(png);
        }

        if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
        {
            png_set_expand_gray_1_2_4_to_8(png);
        }

        if (png_get_valid(png, info, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(png);
        }

        if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
        }

        if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
        {
            png_set_gray_to_rgb(png);
        }

        png_read_update_info(png, info);
        png_bytep* rowPointers = (png_bytep*)std::malloc(sizeof(png_bytep) * height);

        for (int y = 0; y < height; ++y)
        {
            rowPointers[y] = (png_byte*)std::malloc(png_get_rowbytes(png, info));
        }

        png_read_image(png, rowPointers);
        png_destroy_read_struct(&png, &info, NULL);
        std::fclose(fp);
        Allocate(width, height);
        std::int32_t pos = 0;

        for (int y = height - 1; y > -1; --y)
        {
            for (int x = 0; x < width * 4; ++x)
            {
                data[pos++] = rowPointers[y][x];
            }
        }

        for (int y = 0; y < height; ++y)
        {
            std::free(rowPointers[y]);
        }

        std::free(rowPointers);
    }
}