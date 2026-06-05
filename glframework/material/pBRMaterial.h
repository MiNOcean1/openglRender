#pragma once
#include"../core.h"
#include"material.h"
#include"../texture.h"
class PBRMaterial :public Material{
public:
	PBRMaterial();
	~PBRMaterial();
public:
	
	Texture* mAlbedoMap{ nullptr };//反照率图0
	Texture* mNormalMap{ nullptr };//法线图1
	Texture* mMetallicMap{ nullptr };//金属度图2
	Texture* mRoughnessMap{ nullptr };//粗糙度图3
	Texture* mAoMap{ nullptr };//环境光遮蔽图4
	
	//float mRoughness = 0;
	//float mMetallic = 0;
	Texture* mIrradianceConvolutionMap{ nullptr };//环境光辐照图5
	Texture* mPrefilterMap{ nullptr };//预滤波HDR环境贴图6
	Texture* mBrdfLUTMap{ nullptr };//预计算 BRDF图7

};