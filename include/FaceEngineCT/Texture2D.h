#ifndef FACEENGINECT_TEXTURE2D_H_
#define FACEENGINECT_TEXTURE2D_H_

#include <cstdint>
#include <cstdlib>
#include <string>
#include <png.h>

namespace FaceEngineCT
{
    class Texture2D
    {
    private:
        std::uint32_t width, height;
        std::uint8_t* data;
    public:
        inline Texture2D() { width = 0; height = 0; data = nullptr; }

        inline ~Texture2D() { if (width > 0) { delete[] data; } }

        inline std::uint32_t Width() const noexcept { return width; }
        inline std::uint32_t Height() const noexcept { return height; }
        inline std::uint8_t* Data() const noexcept { return data; }
        inline bool HasData() const noexcept { return width > 0; }

        void Allocate(std::uint32_t, std::uint32_t);
        void FromPNGFile(const std::string&);
    };
}

#endif