#pragma once
#include"../core.h"
#include"../texture.h"
#include"iostream"
class FrameBuffer {
public:
	static FrameBuffer* creatCaptureFBO(unsigned int widthAndHeight);//创建存储立方体贴图的fbo
	FrameBuffer(unsigned int width, unsigned height, unsigned int flag);//0是拥有颜色、深度模板附件，1是拥有颜色、深度附件，2是立方体阴影相关
	FrameBuffer();
	~FrameBuffer();
public:
	unsigned int mWidth{ 0 };
	unsigned int mHeight{ 0 };

	unsigned int mFBO{ 0 };
	unsigned int mRBO{ 0 };
	Texture* mColorAttachment{ nullptr };
	Texture* mDepthStencilAttachment{ nullptr };
	Texture* mDepthAttachment{ nullptr };

};