#pragma once
#include"../core.h"
#include"material.h"
#include"../texture.h"
class PrefilterMaterial :public Material {
public:
	PrefilterMaterial();
	~PrefilterMaterial();
public:
	Texture* mEnvMap{ nullptr };
	float mRoughness{ 0.0f };
};