#pragma once
#include"light.h"
#include"../object.h"
class PointLight :public Light,public Object {
public:
	PointLight();
	~PointLight();
public:
	float mK2 = 1.0f;//光源衰减二次项参数
	float mK1 = 1.0f;//光源衰减一次项参数
	float mK0 = 1.0f;//光源衰减常数项参数
};