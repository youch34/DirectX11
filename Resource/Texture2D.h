#pragma once
#include <assimp/material.h>

namespace Bytes
{
	static std::vector<std::byte> UnLoadedTextureDatas{ std::byte{100}, std::byte{100}, std::byte{100} };
	static std::vector<std::byte> UnHandledTextureDatas{ std::byte{255}, std::byte{0}, std::byte{0} };
}

enum class TextureStorageType
{
	Invalid,
	None,
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	Disk
};

enum ResourceViewFlags : UINT
{
	ResourceView_SRV = 1U << 0,
	ResourceView_UAV = 1U << 1,
	ResourceView_RTV = 1U << 2,
	ResourceView_DSV = 1U << 3,
	ResourceView_RODSV = 1U << 4,
};

class Texture2D 
{
public:
	Texture2D(ID3D11Device* pDevice, std::byte data, UINT width, UINT height, aiTextureType type, UINT bind_flag = 0);
	Texture2D(ID3D11Device* pDevice,std::vector<std::byte>& data, UINT width, UINT height, aiTextureType type, UINT bind_flag = 0);
	Texture2D(ID3D11Device* pDevice, std::string& filepath, aiTextureType type, UINT bind_flag = 0);
	Texture2D(ID3D11Device* pDevice, const uint8_t* data, size_t size, aiTextureType type);
	~Texture2D();
public:
	void InitializeTexture();
public:
	void CreateSRV();

public:
	ID3D11ShaderResourceView* GetSRV() { return pSRV.Get(); }
	ID3D11ShaderResourceView** GetAddressofSRV() { return pSRV.GetAddressOf(); }
	UINT GetBindFlag() { return bind_flag; }
	aiTextureType GetType() { return type; }
private:
	ID3D11Device* pDevice;
	Microsoft::WRL::ComPtr<ID3D11Resource> pResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV;

private:
	std::vector<std::byte> data;
	UINT width, height;
	UINT bind_flag = 0;
	aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;
};