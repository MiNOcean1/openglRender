#pragma once
#include"../core.h"
#include"material.h"
#include"../texture.h"
class ScreenMaterial :public Material{
public:
	ScreenMaterial();
	~ScreenMaterial();
public:
	Texture* mScreenTexture{ nullptr };

};