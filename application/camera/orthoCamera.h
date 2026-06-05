#pragma once
#include"camera.h"
#include<iostream>
class OrthoCamera :public Camera {
public:
	OrthoCamera(float mLeft, float mRight, float mBottom, float mTop, float mZNear, float mZFar);
	~OrthoCamera();
	glm::mat4 projectionMat() override;
	void onScale(float dScale)override;//正交投影的滚轮缩放本质上是变化了投影盒体的缩放大小(near far不变)
private:
	float mLeft=0.0f;
	float mRight = 0.0f;
	float mBottom = 0.0f;
	float mTop = 0.0f;
private:
	float mScale = 0.0f;
};