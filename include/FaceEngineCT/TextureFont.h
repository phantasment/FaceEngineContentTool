#ifndef FACEENGINECT_TEXTUREFONT_H_
#define FACEENGINECT_TEXTUREFONT_H_

#include <cstdint>
#include <vector>
#include <string>

namespace FaceEngineCT
{
    struct FontChar
    {
    public:
        std::uint32_t CharCode;
        std::int32_t BearingX, BearingY;
        std::int32_t Advance;
        std::uint32_t TexWidth, TexHeight;
        std::uint8_t* Data;
    };

    class TextureFont
    {
    private:
        std::vector<FontChar> fontChars;
        std::uint32_t size;
        std::int32_t ascender;
        std::int32_t descender;
        std::int32_t lineSpacing;
    public:
        inline ~TextureFont()
        {
            if (fontChars.size() > 0)
            {
                for (FontChar fontChar: fontChars)
                {
                    if (fontChar.TexWidth > 0 && fontChar.TexHeight > 0)
                    {
                        delete[] fontChar.Data;
                    }
                }
            }
        }

        inline void Destroy()
        {
            if (fontChars.size() > 0)
            {
                for (FontChar fontChar: fontChars)
                {
                    if (fontChar.TexWidth > 0 && fontChar.TexHeight > 0)
                    {
                        delete[] fontChar.Data;
                    }
                }

                fontChars.clear();
            }
        }

        inline bool HasData() const noexcept
        {
            return fontChars.size() != 0;
        }

        inline std::uint32_t GetSize() const noexcept { return size; }
        inline std::int32_t GetAscender() const noexcept { return ascender; }
        inline std::int32_t GetDescender() const noexcept { return descender; }
        inline std::int32_t GetLineSpacing() const noexcept { return lineSpacing; }
        inline const std::vector<FontChar>& GetFontChars() const noexcept { return fontChars; }

        void FromFontFile(const std::string&, std::uint32_t);
    };
}

#endif