#include "cameraControl.h"
#include "cameraControl.h"
#include "cameraControl.h"

CameraControl::CameraControl()
{
}

CameraControl::~CameraControl()
{
}

void CameraControl::setCamera(Camera* camera)
{
	mCamera = camera;
}

void CameraControl::setSensitivty(float sensitivty)
{
	mSensitivity = sensitivty;
}



void CameraControl::update()
{
}

void CameraControl::onMouse(int button, int action, double xpos, double ypos)
{
	//std::cout << button <<" "<< GLFW_MOUSE_BUTTON_LEFT << std::endl;
	//判断当前的按键是否按下
	bool pressed = (action == GLFW_PRESS) ;
	//如果按下，记录当前按下的位置
	if (pressed) {
		mCurrentX = xpos;
		mCurrentY = ypos;	
	}
	//根据按下的鼠标按键不同，激活不同的记录	
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		mLeftMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mRightMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mMiddleMouseDown = pressed;
		break;
	}

}

void CameraControl::onCursor(double xpos, double ypos)
{
	//std::cout << "onCursor" << std::endl;
}


void CameraControl::onKey(int key, int action, int mods)
{
	//std::cout << key << std::endl;
	//过滤掉一直按键盘情况,一直按键盘就是不要改变键盘被按下状态
	if (action == GLFW_REPEAT)return;
	mKeyMap[key] = (action == GLFW_PRESS);
	
}

void CameraControl::onScroll(double xoffset, double yoffset)
{

}
