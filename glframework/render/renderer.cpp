#include "renderer.h"

#include<string>
#include<algorithm>
Renderer::Renderer()
{
	mPhongShader = new Shader("assets/shaders/phong.vert", "assets/shaders/phong.frag",nullptr);
	mWhiteShader = new Shader("assets/shaders/white.vert", "assets/shaders/white.frag", nullptr);
	mDepthShader = new Shader("assets/shaders/depth.vert", "assets/shaders/depth.frag", nullptr);
	mOpacityMaskShader = new Shader("assets/shaders/phongOpacityMask.vert", "assets/shaders/phongOpacityMask.frag", nullptr);
	mScreenShader = new Shader("assets/shaders/screen.vert", "assets/shaders/screen.frag", nullptr);
	mCubeShader = new Shader("assets/shaders/cube.vert", "assets/shaders/cube.frag", nullptr);
	mPhongEnvShader = new Shader("assets/shaders/phongEnv.vert", "assets/shaders/phongEnv.frag", nullptr);
	mPhongInstancedShader = new Shader("assets/shaders/phongInstanced.vert", "assets/shaders/phongInstanced.frag", nullptr);
	mGrassInstancedShader = new Shader("assets/shaders/grassInstanced.vert", "assets/shaders/grassInstanced.frag", nullptr);
	mBlinnPhongShader = new Shader("assets/shaders/blinnPhong.vert", "assets/shaders/blinnPhong.frag", nullptr);
	mGammaBlinnPhongShader = new Shader("assets/shaders/gammaBlinnPhong.vert", "assets/shaders/gammaBlinnPhong.frag", nullptr);
	mShadowMappingDepthShader = new Shader("assets/shaders/shadowMappingDepth.vert", "assets/shaders/shadowMappingDepth.frag", nullptr);
	mShadowMappingShader = new Shader("assets/shaders/shadowMapping.vert", "assets/shaders/shadowMapping.frag", nullptr);
	mPointShadowDepthMaterialShader = new Shader("assets/shaders/pointShadowDepth.vert", "assets/shaders/pointShadowDepth.frag", "assets/shaders/pointShadowDepth.geom");
	mPointShadowMaterialShader = new Shader("assets/shaders/pointShadow.vert", "assets/shaders/pointShadow.frag",nullptr);
	mNormalMappingMaterialShader = new Shader("assets/shaders/normalMapping.vert", "assets/shaders/normalMapping.frag", nullptr);
	mParallaxMappingMaterialShader = new Shader("assets/shaders/parallaxMapping.vert", "assets/shaders/parallaxMapping.frag", nullptr);
	mPBRMaterialShader = new Shader("assets/shaders/pBR.vert", "assets/shaders/pBR.frag", nullptr);
	mCameraMaterialShader = new Shader("assets/shaders/camera.vert", "assets/shaders/camera.frag", nullptr);
	mIrradianceConvolutionMaterialShader = new Shader("assets/shaders/irradianceConvolution.vert", "assets/shaders/irradianceConvolution.frag", nullptr);
	mPrefilterMaterialShader = new Shader("assets/shaders/prefilter.vert", "assets/shaders/prefilter.frag", nullptr);
	mBrdfMaterialShader = new Shader("assets/shaders/brdf.vert", "assets/shaders/brdf.frag", nullptr);
}

Renderer::~Renderer()
{
}

void Renderer::render(
	Scene* scene,
	Camera* camera,
	DirectionLight* dirLight,
	const std::vector<PointLight*>& pointLights, 
	AmbientLight* ambLight,
	unsigned int fbo
)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);  
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


	//清理画布前需要全部打开，不然如果绘制的最后一个物体关闭了深度检测将无法清理depthMask
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	//默认颜色混合
	glDisable(GL_BLEND);

	//开启测试，设置基本的写入状态，打开模板测试写入
	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0xFF);//保证模板缓冲可以被清理

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_POLYGON_OFFSET_LINE);
	//清理画布
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//清空两个队列
	mOpacityObjects.clear();
	mTransparentObjects.clear();

	projectObject(scene);
	
	std::sort(mTransparentObjects.begin(), mTransparentObjects.end(), 
		[camera] (const Mesh* a, const Mesh* b) {
			//计算A的相机系的z
			auto modelMatA = a->getModelMat();
			auto worldPositionA = modelMatA * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//初始坐标都是（0,0,0）
			auto viewMatA = camera->getViewMat();
			auto cameraPositonA = viewMatA * worldPositionA;

			//计算B的相机系的z
			auto modelMatB = b->getModelMat();
			auto worldPositionB = modelMatB * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//初始坐标都是（0,0,0）
			auto viewMatB = camera->getViewMat();
			auto cameraPositonB = viewMatB * worldPositionB;
			
			return cameraPositonA.z < cameraPositonB.z;
	});

	
	//渲染两个队列
	for (auto obj : mOpacityObjects) {
		renderObject(obj, camera, dirLight, pointLights, ambLight);
	}
	for (auto obj : mTransparentObjects) {
		renderObject(obj, camera, dirLight, pointLights, ambLight);
	}
}

