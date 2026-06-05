#pragma once
#include"material.h"
#include"../core.h"
#include"../texture.h"
class GammaBlinnPhongMaterial:public Material{
public:
	GammaBlinnPhongMaterial();
	~GammaBlinnPhongMaterial();
public:
	Texture* mDiffuse{ nullptr };//贴图，影响环境光，漫反射
};