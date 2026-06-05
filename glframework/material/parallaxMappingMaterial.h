#pragma once
#include"material.h"
#include"../core.h"
#include"../texture.h"
class ParallaxMappingMaterial :public Material{
public:
	ParallaxMappingMaterial();
	~ParallaxMappingMaterial();
public:
	Texture* mDiffuse{ nullptr };//贴图，影响环境光，漫反射
	Texture* mNormalMap{ nullptr };//法线贴图
	Texture* mDepthMap{ nullptr };//深度贴图
};