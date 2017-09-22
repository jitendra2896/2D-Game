#include"Texture.h"
#include"lodepng.h"
#include<iostream>
#include<GL\glew.h>
#include<GL\freeglut.h>

Texture::Texture(std::string fileName, int width, int height) {
	this->fileName = fileName;
	this->width = width;
	this->height = height;

	decodeFile();
	createTexture();
}

void Texture::decodeFile() {
	unsigned error = lodepng::decode(decodedImage, width, height, fileName);
	if (error) {
		std::cout << "Couldn't decode file : " << fileName << std::endl;
	}
}

void Texture::createTexture() {
	glGenBuffers(1, &texVboId);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, texVboId);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, sizeof(unsigned char)*decodedImage.size(), &decodedImage[0], GL_STATIC_DRAW);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA8, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

int Texture::getHeight() {
	return height;
}

int Texture::getWidth() {
	return width;
}

void Texture::bindTexture() {
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::cleanUp() {
	glDeleteBuffers(1, &texVboId);
	glDeleteTextures(1, &textureId);
	decodedImage.clear();
}