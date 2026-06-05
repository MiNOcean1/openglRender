#include"gameCameraControl.h"

GameCameraControl::GameCameraControl() {
	
}

GameCameraControl::~GameCameraControl()
{
}

void GameCameraControl::onCursor(double xpos, double ypos)
{
	if (mRightMouseDown) {
		float dX = (xpos - mCurrentX) * mSensitivity;//前面是屏幕像素点的差值，后面是差值对应的角度旋转敏感度
		float dY = (ypos - mCurrentY) * mSensitivity;
		pitch(dY);
		yaw(dX);
	}
	mCurrentX = xpos;
	mCurrentY = ypos;
}

void GameCameraControl::update()
{
	
	glm::vec3 direction(0.0f);
	auto front = glm::cross(mCamera->mUp, mCamera->mRight);
	if (mKeyMap[GLFW_KEY_W])direction += front;
	if (mKeyMap[GLFW_KEY_S])direction -= front;
	if (mKeyMap[GLFW_KEY_A])direction -= mCamera->mRight;
	if (mKeyMap[GLFW_KEY_D])direction += mCamera->mRight;
	if (mKeyMap[GLFW_KEY_SPACE])direction += mCamera->mUp;
	if (mKeyMap[GLFW_KEY_LEFT_CONTROL])direction -= mCamera->mUp;
	if(direction!=glm::vec3(0.0f)){
		direction = glm::normalize(direction);
		mCamera->mPosition += direction*mMoveSpeed;
	}
}



void GameCameraControl::pitch(float angle)
{
	mPitch += angle;
	if (mPitch > 89.0f || mPitch < -89.0f) {
		mPitch -= angle;
		return;
	}
	//在GameCameraControl的情况下，pitch不会影响mPosition
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f),glm::radians(angle),mCamera->mRight);
	mCamera->mUp = mat * glm::vec4(mCamera->mUp,0.0f);

}

void GameCameraControl::yaw(float angle)
{
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle),glm::vec3(0.0f,1.0f,0.0f));
	mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
}
