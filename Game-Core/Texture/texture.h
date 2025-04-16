#pragma once
#include<glad/gl.h>

class Texture2D {

public:

	unsigned int id;
	int slot;
	int width, height, nrComponents;
	GLenum internalFormat, imageFormat, wrapS, wrapT;
	GLenum filterMin, filterMax;

	Texture2D();

	void load(const char* path, bool alpha = false,int slot = 0);
	void generate(unsigned char* data);
	void bind();


	unsigned int getId() const;

	unsigned int getWidth() const;
	unsigned int getHeight() const;
	unsigned int getNrComponents() const;
};