#include "textrenderer.h"
#include "shader.h"
#include "texture2d.h"
#include "resourcemanager.h"
#include "font.h"

#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

TextRenderer::TextRenderer(const char* font, ResourceManager* rm)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	activeFont = rm->load_font(font);
}

TextRenderer::TextRenderer(Font* font)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	activeFont = font;
}

void TextRenderer::RenderText(Shader* s, std::string text, float x, float y, float scale, glm::vec3 color)
{
	//using orthographic projection to keep text on screen as a "HUD element"
	glm::mat4 tProj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

	s->use();
	unsigned int textProjLoc = glGetUniformLocation(*s->ID, "proj");
	glUniformMatrix4fv(textProjLoc, 1, GL_FALSE, glm::value_ptr(tProj));
	glUniform3f(glGetUniformLocation(*s->ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character* ch = activeFont->getGlyph(*c);

		float xpos = x + ch->Bearing.x * scale;
		float ypos = y - (ch->Size.y - ch->Bearing.y) * scale;

		float w = ch->Size.x * scale;
		float h = ch->Size.y * scale;

		float vertices[6][4] = {
		{xpos,		ypos + h,		0.0f, 0.0f},
		{xpos,		ypos,			0.0f, 1.0f},
		{xpos + w,	ypos,			1.0f, 1.0f},

		{xpos,		ypos + h,		0.0f, 0.0f},
		{xpos + w,	ypos,			1.0f, 1.0f},
		{xpos + w,	ypos + h,		1.0f, 0.0f}
		};

		//render glyph texture over quad
		ch->texture->Bind();
		//update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch->Advance >> 6) * scale; //bitshift by 6 to get value in pixels (2^6 = 64)
	}
}