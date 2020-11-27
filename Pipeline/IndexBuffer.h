#pragma once
class IndexBuffer 
{
public:
	IndexBuffer();
	~IndexBuffer();
public:
	void Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::vector<unsigned short>& indices)
	{
		ID3D11Buffer* pIndexBuffer;
		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.ByteWidth = sizeof(unsigned short) * indices.size();
		ibd.StructureByteStride = sizeof(unsigned short);
		D3D11_SUBRESOURCE_DATA isd;
		isd.pSysMem = indices.data();
		HRESULT hr = pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);
		assert(SUCCEEDED(hr));
		pContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0u);
		pIndexBuffer->Release();
	}
private:
};