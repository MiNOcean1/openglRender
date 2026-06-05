#include "camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMat()
{
	//lookAt:生成一个viewMartix
	//eye:当前摄像机所在位置
	//center：当前摄像机看的点
	//top：穹顶向量（摄像机大方向是站立还是倒立的）(使用mUp替代)
	//都是NDC坐标，范围都在（-1,1）
	glm::vec3 front = glm::cross(mUp, mRight);
	glm::vec3 center = mPosition + front;//表示看向前方
	return glm::lookAt(mPosition, center, mUp);
}

glm::mat4 Camera::projectionMat()
{
	return glm::mat4(1.0f);
}

void Camera::onScale(float dScale)
{
}


