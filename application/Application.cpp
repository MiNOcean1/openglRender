#include "application.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
//初始化Application的静态变量
Application* Application::mInstance = nullptr;
Application* Application::getInstance() {
	//如果mInstance已经实例化了（new了），就直接返回
	//否则需要实例化再返回
	if (mInstance == nullptr) {
		mInstance = new Application();
	}
	return mInstance;
}

bool Application::init(const int& width , const int& height){
	mWidth = width;
	mHeight = height;

	//初始化GLFW基本环境
	glfwInit();
	//设置主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//设置次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//设置OpenGL核心渲染模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗体对象
	mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGLStudy", NULL, NULL);
	if (mWindow == NULL)return false;
	//设置当前窗体对象为OpenGL绘制舞台
	glfwMakeContextCurrent(mWindow);

	//******使用GLAD加载所有当前版本需要的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {//如果加载失败
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	//监听窗体变化信息
	glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeCallback);
	//监听键盘变化信息
	glfwSetKeyCallback(mWindow, keyCallback);
	//鼠标点击事件响应
	glfwSetMouseButtonCallback(mWindow, mouseCallback);
	//鼠标移动事件响应
	glfwSetCursorPosCallback(mWindow, cursorCallback);
	//鼠标滚轮移动响应
	glfwSetScrollCallback(mWindow, scrollCallback);
	return true;
}

bool Application::update()
{
	if (glfwWindowShouldClose(mWindow))return false;

	//检查消息队列是否有需要处理的鼠标和键盘信息
	//如果有的话就将消息批量处理，清空队列
	glfwPollEvents();


	//执行双缓冲切换
	glfwSwapBuffers(mWindow);

	return true;
}

void Application::destroy()
{
	//退出程序前做相关清理
	glfwTerminate();
}

void Application::setResizeCallback(ResizeCallback callback)
{
	mResizeCallback = callback;
}

void Application::setKeyCallback(KeyCallback callback)
{
	mKeyCallback = callback;
}

void Application::setMouseCallback(MouseCallback callback)
{
	mMouseCallback = callback;
}

void Application::setCursorCallback(CursorCallback callback)
{
	mCursorCallback = callback;
}

void Application::setScrollCallback(ScrollCallback callback)
{
	mScrollCallback = callback;
}

void Application::getCursorPos(double* x, double* y)
{
	glfwGetCursorPos(mWindow, x, y);
}

void Application::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	//std::cout << "resize" << std::endl;
	if (Application::getInstance()->mResizeCallback != nullptr) {
		Application::getInstance()->mResizeCallback(width, height);
	}
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//std::cout << "rekey" << std::endl;
	if (Application::getInstance()->mKeyCallback != nullptr) {
		Application::getInstance()->mKeyCallback(key, scancode, action, mods);
	}
}

void Application::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	//std::cout << "鼠标点击：" << button << " "<<action<<std::endl;//0是左键，1是右键，2是中键，1是按下,0是抬起
	if (Application::getInstance()->mMouseCallback != nullptr) {
		
		Application::getInstance()->mMouseCallback(button, action, mods);
	}
}

void Application::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << "鼠标移动：" << xpos << " " << ypos << std::endl;
	if (Application::getInstance()->mCursorCallback != nullptr) {
		Application::getInstance()->mCursorCallback(xpos,ypos);
	}
}

void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	//std::cout << "***" << std::endl;
	if (Application::getInstance()->mScrollCallback != nullptr) {
		Application::getInstance()->mScrollCallback(xoffset, yoffset);
	}
}


Application::Application() {

}
Application::~Application() {

}