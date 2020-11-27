#pragma once
#include "Object/Mesh.h"
#include "Utility/ModelImporter.h"
class Model 
{
public:
	Model() {};
	Model(ID3D11Device* pDevice,std::string& filename);
	~Model();
public:
	void Render(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, DirectX::XMMATRIX view_proj);
	void Update();
public:
	const DirectX::XMMATRIX GetWorld() { return world_matrix; }
	unsigned int GetIndicesCount() { return indicesCount; }
public:
	void SetPosition(DirectX::XMFLOAT3 pos) { position = pos; }
	void SetPositionXM(DirectX::XMVECTOR pos) { DirectX::XMStoreFloat3(&this->position, pos); }
	void SetScale(DirectX::XMFLOAT3 scale);
public:
	const DirectX::XMFLOAT3& GetPosition() { return position; }
	const DirectX::XMVECTOR& GetFoward() { return Forward; }
	const DirectX::XMVECTOR& GetRight() { return Right; }
private:
	DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
	DirectX::XMMATRIX world_matrix = DirectX::XMMatrixIdentity();
private:
	const DirectX::XMVECTOR DEFAULT_FORWARD = { 0.0f, 0.0f, 1.0f };
	const DirectX::XMVECTOR DEFAULT_RIGHT = { 1.0f, 0.0f, 0.0f };

	DirectX::XMVECTOR Forward;
	DirectX::XMVECTOR Right;
public:
	void SetMeshes(std::vector<Mesh>& meshes) { Meshes = meshes; }
	std::vector<Mesh>& GetMeshes() { return Meshes; }
private:
	std::vector<Mesh> Meshes;
	unsigned int indicesCount;
	std::string filename;
	std::string directory;
};