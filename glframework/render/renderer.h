#pragma once
#include"../core.h"

#include"../../application/camera/camera.h"
#include"../light/directionLight.h"
#include"../light/ambientLight.h"
#include"../light/pointLight.h"
#include"../light/spotLight.h"
#include"../shader.h"
#include"../material/phongMaterial.h"
#include"../material/opacityMaskMaterial.h"
#include"../material/whiteMaterial.h"
#include"../material/depthMaterial.h"
#include"../material/screenMaterial.h"
#include"../material/cubeMaterial.h"
#include"../material/phongEnvMaterial.h"
#include"../material/phongInstancedMaterial.h"
#include"../material/grassInstancedMaterial.h"
#include"../material/blinnPhongMaterial.h"
#include"../material/gammaBlinnPhongMaterial.h"
#include"../material/shadowMappingDepthMaterial.h"
#include"../material/shadowMappingMaterial.h"
#include"../material/pointShadowDepthMaterial.h"
#include"../material/pointShadowMaterial.h"
#include"../material/normalMappingMaterial.h"
#include"../material/parallaxMappingMaterial.h"
#include"../material/pBRMaterial.h"
#include"../material/cameraMaterial.h"
#include"../material/irradianceConvolutionMaterial.h"
#include"../material/prefilterMaterial.h"
#include"../material/brdfMaterial.h"


#include"../mesh/mesh.h"
#include"../mesh/instancedMesh.h"
#include"../scene.h"

#include<iostream>
#include<vector>
class Renderer {
public:
	Renderer();
	~Renderer();
	//渲染功能函数：1.每次调用都会渲染一帧
	void render(
		Scene* scene,
		Camera* camera,
		DirectionLight* dirLight,
		const std::vector<PointLight*>& pointLights,
		AmbientLight* ambLight,
		unsigned int fbo=0
	);
	void renderObject(
		Object* object,
		Camera* camera,
		DirectionLight* dirLight,
		const std::vector<PointLight*>& pointLights,
		AmbientLight* ambLight
	);
	void setClearColor(glm::vec3 color);
public:
	Material* mGlobalMaterial{ nullptr };
private:
	//根据material不同挑选shader
	Shader* pickShader(MaterialType type);

	void setDepthState(Material* material);
	void setPolygonOffsetState(Material* material);
	void setStencilState(Material* material);
	void setBlendState(Material* material);
	void setFaceCallingState(Material* material);

	void projectObject(Object* obj);
private:
	//生成多种不同的shader对象
	//根据材质类型的不同，挑选使用哪个shader对象
	Shader* mPhongShader{ nullptr };
	Shader* mWhiteShader{ nullptr };
	Shader* mDepthShader{ nullptr };
	Shader* mOpacityMaskShader{ nullptr };
	Shader* mScreenShader{ nullptr };
	Shader* mCubeShader{ nullptr };
	Shader* mPhongEnvShader{ nullptr };
	Shader* mPhongInstancedShader{ nullptr };
	Shader* mGrassInstancedShader{ nullptr };
	Shader* mBlinnPhongShader{ nullptr };
	Shader* mGammaBlinnPhongShader{ nullptr };
	Shader* mShadowMappingDepthShader{ nullptr };
	Shader* mShadowMappingShader{ nullptr };
	Shader* mPointShadowDepthMaterialShader{ nullptr };
	Shader* mPointShadowMaterialShader{ nullptr };
	Shader* mNormalMappingMaterialShader{ nullptr };
	Shader* mParallaxMappingMaterialShader{ nullptr };
	Shader* mPBRMaterialShader{ nullptr };
	Shader* mCameraMaterialShader{ nullptr };
	Shader* mIrradianceConvolutionMaterialShader{ nullptr };
	Shader* mPrefilterMaterialShader{ nullptr };
	Shader* mBrdfMaterialShader{ nullptr };

	//不透明物体和透明物体的队列
	//*每一帧绘制前都需清空两个队列
	std::vector<Mesh*> mOpacityObjects;
	std::vector<Mesh*> mTransparentObjects;
};
