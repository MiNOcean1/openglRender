#pragma once
#include"material.h"
#include"../core.h"
#include"../texture.h"
class BlinnPhongMaterial:public Material{
public:
	BlinnPhongMaterial();
	~BlinnPhongMaterial();
public:
	Texture* mDiffuse{ nullptr };//贴图，影响环境光，漫反射
	Texture* mSpecular{ nullptr };//灰度图，决定高光反射
	float mShiness{ 32.0f };//光斑大小
};