#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/version.h>
#include <assimp/scene.h>
#include <assimp/material.h>

#include "Object/Model.h"


class ModelImporter 
{
public:
	ModelImporter(ID3D11Device* pDevice, std::string& filename);
	~ModelImporter();

public:
	void ParseNode(aiNode* node,const aiScene* scene, const DirectX::XMMATRIX& parentTransformMatrix);
	Mesh ParseMesh(aiMesh* mesh,const aiScene* scene, const DirectX::XMMATRIX& TransformMatrix);
	std::vector<Texture2D> LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene);
	TextureStorageType DetermineTextureStorageType(const aiScene* scene, aiMaterial* material, UINT index, aiTextureType type);
	int GetTextureIndex(aiString* pStr);
	std::vector<Mesh>& GetMeshes() { return Meshes; }
private:
	ID3D11Device* pDevice;
private:
	int assimp_flag = 0;
	const aiScene* Scene;
	std::vector<Mesh> Meshes;
	std::vector<Texture2D> textures;
	std::string filename;
};