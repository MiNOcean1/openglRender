#pragma once
#include"../core.h"
enum class MaterialType {
	PhongMaterial,
	WhiteMaterial,
	DepthMaterial,
	OpacityMaskMaterial,
	ScreenMaterial,
	CubeMaterial,
	PhongEnvMaterial,
	PhongInstancedMaterial,
	GrassInstancedMaterial,
	BlinnPhongMaterial,
	GammaBlinnPhongMaterial,
	ShadowMappingDepthMaterial,
	ShadowMappingMaterial,
	PointShadowDepthMaterial,
	PointShadowMaterial,
	NormalMappingMaterial,
	ParallaxMappingMaterial,
	PBRMaterial,
	CameraMaterial,
	IrradianceConvolutionMaterial,
	PrefilterMaterial,
	BrdfMaterial
};
class Material {
public:
	Material();
	~Material();
public:
	MaterialType mType;

	//深度检测相关
	bool mDepthTest{ true };//是否开启深度检测
	GLenum mDepthFunc{ GL_LEQUAL };//深度检测模式
	bool mDepthWrite{ true };//是否更新深度缓冲


	//polygonOffset相关
	bool mPolygonOffset{ false };
	unsigned int mPolygonOffsetType{ GL_POLYGON_OFFSET_FILL };
	float mFactor{ 0.0f };
	float mUnit{ 0.0f };

	//模板测试相关
	bool mStencilTest{ false };
	GLenum mStencilFunc{ GL_NEVER };
	GLint mStencilRef{ 0 };
	GLuint mStencilFuncMask{ 0xFF };
	GLenum mStencilSFail{ GL_KEEP };
	GLenum mStencilZFail{ GL_KEEP };
	GLenum mStencilZPass{ GL_KEEP };
	GLuint mStencilMask{ 0x00 };

	//颜色混合相关
	bool mBlend{ false }; 
	unsigned int mSFactor{ GL_SRC_ALPHA };
	unsigned int mDFactor{ GL_ONE_MINUS_SRC_ALPHA };

	//面剔除相关
	bool mFaceCulling{ false };
	unsigned int mFrontFace{ GL_CCW };
	unsigned int mCullFace{ GL_BACK };

	float mOpacity{ 1.0f };;
};