#pragma once
#include <map>
#include <glm/glm.hpp>
#include <string>
#include "texture2d.h"

struct Character
{
	Texture2D*		texture;		//Texture2D object of glyph texture
	glm::ivec2		Size;		//Size of glyph
	glm::ivec2		Bearing;	//Offset from baseline to left/top of glyph
	unsigned int	Advance = 0;	//Offest to advance to next glyph

	~Character() { delete texture; };
};

class Font
{
	public:
		Character* getGlyph(const char c);
		void addGlyph(unsigned char c, Character* ch);
		Font();
		~Font();

	private:
		std::map<char, Character*> glyphs = {};

};

