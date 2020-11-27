#include "stdafx.h"
#include "Model.h"

Model::Model(ID3D11Device* pDevice, std::string& filename)
{
	this->filename = filename;
	this->directory = StringHelper::GetDirectoryFromPath(filename);
	ModelImporter importer(pDevice, filename);

	Meshes = importer.GetMeshes();
	for (int i = 0; i < Meshes.size(); i++)
	{
		indicesCount += Meshes[i].GetIndices().size();
	}

	Update();
}

Model::~Model()
{
}

void Model::Render(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, DirectX::XMMATRIX view_proj)
{
	for (int i = 0; i < Meshes.size(); i++)
	{
		Meshes[i].Render(pDevice, pContext, this->world_matrix,view_proj);
	}
}

void Model::Update()
{	
	DirectX::XMMATRIX S, R, T;
	S = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&scale));
	R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&rotation));;
	T = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&position));
	Forward = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, R);
	Right = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, R);
	world_matrix = S * R * T;
}

void Model::SetScale(DirectX::XMFLOAT3 scale)
{
	this->scale = scale;
}
