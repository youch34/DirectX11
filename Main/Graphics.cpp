#include "stdafx.h"
#include "Graphics.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>

using namespace Microsoft::WRL;
namespace DMath = DirectX;

Graphics::Graphics(HWND hWnd, Keyboard* kbd)
{
	this->kbd = kbd;

	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferDesc.RefreshRate.Denominator = 0;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = hWnd;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;

	HRESULT hr;

	hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);
	assert(SUCCEEDED(hr));

	ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	hr = pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	assert(SUCCEEDED(hr));
	hr = pDevice->CreateRenderTargetView
	(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
	);
	assert(SUCCEEDED(hr));
	pBackBuffer->Release();

	ComPtr<ID3D11Texture2D> pDepthStecil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.ArraySize = 1u;
	descDepth.MipLevels = 1u;
	descDepth.Width = 780u;
	descDepth.Height = 640u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;

	hr = pDevice->CreateTexture2D(&descDepth, nullptr, pDepthStecil.GetAddressOf());
	assert(SUCCEEDED(hr));


	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	hr = pDevice->CreateDepthStencilView(pDepthStecil.Get(), &descDSV, pDSV.GetAddressOf());
	assert(SUCCEEDED(hr));

	CD3D11_SAMPLER_DESC spDesc(D3D11_DEFAULT);
	spDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	spDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	spDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = pDevice->CreateSamplerState(&spDesc, pSPState.GetAddressOf());
	assert(SUCCEEDED(hr));
	pContext->PSSetSamplers(0, 1, pSPState.GetAddressOf());
	pSPState->Release();

	//VierPort
	D3D11_VIEWPORT vp;
	vp.Width = 780.0f;
	vp.Height = 640.0f;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	//DepthStencilView
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	ComPtr<ID3D11DepthStencilState> pDSState;
	hr = pDevice->CreateDepthStencilState(&dsDesc, pDSState.GetAddressOf());
	assert(SUCCEEDED(hr));
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);
	pDSState->Release();

	//SetRenderTarget
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	//ImGui SetUp
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
	ImGui::StyleColorsDark();

	CreateShader();
	std::string str = "../Model/Sphere.fbx";
	Light = std::make_shared<LightObject>(pDevice.Get(), str);
	Light->SetLight({1.0f,0.0f,0.0f},1.0f);
	models.push_back(Light);

	str = "../Model/dodge_challenger.fbx";
	std::shared_ptr<Model> testmodel = std::make_shared<Model>(pDevice.Get(), str);
	testmodel->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	models.push_back(testmodel);

}

void Graphics::CreateShader()
{
	Shader shader;
	shader.CreateVertexShader(pDevice.Get(), pContext.Get());
	shader.CreatePixelShader(pDevice.Get(), pContext.Get());
	VSblob = shader.GetVSblob();
	PSblob = shader.GetPSblob();

	InputLayout il;
	il.Create(pDevice.Get(), pContext.Get(), VSblob);
}

void Graphics::EndFrame()
{
	HRESULT hr = pSwap->Present(1u, 0u);
	assert(SUCCEEDED(hr));
}

void Graphics::Render(Camera& cam)
{
	for (int i = 0; i < models.size(); i++)
	{
		constantLight.dynamicLightColor = Light->GetColor();
		constantLight.dynamicLightColorStrength = Light->GetStrength();
		constantLight.dynamicLightPosition = Light->GetPosition();
		constantLight.dynamicLightAttenuation_a = Light->attenuation_a;
		constantLight.dynamicLightAttenuation_b = Light->attenuation_b;
		constantLight.dynamicLightAttenuation_c = Light->attenuation_c;
		ConstantBuffer<ConstantLight> clb;
		clb.Create(pDevice.Get(), pContext.Get(), constantLight, BindShader::PS, 0u);

		models[i]->Render(pDevice.Get(), pContext.Get(),cam.ViewProj());
		models[i]->Update();
	}
}

void Graphics::Upate(Camera& cam)
{
	if (kbd->KeyIsPressed('C'))
	{
		Light->SetPositionXM((cam.GetPositionXM() + cam.GetLookXM()*10));
	}
}


void Graphics::ImGuiRender()
{
	//Start Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//CreateWindow
	ImGui::Begin("Light Control");
	ImGui::DragFloat3("Ambient Light Color", reinterpret_cast<float*>(&this->constantLight.ambientColor), 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Ambient Light Strength", &constantLight.ambientColorStrength, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Dynamic Light Color", reinterpret_cast<float*>(&Light->LightColor), 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Dynamic Light Strength", reinterpret_cast<float*>(&Light->LightColorStrength), 0.01f, 1.0f, 20.0f);
	ImGui::DragFloat("Attenuation_A", reinterpret_cast<float*>(&Light->attenuation_a), 0.01f, 1.0f, 10.0f);
	ImGui::DragFloat("Attenuation_B", reinterpret_cast<float*>(&Light->attenuation_b), 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Attenuation_C", reinterpret_cast<float*>(&Light->attenuation_c), 0.01f, 0.0f, 10.0f);

	ImGui::End();
	//Assemble Draw Data
	ImGui::Render();
	//Render DrawData
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
