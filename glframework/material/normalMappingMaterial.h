#pragma once
#include"material.h"
#include"../core.h"
#include"../texture.h"
class NormalMappingMaterial :public Material{
public:
	NormalMappingMaterial();
	~NormalMappingMaterial();
public:
	Texture* mDiffuse{ nullptr };//贴图，影响环境光，漫反射
	Texture* mNormalMap{ nullptr };//法线贴图
	float mShiness{ 1.0f };//光斑大小
};