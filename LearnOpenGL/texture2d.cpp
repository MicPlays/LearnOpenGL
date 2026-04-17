#include "texture2d.h"
#include <iostream>
#include <string>

Texture2D::Texture2D()
{
	ID = new unsigned int(0);
	type = new std::string("");
	this->width = new unsigned int(0);
	this->height = new unsigned int(0);
	this->internal_format = new unsigned int(GL_RGB);
	this->image_format = new unsigned int(GL_RGB);
	this->wrap_s = new unsigned int(GL_REPEAT);
	this->wrap_t = new unsigned int(GL_REPEAT);
	this->filter_min = new unsigned int(GL_LINEAR);
	this->filter_max = new unsigned int(GL_LINEAR);
	glGenTextures(1, this->ID);
}

Texture2D::~Texture2D()
{
	delete this->width;
	delete this->height;
	delete this->internal_format;
	delete this->image_format;
	delete this->wrap_s;
	delete this->wrap_t;
	delete this->filter_min;
	delete this->filter_max;
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
	*this->width = width;
	*this->height = height;
	
	//create texture
	glBindTexture(GL_TEXTURE_2D, *this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, *this->internal_format, *this->width, *this->height, 0, *this->image_format, GL_UNSIGNED_BYTE, data);

	//set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, *this->wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, *this->wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, *this->filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, *this->filter_max);

	glGenerateMipmap(GL_TEXTURE_2D);

}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, *this->ID);
}