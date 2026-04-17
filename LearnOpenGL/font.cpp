#include "font.h"
#include <map>

Font::Font() {}

Font::~Font()
{
	glyphs.clear();
}

Character* Font::getGlyph(const char c)
{
	return this->glyphs[c];
}

void Font::addGlyph(unsigned char c, Character* ch)
{
	this->glyphs.insert(std::pair<char, Character*>(c, ch));
}

