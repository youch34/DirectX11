#pragma once
#include "Resource/Texture2D.h"
#include "Pipeline/VertexBuffer.h"
#include "Pipeline/IndexBuffer.h"

class Mesh 
{
public:
	Mesh() {};
	Mesh(std::vector<Texture2D>& textures);
	~Mesh();
public:
	void Render(ID3D11Device* pDevice,ID3D11DeviceContext* pContext, DirectX::XMMATRIX parent, DirectX::XMMATRIX view_proj);
	void Update();
public:
	const std::vector<unsigned short>& GetIndices() { return indices; }
public:
	void SetVerteices(std::vector<Vertex>& vertices) { this->vertices = vertices; }
	void SetIndices(std::vector<unsigned short>& indices) { this->indices = indices; }
	void SetUV(std::vector<DirectX::XMFLOAT2> uvs) { this->uvs = uvs; }
public:
	void SetMatrix(const DirectX::XMMATRIX& matrix) { this->transformMatrix = matrix; };
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	std::vector<DirectX::XMFLOAT2> uvs;
	std::vector<Texture2D> textures;
	DirectX::XMMATRIX transformMatrix = DirectX::XMMatrixIdentity();
};