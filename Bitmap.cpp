#include <iostream>
#include <fstream>

#include "Bitmap.h"

void Bitmap::FlipVertical()
{

}

void Bitmap::FlipHorizontal()
{

}

bool Bitmap::LoadFile(const std::string& filename)
{
    std::ifstream in;
    in.open(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "can't open file " << filename << "\n";
        in.close();
        return false;
    }
    BITMAPINFOHEADER info;
    uint8_t header[14];
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (!in.good() || header[0] != 0x42 || header[1] != 0x4d)
    {
        in.close();
        std::cerr << "an error occured while reading the header\n";
        return false;
    }

    in.read(reinterpret_cast<char*>(&info), sizeof(info));
    if (!in.good() || info.biBitCount != 24 && info.biBitCount != 32)
    {
        in.close();
        std::cerr << "an error occured while reading the info\n";
        return false;
    }
    this->w = info.biWidth;
    this->h = info.biHeight;
    this->data.resize(this->w * this->h);
    uint32_t offset;
    memcpy(&offset, header + 10, sizeof(uint32_t));
    in.seekg(offset, std::ios_base::beg);
    uint32_t pixelsize = (info.biBitCount + 7) / 8;
    uint32_t pitch = (pixelsize * info.biWidth + 3) & (~3);
    for (int y = 0; y < (int)info.biHeight; y++) {
        for (int x = 0; x < (int)info.biWidth; x++) {
            in.read(reinterpret_cast<char*>(&data[getIndex(x, y)]), sizeof(Color));
            if (!in.is_open())
            {
                std::cerr << "can't read pixel " << std::to_string(x) << " " << std::to_string(y) << "\n";
                in.close();
                return false;
            }
        }
        in.seekg(pitch - info.biWidth * pixelsize, std::ios_base::beg);
    }
    return true;

}

bool Bitmap::SaveFile(const std::string& filename, Format format) const
{
    std::ofstream out;
    out.open(filename, std::ios::binary);
    if (!out.is_open())
    {
        std::cerr << "can't open file " << filename << "\n";
        out.close();
        return false;
    }

    BITMAPINFOHEADER info;
    uint32_t pixelsize = format;
    uint32_t pitch = (weight() * pixelsize + 3) & (~3);
    info.biSizeImage = pitch * height();
    uint32_t bfSize = 54 + info.biSizeImage;
    uint32_t zero = 0, offset = 54;
    info.biSize = 40;
    info.biWidth = weight();
    info.biHeight = height();
    info.biPlanes = 1;
    info.biBitCount = pixelsize * 8;
    info.biCompression = 0;
    info.biXPelsPerMeter = 0xb12;
    info.biYPelsPerMeter = 0xb12;
    info.biClrUsed = 0;
    info.biClrImportant = 0;

    out.put(0x42);
    out.put(0x4d);
    out.write(reinterpret_cast<const char*>(&bfSize), 4);
    out.write(reinterpret_cast<const char*>(&zero), 4);
    out.write(reinterpret_cast<const char*>(&offset), 4);
    out.write(reinterpret_cast<const char*>(&info), sizeof(info));
    if (!out.is_open())
    {
        std::cerr << "can't write header " << filename << "\n";
        out.close();
        return false;
    }

    for (int y = 0; y < height(); y++)
    {
        uint32_t padding = pitch - weight() * pixelsize;
        for (int x = 0; x < weight(); x++)
        {
            out.write(reinterpret_cast<const char*>(&data[getIndex(x, y)]), pixelsize);
            if (!out.is_open())
            {
                std::cerr << "can't write pixel " << std::to_string(x) << " " << std::to_string(y) << "\n";
                out.close();
                return false;
            }
        }
        for (int i = 0; i < (int)padding; i++) out.put(0);
    }

    out.close();
    return true;

}