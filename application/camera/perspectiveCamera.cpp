#include "perspectiveCamera.h"
#include "perspectiveCamera.h"
#include"perspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far)
{
	mFovy = fovy;
	mAspect = aspect;
	mNear = near;
	mFar = far;
}

PerspectiveCamera::~PerspectiveCamera()
{
}

glm::mat4 PerspectiveCamera::projectionMat()
{
	/*
	fovy:y轴方向的视张角
	aspect:近平面的横纵百分比
	near:近平面距离
	far:远平面距离
	*/
	return glm::perspective(glm::radians(mFovy),mAspect,mNear,mFar);//产生透视摄像机盒子
}

void PerspectiveCamera::onScale(float dScale)
{
	mPosition += glm::cross(mUp, mRight) * dScale;
}


