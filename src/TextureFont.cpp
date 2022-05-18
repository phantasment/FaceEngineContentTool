#include "FaceEngineCT/TextureFont.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace FaceEngineCT
{
    void TextureFont::FromFontFile(const std::string& path, std::uint32_t fontSize)
    {
        Destroy();
        FT_Library ftLibrary;

        if (FT_Init_FreeType(&ftLibrary))
        {
            throw std::string("FreeType error.");
        }

        FT_Face ftFace;
        FT_Error error = FT_New_Face(ftLibrary, path.c_str(), 0, &ftFace);

        if (error == FT_Err_Unknown_File_Format)
        {
            throw std::string("Invalid font file.");
        }
        else if (error)
        {
            throw std::string("Couldn't open '" + path + "'.");
        }

        size = fontSize;

        if (FT_Set_Pixel_Sizes(ftFace, size, size))
        {
            throw std::string("Invalid font file.");
        }

        ascender = ftFace->ascender;
        descender = ftFace->descender;
        lineSpacing = ftFace->height;

        for (std::uint8_t asciiCode = 32U; asciiCode <= 126U; ++asciiCode)
        {
            std::uint32_t charIndex = FT_Get_Char_Index(ftFace, asciiCode);

            if (charIndex == 0 || FT_Load_Glyph(ftFace, charIndex, FT_LOAD_DEFAULT) ||
                                  FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL) ||
                                  ftFace->glyph->format != FT_GLYPH_FORMAT_BITMAP)
            {
                continue;
            }

            if (ftFace->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
            {
                FontChar fontChar;
                fontChar.CharCode = asciiCode;
                fontChar.TexWidth = ftFace->glyph->bitmap.width;
                fontChar.TexHeight = ftFace->glyph->bitmap.rows;
                fontChar.BearingX = ftFace->glyph->bitmap_left;
                fontChar.BearingY = ftFace->glyph->bitmap_top;
                fontChar.Advance = (std::int32_t)ftFace->glyph->advance.x / 64;

                if (fontChar.TexWidth != 0 && fontChar.TexHeight != 0)
                {
                    fontChar.Data = new std::uint8_t[fontChar.TexWidth * fontChar.TexHeight];

                    std::size_t pos = 0;

                    for (std::int64_t y = fontChar.TexHeight - 1; y >= 0; --y)
                    {
                        for (std::int64_t x = 0; x < ftFace->glyph->bitmap.pitch; ++x)
                        {
                            fontChar.Data[pos++] = ftFace->glyph->bitmap.buffer[y * ftFace->glyph->bitmap.pitch + x];
                        }
                    }
                }

                fontChars.push_back(std::move(fontChar));
            }
        }
    }
}