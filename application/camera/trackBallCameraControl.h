#pragma once
#include"cameraControl.h"
class TrackBallCameraControl :public CameraControl {
public:
	TrackBallCameraControl();
	~TrackBallCameraControl();
	void onCursor(double xpos,double ypos)override;
	void onScroll(double xoffset, double yoffset)override;
	void pitch(float angle);
	void yaw(float angle);
	void setMoveSpeed(float moveSpeed);
private:
	float mMoveSpeed = 0.05f;
	float mScrollSpeed = 0.3f;
};