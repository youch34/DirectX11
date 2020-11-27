#pragma once
#include "Object/Model.h"
#include "Object/Mesh.h"
#include "Object/Camera.h"
#include "Object/LightObject.h"
#include "Utility/Keyboard.h"

#include "Pipeline/ConstantBuffer.h"
#include "Pipeline/Shader.h"
#include "Pipeline/InputLayout.h"

#include "Resource/Texture2D.h"

class Graphics 
{
public:
	Graphics(HWND hWnd, Keyboard* kbd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void CreateShader();
	void EndFrame();
	void ClearBuffer(float r, float g, float b) 
	{
		const float color[] { r, g, b, 1.0f};
		pContext->ClearRenderTargetView(pTarget.Get(), color);
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}
	void Render(Camera& cam);
	void Upate(Camera& cam);
	void ImGuiRender();
private:
	Keyboard* kbd;
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSPState = nullptr;
private:
	Microsoft::WRL::ComPtr<ID3DBlob> VSblob;
	Microsoft::WRL::ComPtr<ID3DBlob> PSblob;
private:
	ConstantLight constantLight{ XMFLOAT3{1.0f,1.0f,1.0f}, 1.0f };
private:
	std::vector<std::shared_ptr<Model>> models;
	std::shared_ptr<LightObject> Light;
};