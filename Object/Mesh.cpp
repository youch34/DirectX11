#include "stdafx.h"
#include "Mesh.h"
#include "Pipeline/ConstantBuffer.h"

Mesh::Mesh(std::vector<Texture2D>& textures)
{
	this->textures = textures;
}

Mesh::~Mesh()
{
}

void Mesh::Render(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, DirectX::XMMATRIX parent, DirectX::XMMATRIX view_proj)
{
	VertexBuffer vb;
	vb.Create(pDevice, pContext, vertices);

	IndexBuffer ib;
	ib.Create(pDevice, pContext, indices);
	ConstantMatrix cm;
	cm.world = transformMatrix * parent;
	cm.view_proj = view_proj;
	ConstantBuffer<ConstantMatrix> cb;
	cb.Create(pDevice, pContext, cm, BindShader::VS, 0u);

	for (int i = 0; i < textures.size(); i++)
	{
		if(textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE)
			pContext->PSSetShaderResources(0,1,textures[i].GetAddressofSRV());
	}

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->DrawIndexed(indices.size(), 0u, 0u);
}

void Mesh::Update()
{

}
