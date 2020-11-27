#include "stdafx.h"
#include "Shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
	
}

void Shader::CreateVertexShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	HRESULT hr = D3DReadFileToBlob(L"VertexShader.cso", &VSblob);
	assert(SUCCEEDED(hr));
	hr = pDevice->CreateVertexShader(VSblob->GetBufferPointer(), VSblob->GetBufferSize(), nullptr, pVertexShader.GetAddressOf());
	assert(SUCCEEDED(hr));
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

void Shader::CreatePixelShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	HRESULT hr = D3DReadFileToBlob(L"PixelShader.cso", &PSblob);
	assert(SUCCEEDED(hr));
	hr = pDevice->CreatePixelShader(PSblob->GetBufferPointer(), PSblob->GetBufferSize(), nullptr, pPixelShader.GetAddressOf());
	assert(SUCCEEDED(hr));
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
