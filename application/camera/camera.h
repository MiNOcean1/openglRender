#pragma once
#include"../../glframework/core.h"

class Camera {
public:
	Camera();
	~Camera();
	glm::mat4 getViewMat();
	virtual glm::mat4 projectionMat();
	virtual void onScale(float dScale);
public:
	glm::vec3 mPosition{0.0f,0.0f,5.0f };//摄像机的位置
	glm::vec3 mRight{1.0f,0.0f,0.0f};//摄像机的x轴
	glm::vec3 mUp{ 0.0f,1.0f,0.0f };//摄像机的y轴

	float mNear = 0.0f;
	float mFar = 0.0f;
};