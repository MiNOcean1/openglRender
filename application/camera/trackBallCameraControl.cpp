#include "trackBallCameraControl.h"


TrackBallCameraControl::TrackBallCameraControl()
{
}

TrackBallCameraControl::~TrackBallCameraControl()
{
}

void TrackBallCameraControl::onCursor(double xpos, double ypos)
{
	if (mLeftMouseDown) {
		//调整相机各类参数
		//计算经线和纬线的旋转角度
		float dX = (xpos - mCurrentX) * mSensitivity;//前面是屏幕像素点的差值，后面是差值对应的角度旋转敏感度
		float dY = (ypos - mCurrentY) * mSensitivity;

		pitch(dY);
		yaw(dX);
	}
	if (mMiddleMouseDown) {
		float dX = (xpos - mCurrentX) * mMoveSpeed;
		float dY = (ypos - mCurrentY) * mMoveSpeed;

		mCamera->mPosition += mCamera->mRight * -dX;
		mCamera->mPosition += mCamera->mUp * dY;

	}
	mCurrentX = xpos;
	mCurrentY = ypos;
}

void TrackBallCameraControl::onScroll(double xoffset, double yoffset)
{
	mCamera->onScale(yoffset*mScrollSpeed);
}

void TrackBallCameraControl::pitch(float angle)//dY对摄像机参数的影响（pitch角(俯仰角)，绕摄像机坐标系的right轴旋转）,会影响position，up（不影响right）
{
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f),glm::radians(angle),mCamera->mRight);
	mCamera->mPosition = mat * glm::vec4(mCamera->mPosition,1.0f);//坐标转换后面要加一个1.0f，四维导入三维默认只取xyz
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);//向量转换后面要加一个0.0f
}

void TrackBallCameraControl::yaw(float angle)//dX对摄像机参数的影响（yaw角，绕世界坐标系的y轴旋转），摄像头所有信息都会被影响
{
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f,1.0f,0.0f));
	mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);//坐标转换后面要加一个1.0f，四维导入三维默认只取xyz
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);//向量转换后面要加一个0.0f
	mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
}



void TrackBallCameraControl::setMoveSpeed(float moveSpeed)
{
	mMoveSpeed = moveSpeed;
}