void Renderer::renderObject(
	Object* object,
	Camera* camera,
	DirectionLight* dirLight,
	const std::vector<PointLight*>& pointLights,
	AmbientLight* ambLight
)
		
{
	//判断是Mesh还是Object，如果是Mesh需要渲染
	if (object->getType() == ObjectType::Mesh||object->getType() == ObjectType::InstancedMesh) {
		auto mesh = (Mesh*)object;
		auto geometry = mesh->mGeometry;

		//考察是否拥有全局材质
		Material* material = nullptr;
		if (mGlobalMaterial != nullptr) {
			material = mGlobalMaterial;
		}
		else {
			material = mesh->mMaterial;
		}
		
		//设置渲染状态
		setDepthState(material);//检测深度检测
		setPolygonOffsetState(material);//检测polygonOffset
		setStencilState(material);//检测模板测试
		setBlendState(material);//检测颜色混合
		setFaceCallingState(material);//检测面剔除

		/*阴影映射
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		*/
		/*
		//点阴影映射
		GLfloat aspect = (GLfloat)1000 / (GLfloat)750;
		GLfloat near = 1.0f;
		GLfloat far = 25.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
		glm::vec3 lightPos = pointLights[0]->getPosition();
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
		*/
		//1决定使用哪个Shader
		Shader* shader = pickShader(material->mType);
		shader->begin();
		//2更新shader的uniform
		switch (material->mType)
		{
		case MaterialType::PhongMaterial: {

			PhongMaterial* phoneMaterial = (PhongMaterial*)material;//把material强制转换为phongMaterial，否则无法调用phongMaterial自己的参数
			//diffuse贴图
			//将纹理与纹理单元进行挂钩
			phoneMaterial->mDiffuse->bind();
			//phoneMaterial->mSpecular->bind();
			//将纹理采样器与纹理单元挂钩
			shader->setInt("Sampler0", phoneMaterial->mDiffuse->getUnit());
			//shader->setInt("Sampler1", 1);
			//MVP变换
			shader->setMat4("modelMat", mesh->getModelMat());
			shader->setMat4("viewMat", camera->getViewMat());
			shader->setMat4("projectionMat", camera->projectionMat());
			//光源参数的uniform更新

			/*
			//spotLight的更新
			shader->setV3F("spotLight.position", spotLight->getPosition());
			shader->setV3F("spotLight.color", spotLight->mColor);
			shader->setV1F("spotLight.specularIntensity", spotLight->mSpecularIntensity);
			shader->setV1F("spotLight.beta", glm::radians(spotLight->mBeta));
			shader->setV1F("spotLight.theta", glm::radians(spotLight->mTheta));
			shader->setV3F("spotLight.spotDir", spotLight->mSpotDir);
*/
			//directionLight的更新
			shader->setV3F("directionLight.direction", dirLight->mDirection);
			shader->setV3F("directionLight.color", dirLight->mColor);
			shader->setV1F("directionLight.specularIntensity", dirLight->mSpecularIntensity);

			//pointLight的更新
			for (int i = 0;i < pointLights.size();i++) {
				auto pointLight = pointLights[i];
				std::string baseName = "pointLights[0]";
				baseName[12] += i;
				shader->setV3F(baseName + ".position", pointLight->getPosition());
				shader->setV3F(baseName + ".color", pointLight->mColor);
				shader->setV1F(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
				shader->setV1F(baseName + ".k2", pointLight->mK2);
				shader->setV1F(baseName + ".k1", pointLight->mK1);
				shader->setV1F(baseName + ".k0", pointLight->mK0);
			}

			shader->setV1F("shiness", phoneMaterial->mShiness);
			shader->setV3F("ambientColor", ambLight->mColor);
			//相机信息
			shader->setV3F("cameraPosition", camera->mPosition);
			//法线矩阵
			shader->setMat3("normalMatrix", glm::mat3(glm::inverse(glm::transpose(mesh->getModelMat()))));
			shader->setV1F("opacity", material->mOpacity);
		}
										break;
		case MaterialType::OpacityMaskMaterial: {

			OpacityMaskMaterial* opacityMaskMaterial = (OpacityMaskMaterial*)material;//把material强制转换为phongMaterial，否则无法调用phongMaterial自己的参数
			//diffuse贴图
			//将纹理与纹理单元进行挂钩
			opacityMaskMaterial->mDiffuse->bind();
			opacityMaskMaterial->mOpacityMask->bind();
			
			//将纹理采样器与纹理单元挂钩
			shader->setInt("Sampler0", opacityMaskMaterial->mDiffuse->getUnit());
			shader->setInt("Sampler1", opacityMaskMaterial->mOpacityMask->getUnit());//opacityMask的采样

			//MVP变换
			shader->setMat4("modelMat", mesh->getModelMat());
			//shader->setMat4("modelMat", glm::mat4(1.0f));
			shader->setMat4("viewMat", camera->getViewMat());
			shader->setMat4("projectionMat", camera->projectionMat());
			//光源参数的uniform更新
			/*
			//spotLight的更新
			shader->setV3F("spotLight.position", spotLight->getPosition());
			shader->setV3F("spotLight.color", spotLight->mColor);
			shader->setV1F("spotLight.specularIntensity", spotLight->mSpecularIntensity);
			shader->setV1F("spotLight.beta", glm::radians(spotLight->mBeta));
			shader->setV1F("spotLight.theta", glm::radians(spotLight->mTheta));
			shader->setV3F("spotLight.spotDir", spotLight->mSpotDir);
*/
			//directionLight的更新
			shader->setV3F("directionLight.direction", dirLight->mDirection);
			shader->setV3F("directionLight.color", dirLight->mColor);
			shader->setV1F("directionLight.specularIntensity", dirLight->mSpecularIntensity);

			//pointLight的更新
			for (int i = 0;i < pointLights.size();i++) {
				auto pointLight = pointLights[i];
				std::string baseName = "pointLights[0]";
				baseName[12] += i;
				shader->setV3F(baseName + ".position", pointLight->getPosition());
				shader->setV3F(baseName + ".color", pointLight->mColor);
				shader->setV1F(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
				shader->setV1F(baseName + ".k2", pointLight->mK2);
				shader->setV1F(baseName + ".k1", pointLight->mK1);
				shader->setV1F(baseName + ".k0", pointLight->mK0);
			}

			shader->setV1F("shiness", opacityMaskMaterial->mShiness);
			shader->setV3F("ambientColor", ambLight->mColor);
			//相机信息
			shader->setV3F("cameraPosition", camera->mPosition);
			//法线矩阵
			shader->setMat4("normalMatrix", glm::inverse(glm::transpose(mesh->getModelMat())));
			shader->setV1F("opacity", material->mOpacity);
		}
										break;
		case MaterialType::WhiteMaterial: {
			shader->setMat4("modelMat", mesh->getModelMat());
			//shader->setMat4("modelMat", glm::mat4(1.0f));
			shader->setMat4("viewMat", camera->getViewMat());
			shader->setMat4("projectionMat", camera->projectionMat());

			shader->setV3F("directionLight.direction", dirLight->mDirection);
			shader->setV3F("directionLight.color", dirLight->mColor);
			shader->setV1F("directionLight.specularIntensity", dirLight->mSpecularIntensity);

			//pointLight的更新
			for (int i = 0;i < pointLights.size();i++) {
				auto pointLight = pointLights[i];
				std::string baseName = "pointLights[0]";
				baseName[12] += i;
				shader->setV3F(baseName + ".position", pointLight->getPosition());
				shader->setV3F(baseName + ".color", pointLight->mColor);
				shader->setV1F(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
				shader->setV1F(baseName + ".k2", pointLight->mK2);
				shader->setV1F(baseName + ".k1", pointLight->mK1);
				shader->setV1F(baseName + ".k0", pointLight->mK0);
			}

			shader->setV1F("shiness", 32.0f);
			shader->setV3F("ambientColor", ambLight->mColor);
			//相机信息
			shader->setV3F("cameraPosition", camera->mPosition);
			//法线矩阵
			shader->setMat4("normalMatrix", glm::inverse(glm::transpose(mesh->getModelMat())));
			shader->setV1F("opacity", material->mOpacity);

		}
										break;
		case MaterialType::DepthMaterial: {
			shader->setMat4("modelMat", mesh->getModelMat());
			//shader->setMat4("modelMat", glm::mat4(1.0f));
			shader->setMat4("viewMat", camera->getViewMat());
			shader->setMat4("projectionMat", camera->projectionMat());
			shader->setV1F("far",camera->mFar);
			shader->setV1F("near",camera->mNear);
			
		}
										break;
		case MaterialType::ScreenMaterial: {
			ScreenMaterial* screenMaterial = (ScreenMaterial*)material;
			screenMaterial->mScreenTexture->bind();
			shader->setInt("Sampler", screenMaterial->mScreenTexture->getUnit());
			shader->setV1F("textureWidth", screenMaterial->mScreenTexture->getWidth());
			shader->setV1F("textureHeight", screenMaterial->mScreenTexture->getHeight());


		}
										break;
		case MaterialType::CubeMaterial: {
			CubeMaterial* cubeMaterial = (CubeMaterial*)material;
			mesh->setPosition(camera->mPosition);
			cubeMaterial->mDiffuse->bind();
			shader->setInt("Sampler", cubeMaterial->mDiffuse->getUnit());
			shader->setMat4("modelMat", mesh->getModelMat());
			shader->setMat4("viewMat", camera->getViewMat());
			shader->setMat4("projectionMat", camera->projectionMat());
		}
										break;
		case MaterialType::PhongEnvMaterial: {

			PhongEnvMaterial* phongEnvMaterial = (PhongEnvMaterial*)material;//把material强制转换为phongMaterial，否则无法调用phongMaterial自己的参数

			mesh->setPosition(camera->mPosition);
			//diffuse贴图
			//将纹理与纹理单元进行挂钩
			//phongEnvMaterial->mDiffuse->bind();
			//phongEnvMaterial->mSpecular->bind();
			phongEnvMaterial->mEnv->bind();
			//将纹理采样器与纹理单元挂钩
			//shader->setInt("Sampler0", phongEnvMaterial->mDiffuse->getUnit());
			//shader->setInt("Sampler1", phongEnvMaterial->mSpecular->getUnit());
			shader->setInt("envSampler", phongEnvMaterial->mEnv->getUnit());
			//MVP变换
			shader->setMat4("modelMat", mesh->getModelMat());
			shader->setMat4("viewMat", camera->getViewMat());
			shader->setMat4("projectionMat", camera->projectionMat());
			glm::vec3 center = glm::cross(camera->mUp, camera->mRight);
			shader->setV3F("cameraView", center );
			//光源参数的uniform更新
			/*
			//spotLight的更新
			shader->setV3F("spotLight.position", spotLight->getPosition());
			shader->setV3F("spotLight.color", spotLight->mColor);
			shader->setV1F("spotLight.specularIntensity", spotLight->mSpecularIntensity);
			shader->setV1F("spotLight.beta", glm::radians(spotLight->mBeta));
			shader->setV1F("spotLight.theta", glm::radians(spotLight->mTheta));
			shader->setV3F("spotLight.spotDir", spotLight->mSpotDir);
*/
			//directionLight的更新
			shader->setV3F("directionLight.direction", dirLight->mDirection);
			shader->setV3F("directionLight.color", dirLight->mColor);
			shader->setV1F("directionLight.specularIntensity", dirLight->mSpecularIntensity);

			//pointLight的更新
			for (int i = 0;i < pointLights.size();i++) {
				auto pointLight = pointLights[i];
				std::string baseName = "pointLights[0]";
				baseName[12] += i;
				shader->setV3F(baseName + ".position", pointLight->getPosition());
				shader->setV3F(baseName + ".color", pointLight->mColor);
				shader->setV1F(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
				shader->setV1F(baseName + ".k2", pointLight->mK2);
				shader->setV1F(baseName + ".k1", pointLight->mK1);
				shader->setV1F(baseName + ".k0", pointLight->mK0);
			}

			shader->setV1F("shiness", phongEnvMaterial->mShiness);
			shader->setV3F("ambientColor", ambLight->mColor);
			//相机信息
			shader->setV3F("cameraPosition", camera->mPosition);
			//法线矩阵
			shader->setMat4("normalMatrix", glm::inverse(glm::transpose(mesh->getModelMat())));
			shader->setV1F("opacity", material->mOpacity);
		}
										break;
		case MaterialType::PhongInstancedMaterial: {
			PhongInstancedMaterial* phoneInstancedMaterial = (PhongInstancedMaterial*)material;//把material强制转换为phongMaterial，否则无法调用phongMaterial自己的参数
			InstancedMesh* instancedMesh = (InstancedMesh*)mesh;
			//diffuse贴图
			//将纹理与纹理单元进行挂钩
			phoneInstancedMaterial->mDiffuse->bind();
			//phoneMaterial->mSpecular->bind();
			//将纹理采样器与纹理单元挂钩
			shader->setInt("Sampler0", 0);
			//shader->setInt("Sampler1", 1);
			//MVP变换

			shader->setMat4("modelMat", mesh->getModelMat());
			//shader->setMat4("modelMat", glm::mat4(1.0f));
			shader->setMat4("viewMat", camera->getViewMat());
			shader->setMat4("projectionMat", camera->projectionMat());
			//光源参数的uniform更新

			/*
			//spotLight的更新
			shader->setV3F("spotLight.position", spotLight->getPosition());
			shader->setV3F("spotLight.color", spotLight->mColor);
			shader->setV1F("spotLight.specularIntensity", spotLight->mSpecularIntensity);
			shader->setV1F("spotLight.beta", glm::radians(spotLight->mBeta));
			shader->setV1F("spotLight.theta", glm::radians(spotLight->mTheta));
			shader->setV3F("spotLight.spotDir", spotLight->mSpotDir);
*/
//directionLight的更新
			shader->setV3F("directionLight.direction", dirLight->mDirection);
			shader->setV3F("directionLight.color", dirLight->mColor);
			shader->setV1F("directionLight.specularIntensity", dirLight->mSpecularIntensity);

			//pointLight的更新
			for (int i = 0;i < pointLights.size();i++) {
				auto pointLight = pointLights[i];
				std::string baseName = "pointLights[0]";
				baseName[12] += i;
				shader->setV3F(baseName + ".position", pointLight->getPosition());
				shader->setV3F(baseName + ".color", pointLight->mColor);
				shader->setV1F(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
				shader->setV1F(baseName + ".k2", pointLight->mK2);
				shader->setV1F(baseName + ".k1", pointLight->mK1);
				shader->setV1F(baseName + ".k0", pointLight->mK0);
			}

			shader->setV1F("shiness", phoneInstancedMaterial->mShiness);
			shader->setV3F("ambientColor", ambLight->mColor);
			//相机信息
			shader->setV3F("cameraPosition", camera->mPosition);
			//法线矩阵
			shader->setMat4("normalMatrix", glm::inverse(glm::transpose(mesh->getModelMat())));
			shader->setV1F("opacity", material->mOpacity);
		}
										break;
		case MaterialType::GrassInstancedMaterial: {
			GrassInstancedMaterial* grassInstancedMaterial = (GrassInstancedMaterial*)material;//把material强制转换为phongMaterial，否则无法调用phongMaterial自己的参数
			InstancedMesh* instancedMesh = (InstancedMesh*)mesh;
			instancedMesh->sortMatrices(camera->getViewMat());
			instancedMesh->updateMatrices();
			//diffuse贴图
			//将纹理与纹理单元进行挂钩
			grassInstancedMaterial->mDiffuse->bind();
			grassInstancedMaterial->mOpacityMask->bind();
			//将纹理采样器与纹理单元挂钩
			shader->setInt("Sampler0", grassInstancedMaterial->mDiffuse->getUnit());
			shader->setInt("Sampler1", grassInstancedMaterial->mOpacityMask->getUnit());
			//MVP变换

			shader->setMat4("modelMat", mesh->getModelMat());
			//shader->setMat4("modelMat", glm::mat4(1.0f));
			shader->setMat4("viewMat", camera->getViewMat());
			shader->setMat4("projectionMat", camera->projectionMat());
			//光源参数的uniform更新


			shader->setV3F("directionLight.direction", dirLight->mDirection);
			shader->setV3F("directionLight.color", dirLight->mColor);
			shader->setV1F("directionLight.specularIntensity", dirLight->mSpecularIntensity);
			shader->setV1F("directionLight.intensity", dirLight->mIntensity);

			//pointLight的更新
			for (int i = 0;i < pointLights.size();i++) {
				auto pointLight = pointLights[i];
				std::string baseName = "pointLights[0]";
				baseName[12] += i;
				shader->setV3F(baseName + ".position", pointLight->getPosition());
				shader->setV3F(baseName + ".color", pointLight->mColor);
				shader->setV1F(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
				shader->setV1F(baseName + ".k2", pointLight->mK2);
				shader->setV1F(baseName + ".k1", pointLight->mK1);
				shader->setV1F(baseName + ".k0", pointLight->mK0);
			}

			shader->setV1F("shiness", grassInstancedMaterial->mShiness);
			shader->setV3F("ambientColor", ambLight->mColor);
			//相机信息
			shader->setV3F("cameraPosition", camera->mPosition);
			//法线矩阵
			//shader->setMat4("normalMatrix", glm::inverse(glm::transpose(mesh->getModelMat())));
			shader->setV1F("opacity", material->mOpacity);
			//贴图特性
			shader->setV1F("uvScale", grassInstancedMaterial->mUVScale);
			shader->setV1F("brightness", grassInstancedMaterial->mBrightness);
			shader->setV1F("time", glfwGetTime());

			//风力
			shader->setV1F("windScale", grassInstancedMaterial->mWindScale);
			shader->setV3F("windDirection", grassInstancedMaterial->mWindDirection);;
			shader->setV1F("phaseScale", grassInstancedMaterial->mPhaseScale);

			//云层
			grassInstancedMaterial->mCloudMask->bind();
			shader->setInt("Sampler2", grassInstancedMaterial->mCloudMask->getUnit());
			shader->setV3F("cloudBlackColor", grassInstancedMaterial->mCloudBlackColor);
			shader->setV3F("cloudWhiteColor", grassInstancedMaterial->mCloudWhiteColor);
			shader->setV1F("cloudUVScale", grassInstancedMaterial->mCloudUVScale);
			shader->setV1F("cloudSpeed", grassInstancedMaterial->mCloudSpeed);
			shader->setV1F("cloudLerp", grassInstancedMaterial->mCloudLerp);
		}
										break;
		case MaterialType::BlinnPhongMaterial: {

			BlinnPhongMaterial* blinnPhoneMaterial = (BlinnPhongMaterial*)material;//把material强制转换为phongMaterial，否则无法调用phongMaterial自己的参数
			//diffuse贴图
			//将纹理与纹理单元进行挂钩
			blinnPhoneMaterial->mDiffuse->bind();
			//phoneMaterial->mSpecular->bind();
			//将纹理采样器与纹理单元挂钩
			shader->setInt("Sampler0", blinnPhoneMaterial->mDiffuse->getUnit());
			//shader->setInt("Sampler1", 1);
			//MVP变换
			shader->setMat4("modelMat", mesh->getModelMat());
			//shader->setMat4("modelMat", glm::mat4(1.0f));
			shader->setMat4("viewMat", camera->getViewMat());
			shader->setMat4("projectionMat", camera->projectionMat());
			//光源参数的uniform更新
			/*
			//spotLight的更新
			shader->setV3F("spotLight.position", spotLight->getPosition());
			shader->setV3F("spotLight.color", spotLight->mColor);
			shader->setV1F("spotLight.specularIntensity", spotLight->mSpecularIntensity);
			shader->setV1F("spotLight.beta", glm::radians(spotLight->mBeta));
			shader->setV1F("spotLight.theta", glm::radians(spotLight->mTheta));
			shader->setV3F("spotLight.spotDir", spotLight->mSpotDir);
			*/
			//directionLight的更新
			shader->setV3F("directionLight.direction", dirLight->mDirection);
			shader->setV3F("directionLight.color", dirLight->mColor);
			shader->setV1F("directionLight.specularIntensity", dirLight->mSpecularIntensity);

			//pointLight的更新
			for (int i = 0;i < pointLights.size();i++) {
				auto pointLight = pointLights[i];
				std::string baseName = "pointLights[0]";
				baseName[12] += i;
				shader->setV3F(baseName + ".position", pointLight->getPosition());
				shader->setV3F(baseName + ".color", pointLight->mColor);
				shader->setV1F(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
				shader->setV1F(baseName + ".k2", pointLight->mK2);
				shader->setV1F(baseName + ".k1", pointLight->mK1);
				shader->setV1F(baseName + ".k0", pointLight->mK0);
			}

			shader->setV1F("shiness", blinnPhoneMaterial->mShiness);
			shader->setV3F("ambientColor", ambLight->mColor);
			//相机信息
			shader->setV3F("cameraPosition", camera->mPosition);
			//法线矩阵
			shader->setMat4("normalMatrix", glm::inverse(glm::transpose(mesh->getModelMat())));
			shader->setV1F("opacity", material->mOpacity);
		}
										break;
		case MaterialType::GammaBlinnPhongMaterial: {

			GammaBlinnPhongMaterial* gammaBlinnPhongMaterial = (GammaBlinnPhongMaterial*)material;//把material强制转换为phongMaterial，否则无法调用phongMaterial自己的参数
			//diffuse贴图
			//将纹理与纹理单元进行挂钩
			gammaBlinnPhongMaterial->mDiffuse->bind();
			shader->setInt("floorTexture", gammaBlinnPhongMaterial->mDiffuse->getUnit());

			int gamma = (int)glfwGetTime() % 2;
			shader->setMat4("view", camera->getViewMat());
			shader->setMat4("projection", camera->projectionMat());
			shader->setV3F("viewPos", dirLight->mDirection);
			shader->setInt("gamma", gamma);
			//pointLight的更新
			for (int i = 0;i < pointLights.size();i++) {
				auto pointLight = pointLights[i];
				std::string baseName = "lightPositions[0]";
				std::string baseName2 = "lightColors[0]";
				baseName[15] += i;
				baseName2[12] += i;
				shader->setV3F(baseName , pointLight->getPosition());
				shader->setV3F(baseName2, pointLight->mColor);
			}
			//相机信息
			shader->setV3F("viewPos", camera->mPosition);
		}
										break;
		case MaterialType::ShadowMappingDepthMaterial: {
			shader->setMat4("model", mesh->getModelMat());
			//shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		}
										break;
		case MaterialType::ShadowMappingMaterial: {
			ShadowMappingMaterial* shadowMappingMaterial = (ShadowMappingMaterial*)material;
			shadowMappingMaterial->mDiffuse->bind();
			shader->setInt("diffuseTexture",shadowMappingMaterial->mDiffuse->getUnit());
			assert(shadowMappingMaterial->mDiffuse->getUnit()!= shadowMappingMaterial->mShadowMap->getUnit());
			shadowMappingMaterial->mShadowMap->bind();
			shader->setInt("shadowMap", shadowMappingMaterial->mShadowMap->getUnit());

			shader->setMat4("projection",camera->projectionMat());
			shader->setMat4("view", camera->getViewMat());
			shader->setMat4("model", mesh->getModelMat());
			//shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
			//shader->setV3F("lightPos", lightPos);
			shader->setV3F("viewPos", camera->mPosition);

		}
										break;
		case MaterialType::PointShadowDepthMaterial: {
			shader->setMat4("model", mesh->getModelMat());
			std::string baseName = "shadowMatrices[0]";
			for (int i = 0;i < 6;i++) {
				baseName[15] += i;
				//shader->setMat4(baseName,shadowTransforms[i]);
			}
			shader->setV3F("lightPos",pointLights[0]->getPosition());
			//shader->setV1F("far_plane", far);
		}
										break;
		case MaterialType::PointShadowMaterial: {
			PointShadowMaterial* pointShadowMaterial = (PointShadowMaterial*)material;
			pointShadowMaterial->mDiffuse->bind();
			shader->setInt("diffuseTexture",pointShadowMaterial->mDiffuse->getUnit());
			pointShadowMaterial->mShadowMap->bind();
			shader->setInt("depthMap", pointShadowMaterial->mShadowMap->getUnit());
			shader->setMat4("projection", camera->projectionMat());
			shader->setMat4("view", camera->getViewMat());
			shader->setMat4("model", mesh->getModelMat());
			shader->setV3F("lightPos", pointLights[0]->getPosition());
			shader->setV3F("viewPos", camera->mPosition);
			shader->setInt("shadows", 1);
			shader->setInt("reverse_normals",pointShadowMaterial->mReverse_normals);
			//shader->setV1F("far_plane", far);

		}
										break;
		case MaterialType::NormalMappingMaterial: {
			NormalMappingMaterial* normalMappingMaterial = (NormalMappingMaterial*)material;
			normalMappingMaterial->mDiffuse->bind();
			shader->setInt("diffuseMap", normalMappingMaterial->mDiffuse->getUnit());
			normalMappingMaterial->mNormalMap->bind();
			shader->setInt("normalMap", normalMappingMaterial->mNormalMap->getUnit());
			shader->setMat4("model",mesh->getModelMat());
			shader->setMat4("view",camera->getViewMat());
			shader->setMat4("projection", camera->projectionMat());
			shader->setV3F("lightPos",pointLights[0]->getPosition());
			shader->setV3F("viewPos", camera->mPosition);
		}
										break;
		case MaterialType::ParallaxMappingMaterial: {
			ParallaxMappingMaterial* parallaxMappingMaterial = (ParallaxMappingMaterial*)material;
			parallaxMappingMaterial->mDiffuse->bind();
			shader->setInt("diffuseMap", parallaxMappingMaterial->mDiffuse->getUnit());
			parallaxMappingMaterial->mNormalMap->bind();
			shader->setInt("normalMap", parallaxMappingMaterial->mNormalMap->getUnit());
			parallaxMappingMaterial->mDepthMap->bind();
			shader->setInt("depthMap", parallaxMappingMaterial->mDepthMap->getUnit());
			shader->setMat4("model", mesh->getModelMat());
			shader->setMat4("view", camera->getViewMat());
			shader->setMat4("projection", camera->projectionMat());
			shader->setV3F("lightPos", pointLights[0]->getPosition());
			shader->setV3F("viewPos", camera->mPosition);
			shader->setV1F("height_scale", 0.1f);
			shader->setInt("parallax", 1);
		}
										break;
		case MaterialType::PBRMaterial: {
			PBRMaterial* pBRMaterial = (PBRMaterial*)material;
			
			pBRMaterial->mAlbedoMap->bind();
			
			shader->setInt("albedoMap", pBRMaterial->mAlbedoMap->getUnit());
			pBRMaterial->mNormalMap->bind();
			shader->setInt("normalMap", pBRMaterial->mNormalMap->getUnit());
			pBRMaterial->mMetallicMap->bind();
			shader->setInt("metallicMap", pBRMaterial->mMetallicMap->getUnit());
			pBRMaterial->mRoughnessMap->bind();
			shader->setInt("roughnessMap", pBRMaterial->mRoughnessMap->getUnit());
			pBRMaterial->mAoMap->bind();
			shader->setInt("aoMap", pBRMaterial->mAoMap->getUnit());


			pBRMaterial->mIrradianceConvolutionMap->bind();
			shader->setInt("irradianceMap", pBRMaterial->mIrradianceConvolutionMap->getUnit());			
			pBRMaterial->mPrefilterMap->bind();
			shader->setInt("prefilterMap", pBRMaterial->mPrefilterMap->getUnit());
			pBRMaterial->mBrdfLUTMap->bind();
			shader->setInt("brdfLUTMap", pBRMaterial->mBrdfLUTMap->getUnit());

			
			/*
			shader->setV1F("metallic",pBRMaterial->mMetallic);
			shader->setV1F("roughness",pBRMaterial->mRoughness);
			shader->setV3F("albedo", 0.5f, 0.0f, 0.0f);
			shader->setV1F("ao", 1.0f);
			*/

			shader->setMat4("modelMat",mesh->getModelMat());
			shader->setMat4("normalMat", glm::transpose(glm::inverse(mesh->mModelMat)));
			shader->setMat4("viewMat", camera->getViewMat());
			shader->setMat4("projectionMat", camera->projectionMat());

			//pointLight的更新
			for (int i = 0;i < pointLights.size();i++) {
				auto pointLight = pointLights[i];
				std::string baseName = "pointLights[0]";
				baseName[12] += i;
				shader->setV3F(baseName + ".position", pointLight->getPosition());
				shader->setV3F(baseName + ".color", pointLight->mColor);
			}
			shader->setV3F("cameraPos", camera->mPosition);
		}
										break;
		case MaterialType::CameraMaterial: {
			shader->setMat4("modelMat", mesh->getModelMat()* mesh->mModelMat);
			//shader->setMat4("modelMat", glm::mat4(1.0f));
			shader->setMat4("viewMat", camera->getViewMat());
			shader->setMat4("projectionMat", camera->projectionMat());
		}
										break;
		case MaterialType::IrradianceConvolutionMaterial: {
			IrradianceConvolutionMaterial* irradianceConvolutionMaterial = (IrradianceConvolutionMaterial*)material;
			irradianceConvolutionMaterial->mEnvMap->bind();
			shader->setInt("envMap", irradianceConvolutionMaterial->mEnvMap->getUnit());

			mesh->setPosition(camera->mPosition);
			//shader->setMat4("modelMat", mesh->getModelMat());
			shader->setMat4("modelMat", mesh->getModelMat());
			shader->setMat4("viewMat", camera->getViewMat());
			shader->setMat4("projectionMat", camera->projectionMat());
		}
										 break;
		case MaterialType::PrefilterMaterial: {
			PrefilterMaterial* prefilterMaterial = (PrefilterMaterial*)material;
			prefilterMaterial->mEnvMap->bind();
			shader->setInt("envMap", prefilterMaterial->mEnvMap->getUnit());
			shader->setV1F("roughness", prefilterMaterial->mRoughness);
			mesh->setPosition(camera->mPosition);
			//shader->setMat4("modelMat", mesh->getModelMat());
			shader->setMat4("modelMat", mesh->getModelMat());
			shader->setMat4("viewMat", camera->getViewMat());
			shader->setMat4("projectionMat", camera->projectionMat());
		}
										break;
		case MaterialType::BrdfMaterial: {
	
		}
										break;
		default:
			break;
		}
		glBindVertexArray(geometry->getVao());

		if (object->getType() == ObjectType::Mesh) {
			glDrawElements(geometry->getType(), geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
		}
		else {
			InstancedMesh* instancedMesh = (InstancedMesh*)mesh;
			glDrawElementsInstanced(geometry->getType(), geometry->getIndicesCount(), GL_UNSIGNED_INT, 0, instancedMesh->mInstancedCount);
		}
	}
	
}

void Renderer::setClearColor(glm::vec3 color)
{
	glClearColor(color.r,color.g,color.b,1.0f);
}



Shader* Renderer::pickShader(MaterialType type)
{
	Shader* result=nullptr;
	switch (type)
	{
	case MaterialType::PhongMaterial:
		result = mPhongShader;
		break;
	case MaterialType::WhiteMaterial:
		result = mWhiteShader;
		break;
	case MaterialType::DepthMaterial:
		result = mDepthShader;
		break;
	case MaterialType::OpacityMaskMaterial:
		result = mOpacityMaskShader;
		break;
	case MaterialType::ScreenMaterial:
		result = mScreenShader;
		break;
	case MaterialType::CubeMaterial:
		result = mCubeShader;
		break;
	case MaterialType::PhongEnvMaterial:
		result = mPhongEnvShader;
		break;
	case MaterialType::PhongInstancedMaterial:
		result = mPhongInstancedShader;
		break;
	case MaterialType::GrassInstancedMaterial:
		result = mGrassInstancedShader;
		break;
	case MaterialType::BlinnPhongMaterial:
		result = mBlinnPhongShader;
		break;
	case MaterialType::GammaBlinnPhongMaterial:
		result = mGammaBlinnPhongShader;
		break;
	case MaterialType::ShadowMappingDepthMaterial:
		result = mShadowMappingDepthShader;
		break;
	case MaterialType::ShadowMappingMaterial:
		result = mShadowMappingShader;
		break;
	case MaterialType::PointShadowDepthMaterial:
		result = mPointShadowDepthMaterialShader;
		break;
	case MaterialType::PointShadowMaterial:
		result = mPointShadowMaterialShader;
		break;
	case MaterialType::NormalMappingMaterial:
		result = mNormalMappingMaterialShader;
		break;
	case MaterialType::ParallaxMappingMaterial:
		result = mParallaxMappingMaterialShader;
		break;
	case MaterialType::PBRMaterial:
		result = mPBRMaterialShader;
		break;
	case MaterialType::CameraMaterial:
		result = mCameraMaterialShader;
		break;
	case MaterialType::IrradianceConvolutionMaterial:
		result = mIrradianceConvolutionMaterialShader;
		break;
	case MaterialType::PrefilterMaterial:
		result = mPrefilterMaterialShader;
		break;
	case MaterialType::BrdfMaterial:
		result = mBrdfMaterialShader;
		break;
	default:
		std::cout << "Unkown Matrial Type" << std::endl;
		break;
	}
	return result;
}

void Renderer::setDepthState(Material* material)
{
	if (material->mDepthTest) {
		//设置当前帧绘制的时候，OpenGL必要的状态机参数
		//开启深度检测
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(material->mDepthFunc);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
	//该材质是否更新深度缓冲
	glDepthMask(material->mDepthWrite);
}
void Renderer::setPolygonOffsetState(Material* material)
{
	if (material->mPolygonOffset) {
		glEnable(material->mPolygonOffsetType);
		glPolygonOffset(material->mFactor, material->mUnit);
	}
	else {
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_POLYGON_OFFSET_LINE);
	}
}

void Renderer::setStencilState(Material* material)
{
	if (material->mStencilTest) {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(material->mStencilFunc, material->mStencilRef, material->mStencilFuncMask);
		glStencilMask(material->mStencilMask);
		glStencilOp(material->mStencilSFail, material->mStencilZFail, material->mStencilZPass);

	}
	else {
		glDisable(GL_STENCIL_TEST);
	}
}

void Renderer::setBlendState(Material* material)
{
	if (material->mBlend) {
		glEnable(GL_BLEND);
		glBlendFunc(material->mSFactor,material->mDFactor);
	}
	else {
		glDisable(GL_BLEND);
	}
}

void Renderer::setFaceCallingState(Material* material)
{
	if (material->mFaceCulling) {
		glEnable(GL_CULL_FACE);
		glFrontFace(material->mFrontFace);
		glCullFace(material->mCullFace);
	}
	else {
		glDisable(GL_CULL_FACE);
	}
}

void Renderer::projectObject(Object* obj)
{
	if (obj->getType() == ObjectType::Mesh || obj->getType() == ObjectType::InstancedMesh) {
		Mesh* mesh = (Mesh*)obj;
		auto material = mesh->mMaterial;
		if (material->mBlend == true) {
			mTransparentObjects.push_back(mesh);
		}
		else {
			mOpacityObjects.push_back(mesh);
		}
	}
	for (auto child : obj->getChildren()) {
		projectObject(child);
	}
}




