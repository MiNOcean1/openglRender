#pragma once
#include"cameraControl.h"
class GameCameraControl :public CameraControl {
public:
	GameCameraControl();
	~GameCameraControl();
	void onCursor(double xpos,double ypos)override;
	void update()override;//处理键盘被按下的键所产生的事件
	void pitch(float angle);//绕mRight旋转（俯仰角）
	void yaw(float angle);//绕世界坐标系y轴旋转
private:
	float mMoveSpeed = 0.05f;
	float mPitch = 0.0f;
};