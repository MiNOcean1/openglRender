#pragma once
#include"mesh.h"
class InstancedMesh:public Mesh {
public:
	InstancedMesh(Geometry* geometry,Material* material,unsigned int instancedCount);
	~InstancedMesh();
public:
	void updateMatrices();
	void sortMatrices(glm::mat4 viewMatrix);
public:
	unsigned int mInstancedCount{ 0 };
	std::vector<glm::mat4> mInstancedModelMatrices;
	GLuint mMatrixVbo{ 0 };
};