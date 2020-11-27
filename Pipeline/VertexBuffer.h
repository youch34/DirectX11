#pragma once
struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
};

class VertexBuffer 
{
public:
	VertexBuffer();
	~VertexBuffer();
public:
	void Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::vector<Vertex>& vertices)
	{
		ID3D11Buffer* pVertexBuffer;
		D3D11_BUFFER_DESC vbd = {};
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.ByteWidth = sizeof(Vertex) * vertices.size();
		vbd.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA vsd = {};
		vsd.pSysMem = vertices.data();
		HRESULT hr;
		hr = pDevice->CreateBuffer(&vbd, &vsd, &pVertexBuffer);
		assert(SUCCEEDED(hr));
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		pContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);
		pVertexBuffer->Release();
	}
private:

};