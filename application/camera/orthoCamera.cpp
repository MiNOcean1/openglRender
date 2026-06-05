#include "orthoCamera.h"


OrthoCamera::OrthoCamera(float left, float right,  float bottom,float top, float near, float far)
{
	mLeft = left;
	mRight = right;
	mBottom = bottom;
	mTop = top;
	mNear = near;
	mFar = far;
}

OrthoCamera::~OrthoCamera()
{
}

glm::mat4 OrthoCamera::projectionMat()
{
	float scale =  std::pow(2.0f, mScale);
	return glm::ortho(mLeft * scale,mRight * scale,mBottom * scale,mTop * scale,mNear,mFar);
}

void OrthoCamera::onScale(float dScale)
{
	
	mScale += dScale;
}
