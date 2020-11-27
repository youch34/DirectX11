#pragma once
struct ConstantMatrix
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX view_proj = DirectX::XMMatrixIdentity();
};

struct ConstantLight 
{
	DirectX::XMFLOAT3 ambientColor;
	float ambientColorStrength;

	DirectX::XMFLOAT3 dynamicLightColor;
	float dynamicLightColorStrength;

	DirectX::XMFLOAT3 dynamicLightPosition;
	float dynamicLightAttenuation_a;

	float dynamicLightAttenuation_b;
	float dynamicLightAttenuation_c;
	float padding[2];
};

enum class BindShader : UINT
{
	VS,
	PS
};

template<typename T>
class ConstantBuffer 
{
public:
	ConstantBuffer();
	~ConstantBuffer();
public:
	void Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, T cb, BindShader bs, UINT slot)
	{
		ID3D11Buffer* pConstantBuffer;
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0;
		cbd.ByteWidth = sizeof(cb);
		cbd.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &cb;
		HRESULT hr = pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);
		if (bs == BindShader::VS)
			pContext->VSSetConstantBuffers(slot, 1u, &pConstantBuffer);
		else if (bs == BindShader::PS)
			pContext->PSSetConstantBuffers(slot, 1u, &pConstantBuffer);
		pConstantBuffer->Release();
	}
private:
	
};

template<typename T>
inline ConstantBuffer<T>::ConstantBuffer()
{
}

template<typename T>
inline ConstantBuffer<T>::~ConstantBuffer()
{
}
