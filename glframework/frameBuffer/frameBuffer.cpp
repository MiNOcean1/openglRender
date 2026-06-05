#include "frameBuffer.h"



FrameBuffer* FrameBuffer::creatCaptureFBO(unsigned int widthAndHeight)
{
	FrameBuffer* frameBuffer = new FrameBuffer();
	unsigned int& captureFBO = frameBuffer->mFBO;
	unsigned int& captureRBO = frameBuffer->mRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, widthAndHeight, widthAndHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
	return frameBuffer;
}

FrameBuffer::FrameBuffer(unsigned int width, unsigned height,unsigned int flag)
{
	mWidth = width;
	mHeight = height;

	//生成FBO对象并绑定
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	if (flag == 0) {
		//生成颜色附件，并加入FBO
		mColorAttachment = Texture::createColorAttachment(mWidth, mHeight, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment->getTexture(), 0);
		//生成depth Stencil附件，加入FBO
		mDepthStencilAttachment = Texture::createDepthStencilAttachment(width, height, 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthStencilAttachment->getTexture(), 0);
		mDepthStencilAttachment->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}
	else if (flag == 1) {
		//生成颜色附件，并加入FBO
		mColorAttachment = Texture::createColorAttachment(mWidth, mHeight, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment->getTexture(), 0);
		//生成depth Stencil附件，加入FBO
		mDepthAttachment = Texture::createDepthAttachment(width, height, 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthAttachment->getTexture(), 0);
		mDepthAttachment->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		
	}
	else if(flag==2){
		mDepthStencilAttachment = Texture::createDepthAttachment(width, height, 1);
		mDepthStencilAttachment->bind();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthStencilAttachment->getTexture(), 0); 
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		
	}
	//检查当前构造的FBO是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "GL_FRAMEBUFFER_NOT_COMPLETE" << std::endl;
	}
	//解绑
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
	if (mFBO) {
		glDeleteFramebuffers(1, &mFBO);
	}
	if (mColorAttachment != nullptr) {
		delete mColorAttachment;
	}
	if (mDepthStencilAttachment != nullptr) {
		delete mDepthStencilAttachment;
	}
}
