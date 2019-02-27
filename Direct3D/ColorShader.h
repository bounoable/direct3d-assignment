#pragma once
#include "AShader.h"

class ColorShader: public AShader
{
public:
	ColorShader();
	~ColorShader();

protected:
	void createInputLayout(ID3D11Device* device, ID3DBlob* blob);
};
