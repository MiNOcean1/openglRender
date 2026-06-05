#include<iostream>
#include<string>
#include<assert.h>
#include <filesystem>

#include"imgui/imgui.h"
#include"imgui/imgui_impl_glfw.h"
#include"imgui/imgui_impl_opengl3.h"

#include "wrapper/checkError.h"

#include "application/application.h"
#include"application/camera/trackBallCameraControl.h"
#include"application/camera/gameCameraControl.h"
#include"application/camera/perspectiveCamera.h"
#include"application/camera/orthoCamera.h"
#include"application/assimpLoader.h"
#include"application/assimpPBRLoader.h"
#include"application/assimpInstancedLoader.h"
#include"application/plyLoader.h"

#include"glframework/core.h"
#include"glframework/shader.h"
#include"glframework/texture.h"
#include"glframework/geometry.h"

#include"glframework/material/phongMaterial.h"
#include"glframework/material/whiteMaterial.h"
#include"glframework/material/depthMaterial.h"
#include"glframework/material/opacityMaskMaterial.h"
#include"glframework/material/screenMaterial.h"
#include"glframework/material/cubeMaterial.h"
#include"glframework/material/phongInstancedMaterial.h"
#include"glframework/material/grassInstancedMaterial.h"
#include"glframework/material/gammaBlinnPhongMaterial.h"
#include"glframework/material/shadowMappingDepthMaterial.h"
#include"glframework/material/pointShadowDepthMaterial.h"
#include"glframework/material/normalMappingMaterial.h"
#include"glframework/material/parallaxMappingMaterial.h"
#include"glframework/material/pBRMaterial.h"
#include"glframework/material/irradianceConvolutionMaterial.h"

#include"glframework/mesh/mesh.h"
#include"glframework/mesh/instancedMesh.h"
#include"glframework/render/renderer.h"
#include"glframework/light/pointLight.h"
#include"glframework/scene.h"
#include"glframework/frameBuffer/frameBuffer.h"
#include"glframework/tools/tools.h"


/*void doTransform() {
	
	//向量
	glm::vec3 v1(0), v2(0);
	auto vmul = v1 * v2;//对应值相乘
	auto vdot = glm::dot(v1, v2);//点乘
	auto vcross = glm::cross(v1, v2);//叉乘，只能3维向量叉乘

	//矩阵
	glm::mat4 m0(1.0);//初始化单位矩阵
	glm::mat4 m1 = glm::identity<glm::mat4>();//另一种方式初始化单位矩阵
	glm::mat2x3 m2(1.0);
	std::cout << glm::to_string(m2) << std::endl;
	auto transMat = glm::transpose(m0);//矩阵转置
	auto inverseMat = glm::inverse(m0);//逆矩阵
	
	//构建一个旋转矩阵，绕着z轴旋转45度角
	//rotate:用于生成旋转矩阵
	//bug1：旋转角度必须是float类型
	//bug2:接收的是弧度不是角度
	//modelMat = glm::rotate(modelMat,glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));//初始矩阵，旋转角度，绕的轴

	//平移变换,平移变换永远以缩放为基准，例如缩放成0.5倍，平移1.0也是平移0.5
	//model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));//需要像xyz方向平移的大小,坐标系旋转之后是没有变的

	//缩放变换
	//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	//gokuModelMat = glm::rotate(gokuModelMat, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 1.0f));
}*/
/*
//纹理混合实验：
Texture* grassTexture = nullptr;
Texture * landTexture = nullptr;
Texture* noiseTexture = nullptr;
*/
/*void prepareShader() {
	shader = new Shader("assets/shaders/vertex.glsl","assets/shaders/fragment.glsl");
}*/
/*void prepareSingleBuffer() {
	float position[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f
	};
	float color[] = {
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f
	};
	GLuint posvbo = 0, colvbo = 0;

	//位置VBO
	glGenBuffers(1, &posvbo);
	//绑定当前vbo到opengl状态机的当前vbo插槽上面
	//GL_ARRAY_BUFFER:当前vbo这个插槽
	glBindBuffer(GL_ARRAY_BUFFER, posvbo);
	//向当前vbo传输数据，开辟显存
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);//插槽，传入数据个数，需要传入的数据，数据是否频繁修改

	//颜色VBO
	glGenBuffers(1, &colvbo);
	glBindBuffer(GL_ARRAY_BUFFER, colvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	GLuint vao = 0;
	//生成一个vao
	glGenVertexArrays(1, &vao);
	//绑定vao到状态机插槽
	glBindVertexArray(vao);


	//绑定位置属性的vbo到状态机插槽
	glBindBuffer(GL_ARRAY_BUFFER, posvbo);
	//激活vao的0号属性，0号属性存放位置信息
	glEnableVertexAttribArray(0);
	//填充位置vbo信息到0号属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，属性段长，此属性在vbo中每个点内起始位置

	glBindBuffer(GL_ARRAY_BUFFER, colvbo);
	//激活vao的1号属性，1号属性存放颜色信息
	glEnableVertexAttribArray(1);
	//填充位置vbo信息到0号属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，点段长，此属性在vbo中每个点内起始位置
	//完成之后解绑一下
	glBindVertexArray(0);
}*/
/*void prepareVao() {
	box = Geometry::creatBox(3.0f);
	plane = Geometry::creatPlane(4.0f,2.0f);
	sphere = Geometry::creatSphere(3.0f);
}*/
/*void prepareTexture() {
	gokuTexture = new Texture("assets/textures/goku.jpg", 0);//文件路径，要绑定的纹理单元
	//worldTexture = new Texture("assets/textures/world.jpg", 0);
	//grassTexture = new Texture("assets/textures/grass.jpg", 0);
	
	//纹理混合实验：
	grassTexture = new Texture("assets/textures/grass.jpg", 0);
	landTexture = new Texture("assets/textures/land.jpg", 1);
	noiseTexture = new Texture("assets/textures/noise.jpg", 2);
	
	
}*/
/*
void prepareOrtho() {
	orthoMat = glm::ortho(-2.0f,2.0f,-2.0f,2.0f,2.0f,-2.0f);//left: 视锥体的左边界 right: 视锥体的右边界 bottom : 视锥体的下边界 top : 视锥体的上边界 zNear : 视锥体的近剪裁面 zFar : 视锥体的远剪裁面。
}
void preparePerspective() {
	fovy:y轴方向的视张角
	aspect:近平面的横纵百分比
	near:近平面距离
	far:远平面距离
	perspectiveMat = glm::perspective(glm::radians(45.0f),(float)(app->getWidth())/(float)(app->getHeight()), 0.1f, 1000.0f);
}*/
/*void prepareState() {//深度检测
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}*/

