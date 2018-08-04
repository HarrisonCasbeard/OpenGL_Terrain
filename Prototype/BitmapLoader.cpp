#include "BitmapLoader.h"
#include <stdio.h>
#include <glfw3.h>
#include <glfw3native.h>

BitmapLoader::BitmapLoader()
{
	m_bmpHeaderSize = 54;
}

BitmapLoader::~BitmapLoader()
{
}

GLuint BitmapLoader::loadCustomBMP(const char * filepath)
{
	struct Bitmap bmp;
	//Open the bitmap
	FILE* file;
	fopen_s(&file, filepath, "rb");
	if (!file)
	{
		printf("Could not open bitmap file : %s", filepath);
		return 0;
	}

	//Check if it is a bitmap
	if (fread(bmp.header, 1, m_bmpHeaderSize, file) != m_bmpHeaderSize)
	{
		//If header size isn't 54 then something went wrong!
		printf("%s is not a valid bitmap file!", filepath);
		return 0;
	}

	//Check if the bitmap header is valid
	if (bmp.header[0] != 'B' || bmp.header[1] != 'M')
	{
		//If header doesn't contain "BM" at the start, then there's an issue
		printf("%s is not a valid bitmap file!", filepath);
		return 0;
	}

	// Read ints from the byte array
	bmp.dataPos		= *(int*)&(bmp.header[0x0A]);
	bmp.width		= *(int*)&(bmp.header[0x12]);
	bmp.height		= *(int*)&(bmp.header[0x16]);
	bmp.imageSize	= *(int*)&(bmp.header[0x22]);

	//Make up some data if it doesn't exist (misformatting)
	if (!bmp.imageSize)
		bmp.imageSize = bmp.height * bmp.width * 3;

	if (!bmp.dataPos)
		bmp.dataPos = m_bmpHeaderSize;

	//Now all error checking is done - load the data
	bmp.data = new unsigned char[bmp.imageSize];
	fread(bmp.data, 1, bmp.imageSize, file);

	//Done with the file, so close the stream
	fclose(file);

	//Create one OpenGL Texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	//"Bind" the newly created texture so all future texture function calls will modify this
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.width, bmp.height, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.data);

	//When MAGnifying the image (no bigger mipmap available) use . . .
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//Create MipMaps
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
