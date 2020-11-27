#pragma once
class InputLayout 
{
public:
	InputLayout();
	~InputLayout();

public:
	void Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Microsoft::WRL::ComPtr<ID3DBlob> VSblob)
	{
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[]
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		HRESULT hr = pDevice->CreateInputLayout(ied, _countof(ied), VSblob->GetBufferPointer(), VSblob->GetBufferSize(), pInputLayout.GetAddressOf());
		assert(SUCCEEDED(hr));
		pContext->IASetInputLayout(pInputLayout.Get());
		pInputLayout->Release();
	}
};