/*
//平行光：参数（方向，光强）uniform变量形式
glm::vec3 lightDirection = glm::vec3(-1.0f, -1.0f, -1.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
//高光反射强度
float specularIntensity = 1.0f;
//环境光
glm::vec3 ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
//法线矩阵
glm::vec3 normalMatrix;*/
Renderer* renderer{ nullptr };
Scene* sceneFbo= nullptr;
Scene* scene = nullptr;
FrameBuffer* frameBuffer = nullptr;

int WIDTH = 1024;
int HEIGHT = 1024;
int captureWidthAndHeight = 512;
GrassInstancedMaterial* grassInstancedMaterial = nullptr;
Texture* envTexture = nullptr;
DirectionLight* dirLight{ nullptr };
std::vector<PointLight*> pointLights;
SpotLight* spotLight{ nullptr };
AmbientLight* ambLight{ nullptr };

PerspectiveCamera* camera = nullptr;
GameCameraControl* cameraControl = nullptr;

glm::vec3 clearColor{};
//窗体大小变化的回调函数
void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "onresize" << std::endl;
}

void OnKey(int key, int scancode, int action, int mods) {
	/*
	if (key == GLFW_KEY_W) {}//按住了W
	if (action == GLFW_PRESS) {}//按下
	if (action == GLFW_RELEASE) {}//抬起
	if (mods == GLFW_MOD_CONTROL) {}//按了CTRL加key
	if (mods == GLFW_MOD_SHIFT) {}//按了SHIFT加key
	std::cout << "按下了：" << key << std::endl;
	std::cout << "action：" << action << std::endl;
	std::cout << "mods：" << mods << std::endl;
	*/
	//trackBallCameraControl->onKey(key, action, mods);
	cameraControl->onKey(key, action, mods);
}

void OnMouse(int button, int action, int mods) {
	double x, y;
	glApp->getCursorPos(&x, &y);
	cameraControl->onMouse(button, action, x, y);
}

void OnCursor(double xpos, double ypos) {
	cameraControl->onCursor(xpos, ypos);
}

void OnScroll(double xoffset, double yoffset) {
	cameraControl->onScroll(xoffset, yoffset);
}


void prepareCamera() {
	camera = new PerspectiveCamera(
		65.0f,
		(float)(glApp->getWidth()) / (float)(glApp->getHeight()),
		0.1f,
		1000.0f
	);
	//camera = new OrthoCamera(-3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 113.0f);
	//cameraControl = new TrackBallCameraControl();
	cameraControl = new GameCameraControl();
	cameraControl->setCamera(camera);
	cameraControl->setSensitivty(0.3f);

}


void setModelBlend(Object* obj,bool blend,float opacity) {
	if (obj->getType() == ObjectType::Mesh) {
		Mesh* mesh = (Mesh*)obj;
		Material* material = mesh->mMaterial;
		material->mBlend = blend;
		material->mOpacity = opacity;
		//material->mDepthWrite = false;
	}
	for (auto child : obj->getChildren()) {
		setModelBlend(child,blend,opacity);
		
	}

}
/*
//行星构造
void setModelBlend1(Object* obj, bool blend, float opacity) {
	if (obj->getType() == ObjectType::InstancedMesh) {
		InstancedMesh* instancedMesh = (InstancedMesh*)obj;
		Material* material = instancedMesh->mMaterial;
		material->mBlend = blend;
		material->mOpacity = opacity;
		//material->mDepthWrite = false;

		instancedMesh->mInstancedModelMatrices= modelMatrices;

		instancedMesh->updateMatrices();

	}
	for (auto child : obj->getChildren()) {
		setModelBlend1(child, blend, opacity);
	}
}*/
void setInstancedModelMatrix(Object*  obj,unsigned int index,glm::mat4 instancedModelMatrix) {
	if (obj->getType() == ObjectType::InstancedMesh) {
		InstancedMesh* instancedMesh = (InstancedMesh*)obj;
		instancedMesh->mInstancedModelMatrices[index] = instancedModelMatrix;
	}
	for (auto child : obj->getChildren()) {
		setInstancedModelMatrix(child, index, instancedModelMatrix);
	}
}

void setIrradianceConvolutionMap_PrefilterMap_BrdfLUTMap(Object* obj, Texture* irradianceConvolutionMap, Texture* prefiterMap, Texture* brdfLUTMap) {
	if (obj->getType() == ObjectType::Mesh) {
		Mesh* mesh = (Mesh*)obj;
		PBRMaterial* material = (PBRMaterial*)mesh->mMaterial;
		material->mIrradianceConvolutionMap = irradianceConvolutionMap;
		material->mPrefilterMap = prefiterMap;
		material->mBrdfLUTMap = brdfLUTMap;
		//material->mDepthWrite = false;
	}
	for (auto child : obj->getChildren()) {
		setIrradianceConvolutionMap_PrefilterMap_BrdfLUTMap(child, irradianceConvolutionMap, prefiterMap, brdfLUTMap);

	}
}

void updateVbo(Object* obj) {
	if (obj->getType() == ObjectType::InstancedMesh) {
		InstancedMesh* instancedMesh = (InstancedMesh*)obj;
		instancedMesh->updateMatrices();
	}
	for (auto child : obj->getChildren()) {
		updateVbo(child);
	}
}

