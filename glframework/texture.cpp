#include "texture.h"
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION 
#include"../application/stb_image.h"
#include"../application/stb_image_write.h"
std::map<std::string, Texture*>Texture::mTextureCache{};

Texture* Texture::createTexture(const char* path, unsigned int unit)
{
	//检查是否缓存过纹理
	auto iter = mTextureCache.find(path);
	if (iter != mTextureCache.end()) {
		return iter->second;
	}
	//如果本路径对应的texture没有生成过，则重新生成
	auto texture = new Texture(path, unit);
	mTextureCache[path] = texture;
	return texture;
}

Texture* Texture::createTextureFromMemory(const std::string& path, unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn)
{
	//1 检查是否缓存过本路径对应的纹理对象
	auto iter = mTextureCache.find(path);
	if (iter != mTextureCache.end())
	{
		//对于iterater, first->key, second->value
		return iter->second;
	}

	//2 如果本路径对应的texture没有生成过,则重新生成
	auto texture = new Texture(unit, dataIn, widthIn, heightIn);
	mTextureCache[path] = texture;
	return texture;
}

Texture* Texture::createColorAttachment(unsigned int width, unsigned int height, unsigned int unit,bool cubeFlag)
{
	return new Texture(width, height, unit, cubeFlag);
}

Texture* Texture::createDepthStencilAttachment(unsigned int width, unsigned int height, unsigned int unit)
{	
	Texture* depthStencilTexture = new Texture();
	//生成depth Stencil附件，加入FBO
	unsigned int depthStencil;
	glGenTextures(1, &depthStencil);
	glBindTexture(GL_TEXTURE_2D, depthStencil);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	depthStencilTexture->mTexture = depthStencil;
	depthStencilTexture->mWidth = width;
	depthStencilTexture->mHeight = height;
	depthStencilTexture->mUnit = unit;
	return depthStencilTexture;
}

Texture* Texture::createDepthAttachment(unsigned int width, unsigned int height, unsigned int unit)
{
	Texture* depthTexture = new Texture();

	// 生成深度纹理ID
	unsigned int depthBuffer;
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);

	// 注意：这里使用纯深度格式，而非深度+模板
	// 内部格式为GL_DEPTH_COMPONENT24（24位深度精度）
	// 也可根据需求使用GL_DEPTH_COMPONENT32F（更高精度的浮点深度）
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_DEPTH_COMPONENT24,  // 仅深度分量，24位精度
		width,
		height,
		0,
		GL_DEPTH_COMPONENT,    // 格式为深度分量
		GL_UNSIGNED_INT,       // 24位深度对应无符号整数类型
		NULL                   // 初始数据为NULL（空纹理）
	);

	// 深度纹理通常使用GL_NEAREST过滤（避免插值导致深度精度问题）
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// 可选：设置深度纹理的环绕方式（通常设为GL_CLAMP_TO_BORDER或GL_CLAMP_TO_EDGE）
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // 边界值设为最大深度（1.0）
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// 赋值给Texture对象
	depthTexture->mTexture = depthBuffer;
	depthTexture->mWidth = width;
	depthTexture->mHeight = height;
	depthTexture->mUnit = unit;

	return depthTexture;
}

Texture* Texture::createDepthAttachmentCube(unsigned int width, unsigned int height, unsigned int unit)
{
	Texture* depthStencilTexture = new Texture();
	//生成depth Stencil附件，加入FBO
	unsigned int depthStencil;
	glGenTextures(1, &depthStencil);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthStencil);
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	depthStencilTexture->mTextureTarget = GL_TEXTURE_CUBE_MAP;
	depthStencilTexture->mTexture = depthStencil;
	depthStencilTexture->mWidth = width;
	depthStencilTexture->mHeight = height;
	depthStencilTexture->mUnit = unit;
	return depthStencilTexture;
}

Texture* Texture::createEnvCube(unsigned int widthAndHeight, unsigned int unit,bool genMipmap)
{
	Texture* envCubemapTexture=new Texture();
	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
			widthAndHeight, widthAndHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,genMipmap? GL_LINEAR_MIPMAP_LINEAR: GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (genMipmap)
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	envCubemapTexture->mTextureTarget= GL_TEXTURE_CUBE_MAP;
	envCubemapTexture->mTexture = envCubemap;
	envCubemapTexture->mWidth = widthAndHeight;
	envCubemapTexture->mHeight = widthAndHeight;
	envCubemapTexture->mUnit = unit;
	return envCubemapTexture;
}

Texture* Texture::createBrdfLUT(unsigned int width, unsigned int height, unsigned int unit)
{
	Texture* brdfLUTTexture=new Texture();
	unsigned int brdfLUT;
	glGenTextures(1, &brdfLUT);

	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, brdfLUT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, 0);
	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	brdfLUTTexture->mTexture = brdfLUT;
	brdfLUTTexture->mWidth = width;
	brdfLUTTexture->mHeight = height;
	brdfLUTTexture->mUnit = unit;
	return brdfLUTTexture;
}



