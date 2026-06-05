#include "instancedMesh.h"
#include<algorithm>
InstancedMesh::InstancedMesh(Geometry* geometry, Material* material,unsigned int instancedCount):Mesh(geometry, material)
{
	mInstancedCount = instancedCount;
	mInstancedModelMatrices.resize(mInstancedCount);

	glGenBuffers(1, &mMatrixVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)* mInstancedCount, mInstancedModelMatrices.data(), GL_DYNAMIC_DRAW);


	glBindVertexArray(geometry->getVao());
	glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo);

	GLsizei vec4Size = sizeof(glm::vec4);
	for (int i = 0;i < 4;i++) {
		glEnableVertexAttribArray(4 + i);
		glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size * i));//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，属性段长，此属性在vbo中每个点内起始位置
		glVertexAttribDivisor(4 + i, 1);//逐实例更新
	}
	mType = ObjectType::InstancedMesh;
}

InstancedMesh::~InstancedMesh()
{
	
}

void InstancedMesh::updateMatrices()
{
	glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo);
	//如果使用glbufferData更新数据，会导致重新分配显存空间
	glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(glm::mat4) * mInstancedCount, mInstancedModelMatrices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedMesh::sortMatrices(glm::mat4 viewMatrix)
{
	std::sort(mInstancedModelMatrices.begin(), mInstancedModelMatrices.end(),
		[viewMatrix](const glm::mat4 &a, const glm::mat4 &b) {
			//计算A的相机系的z
			auto modelMatA = a;
			auto worldPositionA = modelMatA * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//初始坐标都是（0,0,0）
			auto viewMatA = viewMatrix;
			auto cameraPositonA = viewMatA * worldPositionA;

			//计算B的相机系的z
			auto modelMatB = b;
			auto worldPositionB = modelMatB * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//初始坐标都是（0,0,0）
			auto viewMatB = viewMatrix;
			auto cameraPositonB = viewMatB * worldPositionB;

			return cameraPositonA.z < cameraPositonB.z;
		});
}
