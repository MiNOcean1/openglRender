#pragma once
#include"light.h"
#include"../object.h"
class SpotLight :public Light,public Object {
public:
	SpotLight();
	~SpotLight();
public:
	glm::vec3 mSpotDir=glm::vec3(-1.0f,0.0f,0.0f);
	float mBeta = 45.0f;//聚光灯的最大覆盖角度
	float mTheta = 35.0f;//聚光灯的边缘模糊界限
};