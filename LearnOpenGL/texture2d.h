#pragma once
#include <glad/glad.h>
#include <string>
class Texture2D
{
	public:
		//id of texture object, used for all texture operations to reference to this particular texture
		unsigned int* ID;
		std::string* type;

		unsigned int *width, *height;

		unsigned int* internal_format;
		unsigned int* image_format;

		//OpenGL texture config settings
		unsigned int* wrap_s; //wrapping mode on S axis
		unsigned int* wrap_t; //wrapping mode on T axis
		unsigned int* filter_min; //filtering mode if texture pixels < screen pixels
		unsigned int* filter_max; //filtering mode if texture pixels > screen pixels

		Texture2D();
		~Texture2D();

		void Generate(unsigned int width, unsigned int height, unsigned char* data);
		void Bind() const;
};

