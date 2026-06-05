#pragma once
#include<map>
#include<iostream>
#include"camera.h"
#include"../../glframework/core.h"
class CameraControl {
public:
	CameraControl();
	~CameraControl();
	void setCamera(Camera* camera);
	void setSensitivty(float sensitivty);
protected://基类的保护成员可以在派生类的成员函数中被访问
	//鼠标按键状态
	bool mLeftMouseDown = false;
	bool mRightMouseDown = false;
	bool mMiddleMouseDown = false;
	//鼠标当前位置
	float mCurrentX = 0.0f;
	float mCurrentY = 0.0f;
	//键盘按键状态
	std::map<int, bool>mKeyMap;
	float mSensitivity=0.3f;//灵敏度
	Camera* mCamera = nullptr;//此控制器控制的相机
public:
	virtual void update();//处理键盘被按下的键所产生的事件,(轨迹球控制器不需要用键盘，所以不需要重写)
	virtual void onMouse(int button,int action,double xpos, double ypos);//处理鼠标点击事件，按住点击有可能移动
	virtual void onCursor(double xpos, double ypos);//处理鼠标移动事件
	virtual void onKey(int key, int action, int mods);//处理键盘事件
	virtual void onScroll(double xoffset,double yoffset);//处理滚轮事件
};