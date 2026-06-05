#pragma once
#include"material.h"
#include"../core.h"
#include"../texture.h"
class GrassInstancedMaterial :public Material{
public:
	GrassInstancedMaterial();
	~GrassInstancedMaterial();
public:
	Texture* mDiffuse{ nullptr };//贴图，影响环境光，漫反射
	Texture* mOpacityMask{ nullptr };//透明蒙版
	float mShiness{ 1.0f };//光斑大小

	//草地相关
	float mUVScale{ 11.0f };//草地贴图特性
	float mBrightness{ 1.8f };//草地亮度

	//风力相关
	float mWindScale{ 0.1f };//风力强度
	glm::vec3 mWindDirection{ 1.0f, 0.0f, 1.0f };//风力方向
	float mPhaseScale{ 1.0f };//相位大小

	//云朵相关
	Texture* mCloudMask{ nullptr };
	glm::vec3 mCloudWhiteColor{ 0.023f,0.168f,0.0f };
	glm::vec3 mCloudBlackColor{ 0.576f,1.0f,0.396f };
	float mCloudUVScale{ 6.181f };
	float mCloudSpeed{ 0.1f };
	float mCloudLerp{ 0.308f };//云层与草地混合的百分比
};