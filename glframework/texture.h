#pragma once
#include"core.h"
#include"perlin.h"
#include<vector>
#include<iostream>
class Texture {
public:
	static Texture* createTexture(const char* path, unsigned int unit);
	static Texture* createTextureFromMemory(
		const std::string& path,
		unsigned int unit,
		unsigned char* dataIn,
		uint32_t widthIn,
		uint32_t heightIn
	);
	static Texture* createColorAttachment(unsigned int width, unsigned int height, unsigned int unit, bool cubeFlag = false);
	static Texture* createDepthStencilAttachment(unsigned int width, unsigned int height, unsigned int unit);
	static Texture* createDepthAttachment(unsigned int width, unsigned int height, unsigned int unit);
	static Texture* createDepthAttachmentCube(unsigned int width, unsigned int height, unsigned int unit);
	static Texture* createEnvCube(unsigned int widthAndheight, unsigned int unit, bool genMipmap);
	static Texture* createBrdfLUT(unsigned int width, unsigned int height, unsigned int unit);

	Texture(const char* texturePath, unsigned int unit);//从外存读取texture
	Texture(const char* texturePath, unsigned int unit,int format);//从外存读取texture
	Texture(//从内存读取texture
		unsigned int unit,
		unsigned char* dataIn,
		uint32_t widthIn,
		uint32_t heightIn
	);
	Texture(unsigned int width, unsigned int height, unsigned int unit, bool cubeFlag = false);//创建空白纹理
	Texture(
		const std::vector<const char*>paths,
		unsigned int unit,
		bool depthFlag=false
	);//创建天空盒
	Texture(
		int width,
		int height,
		float scale,
		unsigned int unit
	);//生成perlin噪声图像
	Texture();
	~Texture();
	void bind();
	void setUnit(unsigned int unit);

	int getWidth()const;
	int getHeight()const;
	GLuint getTexture()const;
	unsigned int getUnit()const;

	void exportDepthMap(unsigned int depthFBO, const char* filename);
private:
	GLuint mTexture{ 0 };
	int mWidth{ 0 };
	int mHeight{ 0 };
	int mChannels{ 0 };//读入图片的本身的通道种类
	unsigned int mTextureTarget{ GL_TEXTURE_2D };
	GLuint mUnit{ 0 };//该纹理绑定在了哪个单元

	//注意：静态 属于类的不属于某个对象
	static std::map<std::string, Texture*>mTextureCache;
};