Texture::Texture(const char* texturePath,unsigned int unit)
{	
	//用stbImage读取图片
	//翻转y轴,jdp的（0,0）在左上角，而OpenGL在左下角
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturePath, &mWidth, &mHeight, &mChannels,0);//相对文件路径，宽度，长度，读入图片的本身的通道种类（RGB/RGBA/GREY）,期望读出来的数据格式（RGB/RGBA/GREY）
	if (!data) {
		printf("纹理加载失败：%s，错误原因：%s\n", texturePath, stbi_failure_reason());
		return;
	}
	//创建纹理对象并且绑定纹理单元
	glGenTextures(1, &mTexture);//创建多少个纹理对象，创建出的纹理对象编号数组（1个所以可以不用数组）
	//--激活纹理单元
	mUnit = unit;
	glActiveTexture(GL_TEXTURE0 + mUnit);//激活mUnit号单元
	//--绑定纹理对象
	glBindTexture(GL_TEXTURE_2D, mTexture);//将纹理绑定在GL状态机的哪个插槽，绑定此纹理编号对应的纹理对象
	GLenum format=4;
	if (mChannels == 1)
		format = GL_RED;
	else if (mChannels == 3)
		format = GL_RGB;
	else if (mChannels == 4)
		format = GL_RGBA;
	//传输纹理数据
	GLenum internalFormat = format;
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, data);//给GL状态机的哪个插槽纹理输送数据，给mipmap的哪个层级输送数据，希望纹理对象中图片像素格式，最终宽度，最终高度，历史遗留（总是0），原始图片格式，单通道数据格式（0-255），数据数组指针

	//生成MipMap
	glGenerateMipmap(GL_TEXTURE_2D);

	//**释放数据
	stbi_image_free(data);

	//设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//需要的像素 > 纹理像素 Linear（线性插值）
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//需要的像素 < 纹理像素 Nearest（邻近采样）
	//mipmap设置
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	
	//设置纹理包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//U，横向
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//V，纵向
}

Texture::Texture(const char* texturePath, unsigned int unit,int format)
{
	stbi_set_flip_vertically_on_load(true);
	float* data = stbi_loadf(texturePath, &mWidth, &mHeight, &mChannels, 0);
	if (data)
	{
		glGenTextures(1, &mTexture);
		glBindTexture(GL_TEXTURE_2D,mTexture);
		mUnit = unit;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, mWidth, mHeight, 0, GL_RGB, format, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}

}

Texture::Texture(unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn)
{
	mUnit = unit;

	//1 stbImage 读取图片
	int channels;

	//--反转y轴
	stbi_set_flip_vertically_on_load(true);

	//计算整张图片的大小
	//Assimp规定:如果内嵌纹理是png或者jpg压缩格式的话,height = 0, width就代表了图片大小
	uint32_t dataInSize = 0; //整个图片的大小(包括压缩部分的数据和文件头,不是像素的大小)
	if (!heightIn)
	{
		dataInSize = widthIn;
	}
	else
	{
		//如果内嵌图片不是压缩格式,height = 正常高度,width = 正常宽度
		//偷懒: 统一认为数据格式都是RGBA
		dataInSize = widthIn * heightIn * 4;
	}

	unsigned char* data = stbi_load_from_memory(dataIn, dataInSize, &mWidth, &mHeight, &channels, STBI_rgb_alpha);

	//2 生成纹理并且激活单元绑定
	glGenTextures(1, &mTexture);
	//--激活纹理单元--
	glActiveTexture(GL_TEXTURE0 + mUnit);
	//--绑定纹理对象--
	glBindTexture(GL_TEXTURE_2D, mTexture);

	//3 传输纹理数据,开辟显存
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	//***释放数据 
	stbi_image_free(data);

	//4 设置纹理的过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	//5 设置纹理的包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v
}

