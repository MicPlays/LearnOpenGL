#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include "font.h"
#include "shader.h"
#include "texture2d.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model;

class ResourceManager
{
	public:
		Texture2D* load_texture(const char* file, bool alpha, bool text, std::string name);
		Shader* load_shader(const char* vShaderFile, const char* fShaderFile, std::string name);
		Font* load_font(const char* fontPath);
		void load_model(const std::string modelPath, std::string name, Model* model);
		std::vector<Texture2D*> loadTextureMaps(aiMaterial* mat, aiTextureType type, std::string typeName, std::string directory);

		template <typename T>
		T get_resource(std::string const& key);

		ResourceManager() {};

	private:
		std::vector<Texture2D*> textures = {};
		std::vector<Shader*> shaders = {};
		std::vector<Font*> fonts = {};
		std::vector<Model*> models = {};
		std::unordered_map<std::string, unsigned int> names_to_textures;
		std::unordered_map<std::string, unsigned int> names_to_shaders;
		std::unordered_map<std::string, unsigned int> names_to_fonts;
		std::unordered_map<std::string, unsigned int> names_to_models;

		Shader* load_shader_from_file(const char* vertexPath, const char* fragmentPath);
		Texture2D* load_texture_from_file(const char* file, bool alpha, bool text);
		Font* load_font_from_file(const char* fontPath);
		Model* load_model_from_file(const std::string modelPath);
		std::string generate_key(std::stringstream const& stream) const;
};

