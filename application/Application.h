#pragma once
/*
（4）OpenGL实现Application封装
	1.编写一个单例类的Application
	2.成员变量+成员函数
		2.1成员函数init（初始化）
		2.2成员函数update（每一帧执行）
		2.3成员还是destroy（结尾执行）
	3.响应回调函数（Resize）
		3.1声明一个函数指针ResizeCallback
		3.2声明一个ResizeCallback类型的成员变量
		3.3声明一个SetResizeCallback的函数，设置窗体变化响应回调函数
		3.4声明一个static的静态函数frameBufferSizeCallback,响应glfw窗体变化
		3.5将静态函数设置到glfw的监听Resize监听当中
		3.6* 学会使用glfw的UserPointer
	4.响应回调函数（Rekey）(与上面一样)
*/

#include<iostream>

#define glApp  Application::getInstance()


class GLFWwindow;

using ResizeCallback = void(*)(int width, int height);
using KeyCallback = void(*)(int key, int scancode, int action, int mods);
using MouseCallback = void(*)(int button, int action, int mods);
using CursorCallback = void(*)(double xpos, double ypos);
using ScrollCallback = void(*)(double xoffset, double yoffset);

class Application {
public:
	~Application();
	//用于访问实例的静态函数
	static Application* getInstance();

	bool init(const int& width=800,const int& height=600);
	

	bool update();

	void destroy();

	GLFWwindow* getWindow() const{ return mWindow; }

	uint32_t getWidth()const { return mWidth; }
	uint32_t getHeight()const { return mHeight; }
	void setResizeCallback(ResizeCallback callback);
	void setKeyCallback(KeyCallback callback);
	void setMouseCallback(MouseCallback callback);
	void setCursorCallback(CursorCallback callback);
	void setScrollCallback(ScrollCallback callback);

	void getCursorPos(double* x,double* y);
private:
	static void frameBufferSizeCallback(GLFWwindow* window,int width,int height);//静态函数只能由成员调用，外部不能调用，静态函数只能访问静态成员
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseCallback(GLFWwindow* window, int button, int action, int mods);//窗体，左/中/右键，抬起还是按下，是否按住shift/ctrl等键
	static void cursorCallback(GLFWwindow* window,double xpos,double ypos);//窗体，鼠标移动到的x、y坐标
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
private:
	//全局唯一的静态变量实例
	static Application* mInstance;
	uint32_t mWidth{ 0 };
	uint32_t mHeight{ 0 };
	GLFWwindow* mWindow{ nullptr };
	ResizeCallback mResizeCallback{ nullptr };
	KeyCallback mKeyCallback{ nullptr };
	MouseCallback mMouseCallback{ nullptr };
	CursorCallback mCursorCallback{ nullptr };
	ScrollCallback mScrollCallback{ nullptr };
	Application();
};