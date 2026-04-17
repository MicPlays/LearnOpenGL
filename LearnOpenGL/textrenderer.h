#pragma once
#include <string>
#include <map>
#include <glm/glm.hpp>

class Shader;
class Font;
class ResourceManager;

class TextRenderer
{
	public:
		TextRenderer(const char* font, ResourceManager* rm);
		TextRenderer(Font* font);
		void RenderText(Shader* s, std::string text, float x, float y, float scale, glm::vec3 color);
		unsigned int VAO, VBO;
		Font* activeFont;
};