void setGrassMaterial(Object* obj,Material* material) {
	if (obj->getType() == ObjectType::InstancedMesh) {
		InstancedMesh* instancedMesh = (InstancedMesh*)obj;
		instancedMesh->mMaterial = material;
	}
	for (auto child : obj->getChildren()) {
		setGrassMaterial(child, material);
	}
}
Mesh* mesh = nullptr;
void prepare() {
	renderer = new Renderer();
	scene = new Scene();
	sceneFbo = new Scene();
	//frameBuffer = new FrameBuffer(WIDTH, HEIGHT,1);


	/*
	//草地构建
	auto grassModel = AssimpLoader::load("assets/fbx/grass.obj");
	//grassModel->setScale(glm::vec3(0.02f));
	scene->addChild(grassModel);

	auto grassMaterial = new OpacityMaskMaterial();
	grassMaterial->mDiffuse = new Texture("assets/textures/grass.jpg", 0);
	grassMaterial->mOpacityMask=new Texture("assets/textures/grassMask.png", 1);
	grassMaterial->mBlend = true;
	grassMaterial->mDepthWrite = false;

	renderer->mGlobalMaterial = grassMaterial;
	*/
	/*
	//模板测试
	material->mBlend = true;
	material->mDepthWrite = false;
	material->mOpacity = 0.3f;
	//material->mSpecular = new Texture("assets/textures/container_specular.jpg",1);


	material->mStencilTest = true;
	material->mStencilFunc = GL_ALWAYS;
	material->mStencilRef = 1;
	material->mStencilFuncMask = 0xFF;
	material->mStencilMask = 0xFF;
	material->mStencilSFail = GL_KEEP;
	material->mStencilZFail = GL_KEEP;
	material->mStencilZPass = GL_REPLACE;
	*/
	/*
	//帧缓冲
	//贴到屏幕上的矩形
	auto geometry = Geometry::creatScreenPlane();
	auto material = new ScreenMaterial ();
	material->mScreenTexture = frameBuffer->mColorAttachment;
	auto mesh = new Mesh(geometry, material);
	sceneInScreen->addChild(mesh);
	*/
	/*
	//点光源创建
	auto pointLight0=new PointLight();
	pointLight0->mColor = glm::vec3(1.0f,0.0f,0.0f);
	pointLight0->setPosition(glm::vec3(0.0f,0.0f,1.0f));
	pointLight0->mK0 = 1.0f;
	pointLight0->mK1 = 0.07f;
	pointLight0->mK2 = 0.017f;
	pointLights.push_back(pointLight0);

	auto pointLight1 = new PointLight();
	pointLight1->mColor = glm::vec3(0.0f, 1.0f, 0.0f);
	pointLight1->setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	pointLight1->mK0 = 1.0f;
	pointLight1->mK1 = 0.07f;
	pointLight1->mK2 = 0.017f;
	pointLights.push_back(pointLight1);

	auto pointLight2 = new PointLight();
	pointLight2->mColor = glm::vec3(0.0f, 0.0f, 1.0f);
	pointLight2->setPosition(glm::vec3(0.0f, 0.0f, -1.0f));
	pointLight2->mK0 = 1.0f;
	pointLight2->mK1 = 0.07f;
	pointLight2->mK2 = 0.017f;
	pointLights.push_back(pointLight2);

	auto pointLight3 = new PointLight();
	pointLight3->mColor = glm::vec3(1.0f, 1.0f, 0.0f);
	pointLight3->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	pointLight3->mK0 = 1.0f;
	pointLight3->mK1 = 0.07f;
	pointLight3->mK2 = 0.017f;
	pointLights.push_back(pointLight3);
*/
/*
//创建背包
auto backpack = AssimpLoader::load("assets/fbx/backpack/backpack.obj");
setModelBlend(backpack,true,0.2f);
scene->addChild(backpack);
*/
/*
//立方体贴图
//必须按照右左上下后前的顺序
std::vector<const char*>paths = {
	"assets/textures/skybox/right.jpg",
	"assets/textures/skybox/left.jpg",
	"assets/textures/skybox/top.jpg",
	"assets/textures/skybox/bottom.jpg",
	"assets/textures/skybox/back.jpg",
	"assets/textures/skybox/front.jpg",
};
auto boxGeometry = Geometry::creatBox(1.0f);
//创建一个material并且配置参数
auto boxMaterial = new CubeMaterial();
boxMaterial->mDiffuse =new Texture(paths, 2);
boxMaterial->mDepthWrite = false;
//生成mesh
Mesh* boxMesh = new Mesh(boxGeometry, boxMaterial);
scene->addChild(boxMesh);
*/
/*
//行星构造
unsigned int amount = 100000;
glm::mat4* modelMatrices;
unsigned int buffer;
modelMatrices = new glm::mat4[amount];
srand(glfwGetTime()); // 初始化随机种子
float radius = 40.0;
float offset = 25.0f;
for (unsigned int i = 0; i < amount; i++)
{
	glm::mat4 model=glm::mat4(1.0f);
	// 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
	float angle = (float)i / (float)amount * 360.0f;
	float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
	float x = sin(angle) * radius + displacement;
	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
	float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
	float z = cos(angle) * radius + displacement;
	model = glm::translate(model, glm::vec3(x, y, z));

	// 2. 缩放：在 0.05 和 0.25f 之间缩放
	float scale = (rand() % 20) / 100.0f + 0.05;
	model = glm::scale(model, glm::vec3(scale));

	// 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
	float rotAngle = (rand() % 360);
	model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

	// 4. 添加到矩阵的数组中
	modelMatrices[i] = model;
}

//创建行星
auto planet = AssimpLoader::load("assets/fbx/planet/planet.obj");
setModelBlend(planet, false, 1.0f);
//planet->setScale(glm::vec3(0.3f));
scene->addChild(planet);


//创建岩石
auto rock = AssimpInstancedLoader::load("assets/fbx/rock/rock.obj",amount);
setModelBlend1(rock, false,1.0f);
//rock->setScale(glm::vec3(0.3f));

InstancedMesh* instancedRock = (InstancedMesh*)rock;
instancedRock->mInstancedCount = amount;
instancedRock->mInstancedModelMatrices = modelMatrices;
scene->addChild(rock);
*/
/*草地结业作品
//球体贴图
//创建一个立方体贴图
//创建一个geometry
auto boxGeometry = Geometry::creatBox(1.0f);
//创建一个material并且配置参数
auto boxMaterial = new CubeMaterial();
boxMaterial->mDiffuse = new Texture("assets/textures/bk.jpg",0);
//生成mesh
Mesh* boxMesh = new Mesh(boxGeometry, boxMaterial);
scene->addChild(boxMesh);

unsigned int rNum = 20;
unsigned int cNum = 20;

auto grass = AssimpInstancedLoader::load("assets/fbx/grass.obj", rNum * cNum);
glm::mat4 translate;
glm::mat4 rotate;
glm::mat4 transform;
srand(glfwGetTime());
for (int i = 0;i < rNum;i++) {
	for (int j = 0;j < cNum;j++) {
		translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f * i, 0.0f, 0.2f * j));
		rotate = glm::rotate(glm::radians((float)(rand() % 90)), glm::vec3(0.0f,1.0f,0.0f));
		transform = translate * rotate;
		setInstancedModelMatrix(grass, i * rNum + j, transform);
	}
}
updateVbo(grass);
grassInstancedMaterial = new GrassInstancedMaterial();
grassInstancedMaterial->mDiffuse = new Texture("assets/textures/GRASS.png",0);
grassInstancedMaterial->mOpacityMask = new Texture("assets/textures/grassMask.png",1);
grassInstancedMaterial->mCloudMask = new Texture("assets/textures/CLOUD.png", 2);


grassInstancedMaterial->mUVScale = 2.0f;
setGrassMaterial(grass, grassInstancedMaterial);
grassInstancedMaterial->mBlend = true;
grassInstancedMaterial->mDepthWrite = false;
scene->addChild(grass);

auto house = AssimpLoader::load("assets/fbx/house.fbx");
house->setScale(glm::vec3(0.5f));
house->setPosition(glm::vec3(rNum * 0.2f / 2.0f, 0.4f, cNum * 0.2f / 2.0f));
scene->addChild(house);
*/
/*阴影映射
//地板
std::vector<float> positon = {
	25.0f, -0.5f,  25.0f,
	-25.0f, -0.5f,  25.0f,
	-25.0f, -0.5f, -25.0f,
	25.0f, -0.5f,  -25.0f,
};
std::vector<float> normal = {
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};
std::vector<float> uvs = {
	25.0f,  0.0f,
	0.0f,  0.0f,
	0.0f, 25.0f,
	25.0f,  25.0f,
};
std::vector< unsigned int>indices = {
	0,1,2,
	0,2,3
};

Texture* texture = new Texture("assets/textures/wood.png",0);
Geometry* geometryFloor = new Geometry(positon,normal,uvs,indices);
ShadowMappingDepthMaterial* shadowMappingDepthMaterial = new ShadowMappingDepthMaterial();
Mesh* meshFloorDepth = new Mesh(geometryFloor, shadowMappingDepthMaterial);
fboScene->addChild(meshFloorDepth);
ShadowMappingMaterial * shadowMappingMaterial = new ShadowMappingMaterial();
shadowMappingMaterial->mDiffuse = texture;
shadowMappingMaterial->mShadowMap = frameBuffer->mDepthStencilAttachment;
std::cout << frameBuffer->mDepthStencilAttachment->getUnit() << std::endl;
Mesh* meshFloor = new Mesh(geometryFloor, shadowMappingMaterial);
scene->addChild(meshFloor);

//立方体
Geometry* geometryCube0 = Geometry::creatBox(2.0f);
Mesh* meshCube0Depth = new Mesh(geometryCube0, shadowMappingDepthMaterial);
meshCube0Depth->setPosition(glm::vec3(0.0f, 1.5f, 0.0));
meshCube0Depth->setScale(glm::vec3(0.5f));
fboScene->addChild(meshCube0Depth);
Mesh* meshCube0 = new Mesh(geometryCube0, shadowMappingMaterial);
meshCube0->setPosition(glm::vec3(0.0f, 1.5f, 0.0));
meshCube0->setScale(glm::vec3(0.5f));
scene->addChild(meshCube0);

Geometry* geometryCube1 = Geometry::creatBox(2.0f);
Mesh* meshCube1Depth = new Mesh(geometryCube1, shadowMappingDepthMaterial);
meshCube1Depth->setPosition(glm::vec3(2.0f, 0.0f, 1.0));
meshCube1Depth->setScale(glm::vec3(0.5f));
fboScene->addChild(meshCube1Depth);
Mesh* meshCube1 = new Mesh(geometryCube1, shadowMappingMaterial);
meshCube1->setPosition(glm::vec3(2.0f, 0.0f, 1.0));
meshCube1->setScale(glm::vec3(0.5f));
scene->addChild(meshCube1);

Geometry* geometryCube2 = Geometry::creatBox(2.0f);
Mesh* meshCube2Depth = new Mesh(geometryCube2, shadowMappingDepthMaterial);
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
model = glm::scale(model, glm::vec3(0.25));
glm::vec3 position, eulerAngle, scale;
Tools::decompose(model,position,eulerAngle,scale);
meshCube2Depth->setPosition(position);
meshCube2Depth->setAngle(eulerAngle);
meshCube2Depth->setScale(scale);
fboScene->addChild(meshCube2Depth);
Mesh* meshCube2 = new Mesh(geometryCube2, shadowMappingMaterial);
meshCube2->setPosition(position);
meshCube2->setAngle(eulerAngle);
meshCube2->setScale(scale);
scene->addChild(meshCube2);
*/
/*点阴影映射
Texture* texture = new Texture("assets/textures/wood.png", 0);
PointShadowDepthMaterial* pointShadowDepthMaterial = new PointShadowDepthMaterial();
//立方体
Geometry* geometryCube0 = Geometry::creatBox(1.0f);
Mesh* meshCube0Depth = new Mesh(geometryCube0, pointShadowDepthMaterial);
meshCube0Depth->setScale(glm::vec3(20.0f));
fboScene->addChild(meshCube0Depth);

PointShadowMaterial* pointShadowMaterial0 = new PointShadowMaterial();
pointShadowMaterial0->mReverse_normals = 1;
pointShadowMaterial0->mDiffuse = texture;
pointShadowMaterial0->mShadowMap = frameBuffer->mDepthStencilAttachment;
Mesh* meshCube0 = new Mesh(geometryCube0, pointShadowMaterial0);
meshCube0->setScale(glm::vec3(10.0f));
scene->addChild(meshCube0);

Geometry* geometryCube1 = Geometry::creatBox(1.0f);
Mesh* meshCube1Depth = new Mesh(geometryCube1, pointShadowDepthMaterial);
meshCube1Depth->setPosition(glm::vec3(4.0f, -3.5f, 0.0));
fboScene->addChild(meshCube1Depth);

PointShadowMaterial* pointShadowMaterial = new PointShadowMaterial();
pointShadowMaterial->mReverse_normals = 0;
pointShadowMaterial->mDiffuse = texture;
pointShadowMaterial->mShadowMap = frameBuffer->mDepthStencilAttachment;
Mesh* meshCube1 = new Mesh(geometryCube1, pointShadowMaterial);
meshCube1->setPosition(glm::vec3(4.0f, -3.5f, 0.0));
scene->addChild(meshCube1);

Geometry* geometryCube2 = Geometry::creatBox(1.0f);
Mesh* meshCube2Depth = new Mesh(geometryCube2, pointShadowDepthMaterial);
meshCube2Depth->setPosition(glm::vec3(2.0f, 3.0f, 1.0));
meshCube2Depth->setScale(glm::vec3(1.5f));
fboScene->addChild(meshCube2Depth);

Mesh* meshCube2 = new Mesh(geometryCube2, pointShadowMaterial);
meshCube2->setPosition(glm::vec3(2.0f, 3.0f, 1.0));
meshCube2->setScale(glm::vec3(1.5f));
scene->addChild(meshCube2);

Geometry* geometryCube3 = Geometry::creatBox(1.0f);
Mesh* meshCube3Depth = new Mesh(geometryCube3, pointShadowDepthMaterial);
meshCube3Depth->setPosition(glm::vec3(-3.0f, -1.0f, 0.0));
fboScene->addChild(meshCube3Depth);

Mesh* meshCube3 = new Mesh(geometryCube3, pointShadowMaterial);
meshCube3->setPosition(glm::vec3(-3.0f, -1.0f, 0.0));
scene->addChild(meshCube3);

Geometry* geometryCube4 = Geometry::creatBox(1.0f);
Mesh* meshCube4Depth = new Mesh(geometryCube4, pointShadowDepthMaterial);
meshCube4Depth->setPosition(glm::vec3(-1.5f, 1.0f, 1.5));
fboScene->addChild(meshCube4Depth);

Mesh* meshCube4 = new Mesh(geometryCube4, pointShadowMaterial);
meshCube4->setPosition(glm::vec3(-1.5f, 1.0f, 1.5));
scene->addChild(meshCube4);

Geometry* geometryCube5 = Geometry::creatBox(2.0f);
Mesh* meshCube5Depth = new Mesh(geometryCube5, pointShadowDepthMaterial);
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
model = glm::scale(model, glm::vec3(1.5f));
glm::vec3 position, eulerAngle, scale;
Tools::decompose(model, position, eulerAngle, scale);
meshCube5Depth->setPosition(position);
meshCube5Depth->setAngle(eulerAngle);
meshCube5Depth->setScale(scale);
fboScene->addChild(meshCube5Depth);

Mesh* meshCube5 = new Mesh(geometryCube5, pointShadowMaterial);
meshCube5->setPosition(position);
meshCube5->setAngle(eulerAngle);
meshCube5->setScale(scale);
scene->addChild(meshCube5);

auto geometry = Geometry::creatScreenPlane();
auto material = new ScreenMaterial();
material->mScreenTexture = frameBuffer->mDepthStencilAttachment;
auto mesh = new Mesh(geometry, material);
sceneScreen->addChild(mesh);

//点光源创建
auto pointLight0 = new PointLight();
pointLight0->mColor = glm::vec3(1.0f, 0.0f, 0.0f);
pointLight0->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
pointLights.push_back(pointLight0);
*/
/*
//视差贴图
Geometry* planeTBN = Geometry::creatPlaneWithTBN();
ParallaxMappingMaterial* parallaxMappingMaterial = new ParallaxMappingMaterial();
Texture* diffuse = new Texture("assets/textures/toy_box_diffuse.png", 0);
Texture* normalMap = new Texture("assets/textures/toy_box_normal.png", 1);
Texture* depthMap = new Texture("assets/textures/toy_box_disp.png", 2);
parallaxMappingMaterial->mDiffuse = diffuse;
parallaxMappingMaterial->mNormalMap = normalMap;
parallaxMappingMaterial->mDepthMap = depthMap;
Mesh* mesh = new Mesh(planeTBN, parallaxMappingMaterial);
scene->addChild(mesh);

auto pointLight0 = new PointLight();
pointLight0->setPosition(glm::vec3(0.5f, 1.0f, 0.3f));
pointLights.push_back(pointLight0);
*/
//PBR

	Geometry* cube = Geometry::creatBox(2.0f);

	//利用帧缓冲把hdr图片渲染到立方体贴图上面
	frameBuffer = FrameBuffer::creatCaptureFBO(captureWidthAndHeight);
	Texture* envCubeMap = Texture::createEnvCube(captureWidthAndHeight, 1, false);//存在帧缓冲里面需要被渲染的立方体贴图
	Texture* hdrTexture = new Texture("assets/textures/HDR/newport_loft.hdr", 0, GL_FLOAT);//导入的hdr图片
	CubeMaterial* cubeMaterial = new CubeMaterial();
	cubeMaterial->mDiffuse = hdrTexture;
	Mesh* cubeMesh = new Mesh(cube, cubeMaterial);
	sceneFbo->addChild(cubeMesh);

	glm::vec3 captureUps[] =
	{
		glm::vec3(0.0f, -1.0f,  0.0f),
		glm::vec3(0.0f, -1.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  1.0f),
		glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(0.0f, -1.0f,  0.0f),
		glm::vec3(0.0f, -1.0f,  0.0f)
	};
	glm::vec3 captureRights[] = {
		// 对应captureViews[0]：看向正X，up为(0,-1,0)
		glm::vec3(0.0f, 0.0f, -1.0f),
		// 对应captureViews[1]：看向负X，up为(0,-1,0)
		glm::vec3(0.0f, 0.0f, 1.0f),
		// 对应captureViews[2]：看向正Y，up为(0,0,1)
		glm::vec3(1.0f, 0.0f, 0.0f),
		// 对应captureViews[3]：看向负Y，up为(0,0,-1)
		glm::vec3(1.0f, 0.0f, 0.0f),
		// 对应captureViews[4]：看向正Z，up为(0,-1,0)
		glm::vec3(1.0f, 0.0f, 0.0f),
		// 对应captureViews[5]：看向负Z，up为(0,-1,0)
		glm::vec3(-1.0f, 0.0f, 0.0f)
	};
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->mFBO);
	glViewport(0, 0, captureWidthAndHeight, captureWidthAndHeight);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubeMap->getTexture(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer->setClearColor(clearColor);
		camera->mPosition = glm::vec3(0);
		camera->mUp = captureUps[i];
		camera->mRight = captureRights[i];
		camera->mFovy = 90.0f;
		camera->mAspect = 1.0f;
		camera->mNear = 0.1f;
		camera->mFar = 10.0f;
		renderer->render(sceneFbo, camera, dirLight, pointLights, ambLight, frameBuffer->mFBO);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	//将环境贴图卷积
	sceneFbo = new Scene();
	frameBuffer = FrameBuffer::creatCaptureFBO(32);
	Texture* irradianceConvolutionMap = Texture::createEnvCube(32, 5, false);//需要被渲染成的卷积图
	IrradianceConvolutionMaterial* irradianceConvolution = new IrradianceConvolutionMaterial();
	irradianceConvolution->mEnvMap = envCubeMap;
	Mesh* irradianceConvolutionMesh = new Mesh(cube, irradianceConvolution);
	sceneFbo->addChild(irradianceConvolutionMesh);



	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->mFBO);
	glViewport(0, 0, 32, 32);

	for (unsigned int i = 0; i < 6; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceConvolutionMap->getTexture(), 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer->setClearColor(clearColor);
		camera->mPosition = glm::vec3(0);
		camera->mUp = captureUps[i];
		camera->mRight = captureRights[i];
		camera->mFovy = 90.0f;
		camera->mAspect = 1.0f;
		camera->mNear = 0.1f;
		camera->mFar = 10.0f;
		renderer->render(sceneFbo, camera, dirLight, pointLights, ambLight, frameBuffer->mFBO);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//捕获预过滤 mipmap 级别
	sceneFbo = new Scene();
	PrefilterMaterial* prefilterMaterial = new PrefilterMaterial();
	Texture* prefilterMap = Texture::createEnvCube(128, 6, true);
	prefilterMaterial->mEnvMap = envCubeMap;
	Mesh* prefilterMesh = new Mesh(cube, prefilterMaterial);
	sceneFbo->addChild(prefilterMesh);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->mFBO);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
		unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
		glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer->mRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		prefilterMaterial->mRoughness = roughness;
		//prefilterShader.setFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			//prefilterShader.setMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap->getTexture(), mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderer->setClearColor(clearColor);
			camera->mPosition = glm::vec3(0);
			camera->mUp = captureUps[i];
			camera->mRight = captureRights[i];
			camera->mFovy = 90.0f;
			camera->mAspect = 1.0f;
			camera->mNear = 0.1f;
			camera->mFar = 10.0f;
			renderer->render(sceneFbo, camera, dirLight, pointLights, ambLight, frameBuffer->mFBO);

			//renderCube();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//预计算 BRDF
	Texture* brdfLUTMap = Texture::createBrdfLUT(512, 512, 7);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->mFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer->mRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTMap->getTexture(), 0);

	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Geometry* brdfLUT = Geometry::creatPlane(2.0f, 2.0f);
	BrdfMaterial* brdfMaterial = new BrdfMaterial();
	sceneFbo = new Scene();
	Mesh* brdfLUTMesh = new Mesh(brdfLUT, brdfMaterial);
	sceneFbo->addChild(brdfLUTMesh);
	renderer->render(sceneFbo, camera, dirLight, pointLights, ambLight, frameBuffer->mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




	//环境的立方体贴图
	PhongEnvMaterial* phongEnvMaterial = new PhongEnvMaterial();
	phongEnvMaterial->mEnv = envCubeMap;
	Mesh* envMesh = new Mesh(cube, phongEnvMaterial);
	scene->addChild(envMesh);

	//auto chair = AssimpPBRLoader::load("assets/fbx/chair.fbx");
	//setModelBlend(chair, false, 1.0f);
	//setIrradianceConvolutionMap(chair,irradianceConvolutionMap);
	//chair->setScale(glm::vec3(0.1f));
	//chair->rotateX(180);
	//scene->addChild(chair);

	/*
	auto wall = AssimpPBRLoader::load("assets/fbx/StarSparrow/StarSparrow.fbx");
	wall->setScale(glm::vec3(0.01f));
	setIrradianceConvolutionMap_PrefilterMap_BrdfLUTMap(wall, irradianceConvolutionMap, prefilterMap, brdfLUTMap);
	scene->addChild(wall);
	*/

	//导入飞机模型
	auto wall = AssimpPBRLoader::load("assets/fbx/StarSparrow/StarSparrow.fbx");
	wall->setScale(glm::vec3(0.01f));
	setIrradianceConvolutionMap_PrefilterMap_BrdfLUTMap(wall, irradianceConvolutionMap, prefilterMap, brdfLUTMap);
	scene->addChild(wall);

	Geometry* plane = Geometry::creatPlane(32.0f,32.0f);
	PhongMaterial* planeMaterial = new PhongMaterial();
	planeMaterial->mDiffuse= new Texture("assets/textures/wood.png", 0);
	Mesh* planMesh = new Mesh(plane, planeMaterial);
	planMesh->rotateX(90);
	//scene->addChild(planMesh);
	

	camera->mPosition = glm::vec3(0.0f,0.0f,5.0f);
	camera->mFovy = 65.0f;
	camera->mAspect = (float)(glApp->getWidth()) / (float)(glApp->getHeight());
	camera->mNear = 0.1f;
	camera->mFar = 100.0f;
	camera->mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camera->mRight = glm::vec3(1.0f, 0.0f, 0.0f);




	/*
	//pbr
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;
	unsigned int amount = 49;
	
	
	//实例绘制
	PBRMaterial* pBRMaterial = new PBRMaterial();
	Geometry* geometry = Geometry::creatSphere(1.0f);  // 共享几何（球体）
	InstancedMesh* instancedMesh = new InstancedMesh(geometry, pBRMaterial, amount);  // 49个Mesh
	
	Texture* albedoMap = new Texture("assets/textures/pbr/rusted_iron/albedo.png", 2);
	Texture* normalMap = new Texture("assets/textures/pbr/rusted_iron/normal.png", 3);
	Texture* metallicMap = new Texture("assets/textures/pbr/rusted_iron/metallic.png", 4);
	Texture* roughnessMap = new Texture("assets/textures/pbr/rusted_iron/roughness.png", 5);
	Texture* aoMap = new Texture("assets/textures/pbr/rusted_iron/ao.png", 6);
	pBRMaterial->mAlbedoMap = albedoMap;
	pBRMaterial->mNormalMap = normalMap;
	pBRMaterial->mMetallicMap = metallicMap;
	pBRMaterial->mRoughnessMap = roughnessMap;
	pBRMaterial->mAoMap = aoMap;
	pBRMaterial->mIrradianceConvolutionMap = irradianceMap;

	glm::mat4 model = glm::mat4(1.0f);
	for (int row = 0; row < nrRows; ++row)
	{

		for (int col = 0; col < nrColumns; ++col)
		{

			// we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off on direct lighting.
			int id = row * nrColumns + col;
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(
				(col - (nrColumns / 2)) * spacing,
				(row - (nrRows / 2)) * spacing,
				0.0f
			));
			instancedMesh->mInstancedModelMatrices[id] = model;
			//meshes[id]->setPosition(glm::vec3((col - (nrColumns / 2)) * spacing, (row - (nrRows / 2)) * spacing, 0.0f));

		}
	}
	instancedMesh->updateMatrices();
	scene->addChild(instancedMesh);
	
	
	//正常绘制
	// 49个Mesh
	Geometry* geometry = Geometry::creatSphere(1.0f);  // 共享几何（球体）
	glm::mat4 model = glm::mat4(1.0f);
	for (int row = 0; row < nrRows; ++row)
	{	
		for (int col = 0; col < nrColumns; ++col)
		{	
			PBRMaterial* pBRMaterial = new PBRMaterial();
			pBRMaterial->mIrradianceConvolutionMap = irradianceMap;
			Mesh* mesh = new Mesh(geometry, pBRMaterial);
			pBRMaterial->mMetallic = (float)row / (float)nrRows;
			pBRMaterial->mRoughness = glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f);

			int id = row * nrColumns + col;
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(
				(col - (nrColumns / 2)) * spacing,
				(row - (nrRows / 2)) * spacing,
				0.0f
			));
			mesh->mModelMat = model;
			scene->addChild(mesh);	
		}
	}
	*/
	
	
	auto pointLight0 = new PointLight();
	pointLight0->mColor = glm::vec3(100.0f, 100.0f, 100.0f);
	pointLight0->setPosition(glm::vec3(0.0f, 10.0f, 10.0f));
	pointLight0->mK0 = 1.0f;
	pointLight0->mK1 = 0.07f;
	pointLight0->mK2 = 0.017f;
	pointLights.push_back(pointLight0);
	/*
	auto pointLight1 = new PointLight();
	pointLight1->mColor = glm::vec3(10.0f, 10.0f, 10.0f);
	pointLight1->setPosition(glm::vec3(10.0f, 10.0f, 10.0f));
	pointLight1->mK0 = 1.0f;
	pointLight1->mK1 = 0.07f;
	pointLight1->mK2 = 0.017f;
	pointLights.push_back(pointLight1);

	auto pointLight2 = new PointLight();
	pointLight2->mColor = glm::vec3(10.0f, 10.0f, 10.0f);
	pointLight2->setPosition(glm::vec3(-10.0f, -10.0f, 10.0f));
	pointLight2->mK0 = 1.0f;
	pointLight2->mK1 = 0.07f;
	pointLight2->mK2 = 0.017f;
	pointLights.push_back(pointLight2);

	auto pointLight3 = new PointLight();
	pointLight3->mColor = glm::vec3(10.0f, 10.0f, 10.0f);
	pointLight3->setPosition(glm::vec3(10.0f, -10.0f, 10.0f));
	pointLight3->mK0 = 1.0f;
	pointLight3->mK1 = 0.07f;
	pointLight3->mK2 = 0.017f;
	pointLights.push_back(pointLight3);*/
	/*
	Texture* texture = new Texture("assets/textures/wood.png", 0);
	PhongMaterial* phongMaterial = new PhongMaterial();
	phongMaterial->mDiffuse = texture;
	Geometry* geometry = Geometry::creatPlane(1.0f, 1.0f);
	Mesh* mesh = new Mesh(geometry, phongMaterial);
	scene->addChild(mesh);
	*/
	/*
	金字塔
	//auto plant = PlyLoader::load("assets/fbx/gear.ply");
	
	Geometry* pyramid = Geometry::creatPyramid(146.5, 230);
	Geometry* pyramidCube = Geometry::creatPyramidCube(146.5, 230);
	BlinnPhongMaterial* blinnPhongMaterial1 = new BlinnPhongMaterial();
	BlinnPhongMaterial* blinnPhongMaterial2 = new BlinnPhongMaterial();
	Texture* brick = new Texture("assets/textures/brick_tex.png", 0);
	Texture* concrete = new Texture(512, 512, 32.0f, 1);
	blinnPhongMaterial1->mDiffuse = brick;
	blinnPhongMaterial2->mDiffuse =concrete;
	Mesh* mesh1 = new Mesh(pyramid, blinnPhongMaterial1);
	Mesh* mesh2 = new Mesh(pyramidCube, blinnPhongMaterial2);
	scene->addChild(mesh1);
	scene->addChild(mesh2);
	camera->mPosition =glm::vec3 (0, 0, 200.0f);
	*/
	//平行光创建
	dirLight = new DirectionLight();
	dirLight->mDirection = glm::vec3(0.0f,0.0f,-1.0f);
	dirLight->mSpecularIntensity = 0.5f;

	ambLight = new AmbientLight();
	ambLight->mColor = glm::vec3(1.0f);
}

