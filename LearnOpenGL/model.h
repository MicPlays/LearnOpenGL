#pragma once
#include "resourcemanager.h"
#include "mesh.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model
{
	public:
		Model() { meshes = new std::vector<Mesh>(); };
		Model(const char* path, std::string name, ResourceManager* rm);
		~Model();
		void Draw(Shader* shader);
		std::string directory = "";
		std::vector<Mesh> *meshes;

		void processNodes(aiNode* node, const aiScene* scene, ResourceManager* rm);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene, ResourceManager* rm);
};

