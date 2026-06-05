#pragma once
#include"material.h"
#include"../core.h"
#include"../texture.h"
class PhongInstancedMaterial :public Material{
public:
	PhongInstancedMaterial();
	~PhongInstancedMaterial();
public:
	Texture* mDiffuse{ nullptr };//贴图，影响环境光，漫反射
	Texture* mSpecular{ nullptr };//灰度图，决定高光反射
	float mShiness{ 1.0f };//光斑大小
};