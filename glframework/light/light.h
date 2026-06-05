#pragma once
#include"../core.h"
class Light {
public:
	Light();
	~Light();
public:
	glm::vec3 mColor{1.0f};//环境光颜色大小
	float mSpecularIntensity{ 1.0f };//高光反射强度
	float mIntensity{ 1.0f };//光照强度
};