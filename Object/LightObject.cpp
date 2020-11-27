#include "stdafx.h"
#include "LightObject.h"

LightObject::LightObject(ID3D11Device* pDevice, std::string& filename)
	:Model(pDevice, filename)
{
}

LightObject::~LightObject()
{
}
