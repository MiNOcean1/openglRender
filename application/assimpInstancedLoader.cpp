#include "assimpInstancedLoader.h"
#include"../glframework/tools/tools.h"
#include"../glframework/material/phongInstancedMaterial.h"
AssimpInstancedLoader::AssimpInstancedLoader()
{
}
AssimpInstancedLoader::~AssimpInstancedLoader()
{
}
Object* AssimpInstancedLoader::load(const std::string& path, unsigned int instancedCount)
{
	//拿出模型所在目录
	std::size_t lastIndex = path.find_last_of('/');
	auto rootPath = path.substr(0, lastIndex + 1);

	Object* rootNode = new Object();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);//将四边形转为三角形点位，产生法线

	//验证读取是否正确
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {//空的，不完整的，没有根节点
		std::cerr << "Error:Model Read Failed" << std::endl;
		return nullptr;
	}
	processNode(scene->mRootNode, rootNode, scene, rootPath, instancedCount);

	return rootNode;

}

void AssimpInstancedLoader::processNode(aiNode* ainode, Object* parent, const aiScene* scene, const std::string& rootPath, unsigned int instancedCount)
{
	Object* node = new Object();
	parent->addChild(node);
	glm::mat4 localMatrix = getMat4f(ainode->mTransformation);
	//位置 旋转 缩放
	glm::vec3 position, eulerAngle, scale;
	Tools::decompose(localMatrix, position, eulerAngle, scale);
	node->setPosition(position);
	node->setAngleX(eulerAngle.x);
	node->setAngleY(eulerAngle.y);
	node->setAngleZ(eulerAngle.z);
	node->setScale(scale);

	//检查有没有mesh，并且解析
	for (int i = 0;i < ainode->mNumMeshes;i++) {
		int meshID = ainode->mMeshes[i];
		aiMesh* aimesh = scene->mMeshes[meshID];
		auto mesh = processMesh(aimesh, scene, rootPath, instancedCount);
		node->addChild(mesh);
	}

	for (int i = 0;i < ainode->mNumChildren;i++) {
		processNode(ainode->mChildren[i], node, scene, rootPath, instancedCount);
	}
}

InstancedMesh* AssimpInstancedLoader::processMesh(aiMesh* aimesh, const aiScene* scene, const std::string& rootPath, unsigned int instancedCount)
{
	std::vector<float>position;
	std::vector<float>normal;
	std::vector<float>uvs;
	std::vector<float>color;
	std::vector<unsigned int>indices;

	for (int i = 0;i < aimesh->mNumVertices;i++) {
		//第i个顶点的位置
		position.push_back(aimesh->mVertices[i].x);
		position.push_back(aimesh->mVertices[i].y);
		position.push_back(aimesh->mVertices[i].z);

		//第i个顶点的法线
		normal.push_back(aimesh->mNormals[i].x);
		normal.push_back(aimesh->mNormals[i].y);
		normal.push_back(aimesh->mNormals[i].z);

		//第i个顶点的color,与uv信息类似
		if (aimesh->HasVertexColors(0)) {
			color.push_back(aimesh->mColors[0][i].r);
			color.push_back(aimesh->mColors[0][i].g);
			color.push_back(aimesh->mColors[0][i].b);
		}

		//第i个顶点的uv
		//模型可能有很多套uv，关注其第0套uv，一般情况下是贴图uv
		if (aimesh->mTextureCoords[0]) {
			uvs.push_back(aimesh->mTextureCoords[0][i].x);
			uvs.push_back(aimesh->mTextureCoords[0][i].y);
		}
		else {
			uvs.push_back(0.0f);
			uvs.push_back(0.0f);
		}
	}
	//解析mesh中的索引值,mesh中有很多face，每个face包含自己的索引
	for (int i = 0;i < aimesh->mNumFaces;i++) {
		aiFace face = aimesh->mFaces[i];
		for (int j = 0;j < face.mNumIndices;j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	auto geometry = new Geometry(position, normal, uvs, color,indices);
	auto material = new PhongInstancedMaterial();

	if (aimesh->mMaterialIndex >= 0) {
		Texture* texture = nullptr;
		aiMaterial* aiMaterial = scene->mMaterials[aimesh->mMaterialIndex];

		//读取了diffuse贴图
		texture = processTexture(aiMaterial, aiTextureType_DIFFUSE, scene, rootPath);
		if (texture == nullptr) {
			texture = Texture::createTexture("assets/textures/goku.jpg", 0);
		}
		texture->setUnit(0);
		material->mDiffuse = texture;

		//读取specular贴图
		auto specularMask = processTexture(aiMaterial, aiTextureType_SPECULAR, scene, rootPath);
		if (specularMask == nullptr)
		{
			specularMask = Texture::createTexture("assets/textures/goku.jpg", 0);
		}
		specularMask->setUnit(1);
		material->mSpecular = specularMask;

	}
	else {
		//没有texture的情况下
		material->mDiffuse = Texture::createTexture("assets/textures/goku.jpg", 0);
	}
	return new InstancedMesh(geometry, material, instancedCount);
}

Texture* AssimpInstancedLoader::processTexture(const aiMaterial* aiMaterial, const aiTextureType& type, const aiScene* scene, const std::string& rootPath)
{
	Texture* texture = nullptr;
	//获取图片路径
	aiString aiPath;
	aiMaterial->Get(AI_MATKEY_TEXTURE(type, 0), aiPath);//得到第0张diffuse路径存到aiPath


	if (!aiPath.length)return nullptr;
	//判断是否是嵌入fbx的图片
	const aiTexture* aitexture = scene->GetEmbeddedTexture(aiPath.C_Str());
	if (aitexture) {//纹理图片是内嵌的
		unsigned char* dataIn = reinterpret_cast<unsigned char*>(aitexture->pcData);
		uint32_t widthIn = aitexture->mWidth;//通常情况下（png、jpg），代表了整张图片大小
		uint32_t heightIn = aitexture->mHeight;
		texture = Texture::createTextureFromMemory(aiPath.C_Str(), 0, dataIn, widthIn, heightIn);
	}
	else {//纹理图片在硬盘上
		std::string fullPath = rootPath + aiPath.C_Str();
		texture = Texture::createTexture(fullPath.c_str(), 0);
	}
	return texture;
}



glm::mat4 AssimpInstancedLoader::getMat4f(aiMatrix4x4 value)
{
	glm::mat4 to(
		value.a1, value.a2, value.a3, value.a4,
		value.b1, value.b2, value.b3, value.b4,
		value.c1, value.c2, value.c3, value.c4,
		value.d1, value.d2, value.d3, value.d4
	);
	return to;
}
