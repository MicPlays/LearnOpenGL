#include "model.h"
#include "resourcemanager.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Model::Model(const char* path, std::string name, ResourceManager* rm)
{
	this->meshes = new std::vector<Mesh>();
	rm->load_model(path, name, this);
}

Model::~Model()
{
	delete meshes;
}

void Model::Draw(Shader* shader)
{
	for (unsigned int i = 0; i < meshes->size(); i++)
		(*meshes)[i].Draw(shader);
}

void Model::processNodes(aiNode *node, const aiScene *scene, ResourceManager* rm)
{
	//proces all the node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes->push_back(processMesh(mesh, scene, rm));
	}
	//then do the same for child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNodes(node->mChildren[i], scene, rm);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, ResourceManager* rm)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture2D*> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		//process vertex positions, normals, and texture coords
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		glm::vec3 normVec;
		normVec.x = mesh->mNormals[i].x;
		normVec.y = mesh->mNormals[i].y;
		normVec.y = mesh->mNormals[i].y;
		vertex.Normal = normVec;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	//process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture2D*> diffuseMaps = rm->loadTextureMaps(material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture2D*> specularMaps = rm->loadTextureMaps(material, aiTextureType_SPECULAR, "texture_specular", directory);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures, rm);
}