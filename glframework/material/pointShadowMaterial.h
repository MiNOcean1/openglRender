#pragma once
#include"../core.h"
#include"material.h"
#include"../texture.h"
class PointShadowMaterial :public Material {
public:
	PointShadowMaterial();
	~PointShadowMaterial();
public:
	Texture* mDiffuse{ nullptr };//贴图，影响环境光，漫反射
	Texture* mShadowMap{ nullptr };//阴影深度图
	bool mReverse_normals = false;//是否翻转法线，外表的大正方体法线朝内
};