#pragma once
class Shader 
{
public:
	Shader();
	~Shader();
public:
	void CreateVertexShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void CreatePixelShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
public:
	Microsoft::WRL::ComPtr<ID3DBlob> GetVSblob() { return VSblob; }
	Microsoft::WRL::ComPtr<ID3DBlob> GetPSblob() { return PSblob; }
private:
	Microsoft::WRL::ComPtr<ID3DBlob> VSblob;
	Microsoft::WRL::ComPtr<ID3DBlob> PSblob;
};