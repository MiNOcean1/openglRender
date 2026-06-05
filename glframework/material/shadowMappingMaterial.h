#pragma once
#include"../core.h"
#include"material.h"
#include "../texture.h"
class ShadowMappingMaterial :public Material{
public:
	ShadowMappingMaterial();
	~ShadowMappingMaterial();
public:
	Texture* mDiffuse{ nullptr };//贴图，影响环境光，漫反射
	Texture* mShadowMap{ nullptr };//灰度图，决定高光反射
};