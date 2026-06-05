#pragma once
#include"../core.h"
#include"material.h"
#include"../texture.h"
class IrradianceConvolutionMaterial :public Material{
public:
	IrradianceConvolutionMaterial();
	~IrradianceConvolutionMaterial();
public:
	Texture* mEnvMap{ nullptr };
};