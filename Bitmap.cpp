#include "Bitmap.h"

void Bitmap::FlipVertical()
{

}

void Bitmap::FlipHorizontal()
{

}

void Bitmap::LoadFile(const std::string& filename)
{

}

bool Bitmap::SaveFile(const std::string& filename, bool withAlpha) const
{
	FILE* fp = fopen(filename.c_str(), "wb");
	if (fp == NULL) return false;
	BITMAPINFOHEADER info;
	uint32_t pixelsize = (withAlpha) ? 4 : 3;
	uint32_t pitch = (weight() * pixelsize + 3) & (~3);
	info.biSizeImage = pitch * height();
	uint32_t bfSize = 54 + info.biSizeImage;
	uint32_t zero = 0, offset = 54;
	fputc(0x42, fp);
	fputc(0x4d, fp);
	fwrite(&bfSize, 4, 1, fp);
	fwrite(&zero, 4, 1, fp);
	fwrite(&offset, 4, 1, fp);
	info.biSize = 40;
	info.biWidth = weight();
	info.biHeight = height();
	info.biPlanes = 1;
	info.biBitCount = (withAlpha) ? 32 : 24;
	info.biCompression = 0;
	info.biXPelsPerMeter = 0xb12;
	info.biYPelsPerMeter = 0xb12;
	info.biClrUsed = 0;
	info.biClrImportant = 0;
	fwrite(&info, sizeof(info), 1, fp);
	// printf("pitch=%d %d\n", (int)pitch, info.biSizeImage);
	for (int y = 0; y < height(); y++) {
		uint32_t padding = pitch - weight() * pixelsize;
		for (int x = 0; x < weight(); x++) {
			fwrite(&_bits[getIndex(x,y)], pixelsize, 1, fp);
		}
		for (int i = 0; i < (int)padding; i++) fputc(0, fp);
	}
	fclose(fp);
	return true;
}