#include "stdafx.h"
#include "Texture2D.h"
#include "DirectXTK/WICTextureLoader.h"
#include "DirectXTK/DDSTextureLoader.h"

Texture2D::Texture2D(ID3D11Device* pDevice, std::byte data, UINT width, UINT height, aiTextureType type, UINT bind_flag)
{
	this->pDevice = pDevice;
	this->data.emplace_back(data);
	this->width = width;
	this->height = height;
	this->type = type;
	InitializeTexture();
}

Texture2D::Texture2D(ID3D11Device* pDevice, std::vector<std::byte>& data, UINT width, UINT height, aiTextureType type, UINT bind_flag)
{
	this->pDevice = pDevice;
	this->data = data;
	this->width = width;
	this->height = height;
	this->type = type;
	this->bind_flag = bind_flag;
	InitializeTexture();
}

Texture2D::Texture2D(ID3D11Device* pDevice, std::string& filepath, aiTextureType type, UINT bind_flag)
{
	this->pDevice = pDevice;
	this->type = type;
	this->bind_flag = bind_flag;
	if (StringHelper::GetFileExtension(filepath) == ".dds")
	{
		HRESULT hr = DirectX::CreateDDSTextureFromFile(pDevice, StringHelper::StringToWide(filepath).c_str(), pResource.GetAddressOf(), this->pSRV.GetAddressOf());
		if (FAILED(hr))
		{
			this->data = Bytes::UnLoadedTextureDatas;
			this->width = 1;
			this->height = 1;
			this->InitializeTexture();
		}
		return;
	}
	else
	{
		HRESULT hr = DirectX::CreateWICTextureFromFile(pDevice, StringHelper::StringToWide(filepath).c_str(), pResource.GetAddressOf(), this->pSRV.GetAddressOf());
		if (FAILED(hr))
		{
			this->data = Bytes::UnLoadedTextureDatas;
			this->width = 1;
			this->height = 1;
			this->InitializeTexture();
		}
		return;
	}
}

Texture2D::Texture2D(ID3D11Device* pDevice, const uint8_t* data, size_t size, aiTextureType type)
{
	this->type = type;
	HRESULT hr = DirectX::CreateWICTextureFromMemory(pDevice, data, size, this->pResource.GetAddressOf(), this->pSRV.GetAddressOf());
}

Texture2D::~Texture2D()
{
}

void Texture2D::InitializeTexture()
{
	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	ID3D11Texture2D* p2DTexture = nullptr;
	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = data.data();
	initialData.SysMemPitch = width * sizeof(std::byte);
	HRESULT hr = pDevice->CreateTexture2D(&textureDesc, &initialData, &p2DTexture);
	pResource = static_cast<ID3D11Texture2D*>(p2DTexture);
	p2DTexture->Release();
	if (bind_flag & ResourceView_SRV)
		CreateSRV();
}

void Texture2D::CreateSRV()
{
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R8G8B8A8_UNORM);
	HRESULT hr = pDevice->CreateShaderResourceView(pResource.Get(), &srvDesc, pSRV.GetAddressOf());
}
