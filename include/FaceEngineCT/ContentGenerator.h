#ifndef FACEENGINECT_CONTENTGENERATOR_H_
#define FACEENGINECT_CONTENTGENERATOR_H_

#include <cstdint>
#include <array>
#include <cstdlib>
#include <string>

#include "FaceEngineCT/Texture2D.h"

namespace FaceEngineCT
{
    enum ContentFileType : std::uint8_t
    {
        TypeTexture2D = 1,
        TypeTextureFont = 2
    };

    class ContentGenerator
    {
    private:
        static std::array<std::uint8_t, 16> contentFileHeader;
        static std::uint8_t contentFileVersion;
    public:
        static const std::array<std::uint8_t, 4> Int32ToBytes(std::uint32_t) noexcept;

        static void GenerateTexture2D(Texture2D*, const std::string&);
    };
}

#endif