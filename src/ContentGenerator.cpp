#include "FaceEngineCT/ContentGenerator.h"
#include <zlib.h>

namespace FaceEngineCT
{
    std::array<std::uint8_t, 16> ContentGenerator::contentFileHeader = { 'F', 'E', 'C', 'F', 2, 3, 1, 0, 7, 2, 2, 2, 9, 6, 'E', 'W' };
    std::uint8_t ContentGenerator::contentFileVersion = 1;

    const std::array<std::uint8_t, 4> ContentGenerator::Int32ToBytes(std::uint32_t i) noexcept
    {
        return { (std::uint8_t)(i >> 24), (std::uint8_t)(i >> 16), (std::uint8_t)(i >> 8), (std::uint8_t)i };
    }

    void ContentGenerator::GenerateTexture2D(Texture2D* tex2d, const std::string& dest)
    {
        std::FILE* fp = std::fopen(dest.c_str(), "wb");

        if (!fp)
        {
            throw std::string("Couldn't open file for writing.");
        }

        // write header (16 bytes)
        std::fwrite(contentFileHeader.data(), contentFileHeader.size(), 1, fp);
        // write content file version (unsigned 8 bit int, 1 byte)
        std::fwrite(&contentFileVersion, 1, 1, fp);
        // write content type (unsigned 8 bit int, 1 byte)
        std::uint8_t contentType = TypeTexture2D;
        std::fwrite(&contentType, 1, 1, fp);
        // write width (unsigned 32 bit int, 4 bytes big endian)
        std::fwrite(Int32ToBytes(tex2d->Width()).data(), 4, 1, fp);
        // write height (unsigned 32 bit int, 4 bytes big endian)
        std::fwrite(Int32ToBytes(tex2d->Height()).data(), 4, 1, fp);

        // if the image data is <= 100 bytes write it raw with no compression
        // if the image data is > 100 compress it with zlib level 9 compression,
        // write the length, then write the compressed data
        std::size_t imageDataSize = tex2d->Width() * tex2d->Height() * 4;
        std::uint8_t compressLevel = imageDataSize > 100 ? Z_BEST_COMPRESSION : 0;
        std::fwrite(&compressLevel, 1, 1, fp);

        if (compressLevel > 0)
        {
            std::uint8_t* compressedImageDataBuffer = new std::uint8_t[imageDataSize];
            z_stream zStream;
            zStream.zalloc = nullptr;
            zStream.zfree = nullptr;
            zStream.opaque = nullptr;
            zStream.avail_in = imageDataSize;
            zStream.next_in = (Bytef*)tex2d->Data();
            zStream.avail_out = imageDataSize;
            zStream.next_out = (Bytef*)compressedImageDataBuffer;
            deflateInit(&zStream, compressLevel);
            deflate(&zStream, Z_FINISH);
            deflateEnd(&zStream);
            std::size_t compressedImageDataSize = zStream.total_out;
            std::fwrite(Int32ToBytes(compressedImageDataSize).data(), 4, 1, fp);
            std::fwrite(compressedImageDataBuffer, compressedImageDataSize, 1, fp);
            delete[] compressedImageDataBuffer;
        }
        else
        {
            std::fwrite(tex2d->Data(), imageDataSize, 1, fp);
        }

        std::fclose(fp);
    }
}