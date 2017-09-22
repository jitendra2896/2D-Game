#pragma once
#include<string>
#include<vector>
class Texture {
	std::string fileName;
	unsigned int width, height;
	std::vector<unsigned char> decodedImage;
	unsigned int texVboId;
	unsigned int textureId;
	void decodeFile();
	void createTexture();
public:
	Texture(std::string fileName, int width, int height);
	Texture() {

	}

	int getWidth();
	int getHeight();

	void bindTexture();
	void cleanUp();
};