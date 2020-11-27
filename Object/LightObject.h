#pragma once
#include "Model.h"

class LightObject : public Model
{
public:
	LightObject() {};
	LightObject(ID3D11Device* pDevice, std::string& filename);
	~LightObject();
public:
	void SetLight(DirectX::XMFLOAT3 color, float strength) { LightColor = color; LightColorStrength = strength; }
public:
	const DirectX::XMFLOAT3& GetColor() { return LightColor; }
	const float GetStrength() { return LightColorStrength; }
public:
	float attenuation_a = 1.0f;
	float attenuation_b = 0.1f;
	float attenuation_c = 0.1f;
public:
	DirectX::XMFLOAT3 LightColor;
	float LightColorStrength;
};