void initIMGUI(){
	ImGui::CreateContext();//创建imgui上下文
	ImGui::StyleColorsDark();//选择一个主题
	ImGuiIO& io = ImGui::GetIO();
	//设置ImGui与glfw和OpenGL的绑定
	//ImGui_ImplGlfw_InitForOpenGL(app->getWindow(), true);
	ImGui_ImplGlfw_InitForOpenGL(glApp->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void renderIMGUI() {
	//开启当前的IMGUI渲染
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("PointLightControl");
	ImGui::ColorEdit3("PointLight0", (float*)&pointLights[0]->mPosition);
	ImGui::End();
	/*
	//决定当前的GUI上面有哪些控件，从上到下
	ImGui::Begin("GrassMaterialEditor");

	ImGui::Text("GrassColor");
	ImGui::SliderFloat("UVScale", &grassInstancedMaterial->mUVScale, 0.0f, 100.0f);
	ImGui::InputFloat("Brightness", &grassInstancedMaterial->mBrightness);

	ImGui::Text("Wind");
	ImGui::InputFloat("WindScale", &grassInstancedMaterial->mWindScale);
	ImGui::InputFloat("PhaseScale", &grassInstancedMaterial->mPhaseScale);
	ImGui::ColorEdit3("WindDirection",(float*)&grassInstancedMaterial->mWindDirection);

	ImGui::Text("Cloud");
	ImGui::ColorEdit3("CloudBlackColor", (float*)&grassInstancedMaterial->mCloudBlackColor);
	ImGui::ColorEdit3("CloudWhiteColor", (float*)&grassInstancedMaterial->mCloudWhiteColor);
	ImGui::SliderFloat("CloudUVScale", &grassInstancedMaterial->mCloudUVScale, 0.0f, 100.0f);
	ImGui::InputFloat("CloudSpeed", &grassInstancedMaterial->mCloudSpeed);
	ImGui::SliderFloat("CloudLerp", &grassInstancedMaterial->mCloudLerp, 0.0f, 1.0f);

	ImGui::Text("Light");
	ImGui::InputFloat("LightIntensity", &dirLight->mIntensity);
	ImGui::End();
	*/

	//执行UI渲染
	ImGui::Render();
	//获取当前窗体的宽高
	int display_w, display_h;
	glfwGetFramebufferSize(glApp->getWindow(),&display_w,&display_h);
	//重置视口大小
	glViewport(0,0,display_w,display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

int main() {
	if (!glApp->init(WIDTH,HEIGHT)) return -1;
	glApp->setResizeCallback(OnResize);
	glApp->setKeyCallback(OnKey);
	glApp->setMouseCallback(OnMouse);
	glApp->setCursorCallback(OnCursor);
	glApp->setScrollCallback(OnScroll);
	GL_CALL(glViewport(0, 0, WIDTH,HEIGHT);)//规定视口大小：左下角起始坐标，视口大小
	GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f);)//规定画布颜色
		
	
	prepareCamera();
	prepare();
	initIMGUI();
	//执行窗体循环

	GL_CALL(glViewport(0, 0, WIDTH, HEIGHT);)//规定视口大小：左下角起始坐标，视口大小
	while (glApp->update()) {
		//scene->getChildren()[0]->rotateZ(0.01f);
		//mesh->setPosition(glm::mat3(glm::rotate(glm::mat4(1.0f),glm::radians(1.0f),glm::vec3(0.0f,0.0f,1.0f)))*mesh->getPosition());
		//pointLight->setPosition(glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f))) * pointLight->getPosition());
		cameraControl->update();
		renderer->setClearColor(clearColor);
		//renderer->render(scene, camera, dirLight, pointLights, ambLight, 0);
		
		//渲染深度画布
		//renderer->render(sceneFbo, camera, dirLight, pointLights, ambLight, frameBuffer->mFBO);
		//渲染阴影图到屏幕画布
		renderer->render(scene, camera, dirLight, pointLights, ambLight, 0);
		renderIMGUI();

	}
	//退出程序前做相关清理
	
	glApp->destroy();
	return 0;
}