Texture::Texture(unsigned int width, unsigned int height, unsigned int unit,bool cubeFlag)
{	
	mWidth = width;
	mHeight = height;
	mUnit = unit;
	//创建纹理对象并且绑定纹理单元
	glGenTextures(1, &mTexture);//创建多少个纹理对象，创建出的纹理对象编号数组（1个所以可以不用数组）
	//--激活纹理单元
	glActiveTexture(GL_TEXTURE0 + mUnit);//激活mUnit号单元
	if (!cubeFlag) {
		mTextureTarget = GL_TEXTURE_CUBE_MAP;
		//--绑定纹理对象
		glBindTexture(GL_TEXTURE_2D, mTexture);//将纹理绑定在GL状态机的哪个插槽，绑定此纹理编号对应的纹理对象

		//传输纹理数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//需要的像素 > 纹理像素 Linear（线性插值
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//需要的像素 > 纹理像素 Linear（线性插值）
	}
	else {
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);//将纹理绑定在GL状态机的哪个插槽，绑定此纹理编号对应的纹理对象
		const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		for (GLuint i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}

//传入顺序：右左上下后前(+x -x +y -y +z -z)
Texture::Texture(const std::vector<const char*> paths, unsigned int unit,bool depthFlag)
{
	//cubemap不需要翻转y轴
	stbi_set_flip_vertically_on_load(false);
	mUnit = unit;
	mTextureTarget = GL_TEXTURE_CUBE_MAP;
	//创建cubeMap对象
	glGenTextures(1, &mTexture);//创建多少个纹理对象，创建出的纹理对象编号数组（1个所以可以不用数组）
	//--激活纹理单元
	glActiveTexture(GL_TEXTURE0 + mUnit);//激活mUnit号单元
	//--绑定纹理对象
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);//将纹理绑定在GL状态机的哪个插槽，绑定此纹理编号对应的纹理对象

	if (depthFlag) {
		//循环读取六张贴图，并且放置在cubeMap的6个GPU空间
		int width = 0, height = 0;
		unsigned char* data = nullptr;
		for (int i = 0;i < paths.size();i++) {
			data = stbi_load(paths[i], &width, &height, &mChannels, STBI_rgb_alpha);
			if (data != nullptr) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			else {
				std::cout << "Error::CubeMap Texture Failed to load at path - " << paths[i] << std::endl;
			}
			stbi_image_free(data);
		}
		//设置纹理过滤方式
		glTexParameteri(mTextureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//需要的像素 > 纹理像素 Linear（线性插值）
		glTexParameteri(mTextureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//需要的像素 < 纹理像素 Nearest（邻近采样）

		//设置纹理包裹方式
		glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);//U，横向
		glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);//V，纵向
	}
	else {
		const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		for (GLuint i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}

Texture::Texture(int width, int height, float scale, unsigned int unit)
{
	mWidth = width;
	mHeight = height;
	Perlin* perlin = new Perlin();
	std::vector<unsigned char>perlinData = perlin->GenerateNoise(width, height, scale);

	//创建纹理对象并且绑定纹理单元
	glGenTextures(1, &mTexture);//创建多少个纹理对象，创建出的纹理对象编号数组（1个所以可以不用数组）
	//--激活纹理单元
	mUnit = unit;
	glActiveTexture(GL_TEXTURE0 + mUnit);//激活mUnit号单元
	//--绑定纹理对象
	glBindTexture(GL_TEXTURE_2D, mTexture);//将纹理绑定在GL状态机的哪个插槽，绑定此纹理编号对应的纹理对象

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, perlinData.data());//给GL状态机的哪个插槽纹理输送数据，给mipmap的哪个层级输送数据，希望纹理对象中图片像素格式，最终宽度，最终高度，历史遗留（总是0），原始图片格式，单通道数据格式（0-255），数据数组指针

	//生成MipMap
	glGenerateMipmap(GL_TEXTURE_2D);

	//设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//需要的像素 > 纹理像素 Linear（线性插值）
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//需要的像素 < 纹理像素 Nearest（邻近采样）
	//mipmap设置
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	//设置纹理包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//U，横向
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//V，纵向
}

Texture::Texture()
{
}

Texture::~Texture()
{
	if (mTexture != 0) {//如果构建出了texture就需要释放
		glDeleteTextures(1,&mTexture);
	}
}

void Texture::bind()
{
	//先切换纹理单元，再绑定纹理对象，不然可能绑其他纹理单元上去了
	glActiveTexture(GL_TEXTURE0 + mUnit);
	glBindTexture(mTextureTarget, mTexture);
}

void Texture::setUnit(unsigned int unit)
{
	mUnit = unit;
}

int Texture::getWidth() const
{
	return mWidth;
}

int Texture::getHeight() const
{
	return mHeight;
}

GLuint Texture::getTexture() const
{
	return mTexture;
}

unsigned int Texture::getUnit() const
{
	return mUnit;
}

void Texture::exportDepthMap(unsigned int depthFBO, const char* filename)
{
	// 1. 绑定目标帧缓冲
	glBindFramebuffer(GL_READ_FRAMEBUFFER, depthFBO);
	float width = this->mWidth;
	float height = this->mHeight;
	// 2. 读取深度缓冲数据（注意：深度值是 float 类型，范围 [0,1]）
	std::vector<float> depthData(width * height);
	glReadPixels(0, 0, width, height,
		GL_DEPTH_COMPONENT,  // 读取深度分量
		GL_FLOAT,            // 数据类型为 float
		depthData.data());   // 存储数据的缓冲区

	// 3. 将深度值转换为 [0,255] 的灰度值（用于可视化）
	std::vector<unsigned char> imageData(width * height);
	for (int i = 0; i < width * height; ++i) {
		// 深度值通常是非线性的，这里直接线性映射到 [0,255]
		float depthValue = depthData[i];
		imageData[i] = static_cast<unsigned char>(depthValue * 255.0f);
	}

	// 4. 翻转图像（OpenGL 原点在左下角，图片通常原点在左上角）
	stbi_flip_vertically_on_write(1);

	// 5. 保存为 PNG 图片（单通道灰度图）
	stbi_write_png(filename, width, height, 1,
		imageData.data(), width * 1);  // 每行字节数：width * 通道数（1）

	// 解绑帧缓冲
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}
