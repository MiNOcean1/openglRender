#pragma once
#include"camera.h"
class PerspectiveCamera:public Camera {//注意默认是private继承Camera，这会导致Camera中public函数被PerspectiveCamera继承后无法被外界访问
public:
	PerspectiveCamera(float fovy,float aspect,float near,float far);
	~PerspectiveCamera();
	glm::mat4 projectionMat() override;
	void onScale(float dScale)override;
public:
	float mFovy = 0.0f;
	float mAspect = 0.0f;
};