#ifndef __BITMAP_LOADER__
#define __BITMAP_LOADER__

#include <glad.h>

struct Bitmap
{
	unsigned char header[54];		// Each BMP file begins by a 54-bytes header
	unsigned int  dataPos;			// Position in the file where the actual data begins
	unsigned int  width, height;
	unsigned int  imageSize;		// = width*height*3
	unsigned char * data;			// Actual RGB data
};

class BitmapLoader
{
private:
	unsigned short m_bmpHeaderSize;

public:
	 BitmapLoader();
	~BitmapLoader();

	GLuint loadCustomBMP(const char* filepath);
};

#endif
