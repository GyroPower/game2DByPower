
#include"texture.h"
#include<stb_image/stb_image.h>
#include<iostream>

Texture2D::Texture2D()
	:width(0), height(0), internalFormat(GL_RGB), imageFormat(GL_RGB), wrapS(GL_CLAMP_TO_EDGE),
	wrapT(GL_CLAMP_TO_EDGE), filterMin(GL_LINEAR), filterMax(GL_LINEAR), nrComponents(0)
{
	glGenTextures(1, &this->id);
}

void Texture2D::load(const char* path, bool alpha,int slot) {
	if (alpha) {
		this->internalFormat = GL_RGBA;
		this->imageFormat = GL_RGBA;
	}

	this->slot = slot;

	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &this->width, &this->height, &this->nrComponents, 0);
	if (data) {
		glBindTexture(GL_TEXTURE_2D, this->id);
		glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat,this->width,
			this->height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);

	}
	else {
		std::cout << "Failed to load texture at path: " << path << "\n";
		stbi_image_free(data);
	}


}

void Texture2D::generate(unsigned char* data) {
	
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, this->width, this->height,
		0, this->imageFormat, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() {
	glBindTexture(GL_TEXTURE_2D, this->id);
}


unsigned int Texture2D::getId() const {
	return this->id;
}

unsigned int Texture2D::getWidth() const {
	return this->width;
}

unsigned int Texture2D::getHeight() const {
	return this->height;
}

unsigned int Texture2D::getNrComponents() const {
	return this->nrComponents;
}