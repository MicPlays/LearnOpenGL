#include "resourcemanager.h"
#include "shader.h"
#include "texture2d.h"
#include "textrenderer.h"
#include "font.h"
#include "model.h"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include <stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

template <typename T>
T ResourceManager::get_resource(std::string const& key)
{
	int id = -1;

	if constexpr (std::is_same_v<T, Texture2D*>)
	{
		auto const it = names_to_textures.find(key);
		if (it != names_to_textures.end())
		{
			id = it->second;
			return textures[id];
		}
	}

	else if constexpr (std::is_same_v<T, Shader*>)
	{
		auto const it = names_to_shaders.find(key);
		if (it != names_to_shaders.end())
		{
			id = it->second;
			return shaders[id];
		}
	}

	else if constexpr (std::is_same_v<T, Font*>)
	{
		auto const it = names_to_fonts.find(key);
		if (it != names_to_fonts.end())
		{
			id = it->second;
			return fonts[id];
		}
	}
	else if constexpr (std::is_same_v<T, Model*>)
	{
		auto const it = names_to_models.find(key);
		if (it != names_to_models.end())
		{
			id = it->second;
			return models[id];
		}
	}

	return nullptr;

}

Texture2D* ResourceManager::load_texture(const char* file, bool alpha, bool text, std::string name)
{
	//generate a key for given shader
	std::stringstream stream;
	stream << file << name;
	std::string const& key = generate_key(stream);

	//attempt to get loaded shader using key
	Texture2D* texture = get_resource<Texture2D*>(key);

	if (texture != nullptr)
		return texture;

	//if shader isn't loaded, load it and store it in loaded shaders vector
	Texture2D* newTexture = load_texture_from_file(file, alpha, text);
	textures.emplace_back(newTexture);
	names_to_textures.insert(std::make_pair(key, textures.size() - 1));
	return newTexture;
}

Texture2D* ResourceManager::load_texture_from_file(const char* file, bool alpha, bool text)
{
	Texture2D* texture = new Texture2D();

	//load image for texture
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	if (data)
	{
		if (alpha)
		{
			*texture->internal_format = GL_RGBA;
			*texture->image_format = GL_RGBA;
		}
		if (text)
		{
			*texture->wrap_s = GL_CLAMP_TO_EDGE;
			*texture->wrap_t = GL_CLAMP_TO_EDGE;
		}

		texture->Generate(width, height, data);
	}
	else std::cout << "Failed to load texture" << std::endl;
	//free image memory
	stbi_image_free(data);
	return texture;
}


Shader* ResourceManager::load_shader(const char* vShaderFile, const char* fShaderFile, std::string name)
{
	//generate a key for given shader
	std::stringstream stream;
	stream << vShaderFile << fShaderFile << name;
	std::string key = generate_key(stream);

	//attempt to get loaded shader using key
	Shader* shader = get_resource<Shader*>(key);

	if (shader != nullptr)
		return shader;

	//if shader isn't loaded, load it and store it in loaded shaders vector
	Shader* newShader = load_shader_from_file(vShaderFile, fShaderFile);
	shaders.emplace_back(newShader);
	names_to_shaders.insert(std::make_pair(key, shaders.size() - 1));
	return shaders.back();
}

Shader* ResourceManager::load_shader_from_file(const char* vertexPath, const char* fragmentPath)
{
	//retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	//ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//close file handlers
		vShaderFile.close();
		fShaderFile.close();
		//convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	Shader* shader = new Shader();
	shader->compile(vShaderCode, fShaderCode);

	return shader;
}

Font* ResourceManager::load_font(const char* fontPath)
{
	//generate a key for given font
	std::stringstream stream;
	stream << fontPath;
	std::string key = generate_key(stream);

	//attempt to get loaded font using key
	Font* font = get_resource<Font*>(key);

	if (font != nullptr)
		return font;

	//if shader isn't loaded, load it and store it in loaded shaders vector
	Font* newFont = load_font_from_file(fontPath);

	fonts.emplace_back(newFont);
	names_to_fonts.insert(std::make_pair(key, fonts.size() - 1));
	return newFont;
}

Font* ResourceManager::load_font_from_file(const char* fontPath)
{
	//initialize freetype library for text rendering
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	//load font
	FT_Face face;
	if (FT_New_Face(ft, fontPath, 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	//define pixel font size to extract from loaded font face
	FT_Set_Pixel_Sizes(face, 0, 48);

	//storing character colors in one byte, OpenGL expects 4 byte alignment so this line addresses that
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	Font* font = new Font();

	//load first 128 ASCII characters into map
	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load glyph" << std::endl;
			continue;
		}

		Texture2D* texture = new Texture2D();
		*texture->wrap_s = GL_CLAMP_TO_EDGE;
		*texture->wrap_t = GL_CLAMP_TO_EDGE;
		*texture->internal_format = GL_RED;
		*texture->image_format = GL_RED;

		texture->Generate(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);

		//store character for later use
		Character* character =  new Character {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};
		font->addGlyph(c, character);
	}

	//free freetype's resources
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return font;

}

void ResourceManager::load_model(const std::string modelPath, std::string name, Model* model)
{
	//generate a key for given shader
	std::stringstream stream;
	stream << modelPath << name;
	std::string const& key = generate_key(stream);

	//attempt to get loaded shader using key
	Model* existModel = get_resource<Model*>(key);

	if (existModel == nullptr)
	{
		//if shader isn't loaded, load it and store it in loaded shaders vector
		Model* newModel = load_model_from_file(modelPath);
		model->directory = newModel->directory;
		model->meshes = newModel->meshes;
		models.emplace_back(model);
		names_to_models.insert(std::make_pair(key, models.size() - 1));
	}
	else
	{
		model->directory = existModel->directory;
		model->meshes = existModel->meshes;
	}
}

Model* ResourceManager::load_model_from_file(const std::string modelPath)
{
	Assimp::Importer importer;
	//triangulate converts all primitives to triangles, flipUVs flips texture coords on y-axis 
	const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	Model* model = new Model();
	model->directory = modelPath.substr(0, modelPath.find_last_of('/'));

	model->processNodes(scene->mRootNode, scene, this);

	return model;
}

std::vector<Texture2D*> ResourceManager::loadTextureMaps(aiMaterial *mat, aiTextureType type, std::string typeName, std::string directory)
{
	std::vector<Texture2D*> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = directory + "/" + str.C_Str();
		Texture2D* texture = load_texture(path.c_str(), 0, 0, typeName);
		*texture->type = typeName;
		textures.push_back(texture);
	}
	return textures;
}

std::string ResourceManager::generate_key(std::stringstream const& stream) const
{
	return stream.str();
}
