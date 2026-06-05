#pragma once
#include"core.h"
#include<vector>
#include<iostream>
enum class ObjectType
{
	Object,
	Mesh,
	Scene,
	InstancedMesh
};
class Object {
public:
	Object();
	~Object();
	void setPosition(glm::vec3 pos);
	
	void setAngle(glm::vec3 angle);
	void setAngleX(float angle);
	void setAngleY(float angle);
	void setAngleZ(float angle);
	//增量旋转
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);

	void setScale(glm::vec3 scale);
	glm::vec3 getPosition() const;
	glm::mat4 getModelMat() const;
	glm::mat4 getNormalMat() const;
	void addChild(Object* child);
	std::vector<Object*> getChildren();
	Object* getParent();
	ObjectType getType() { return mType; }
	
	ObjectType mType;
	glm::vec3 mPosition{0.0f};
	glm::mat4 mModelMat = glm::mat4(1.0f);
protected:

	//unity旋转标准：pitch yaw roll
	float mAngleX{ 0.0f };//绕x旋转角度
	float mAngleY{ 0.0f };//绕y旋转角度
	float mAngleZ{ 0.0f };//绕z旋转角度

	glm::vec3 mScale{ 1.0f };//缩放比例
	//父子关系
	std::vector<Object*> mChildren;
	Object* mParent{ nullptr };

	//类型记录
};