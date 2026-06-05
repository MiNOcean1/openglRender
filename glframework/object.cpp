#include "object.h"
Object::Object() {
	mType =ObjectType::Object;
}

Object::~Object()
{
}

void Object::setPosition(glm::vec3 pos)
{
	mPosition = pos;
}

void Object::setAngle(glm::vec3 angle)
{
	mAngleX = angle.x;
	mAngleY = angle.y;
	mAngleZ = angle.z;
}

void Object::setAngleX(float angle)
{
	mAngleX = angle;
}

void Object::setAngleY(float angle)
{
	mAngleY = angle;
}

void Object::setAngleZ(float angle)
{
	mAngleZ = angle;
}

void Object::rotateX(float angle)
{
	mAngleX += angle;
}

void Object::rotateY(float angle)
{
	mAngleY += angle;
}

void Object::rotateZ(float angle)
{
	mAngleZ += angle;
}

void Object::setScale(glm::vec3 scale)
{
	mScale = scale;
}

glm::vec3 Object::getPosition() const
{
	return mPosition;
}

glm::mat4 Object::getModelMat() const
{
	//首先获取父亲的变换矩阵
	glm::mat4 parentMatrix{ 1.0f };
	if (mParent != nullptr)
	{
		parentMatrix = mParent->getModelMat();
	}

	//unity: 缩放 旋转 平移
	glm::mat4 transform{ 1.0f };

	transform = glm::scale(transform, mScale);

	//unity旋转标准:pitch yaw roll (x-y-z)
	transform = glm::rotate(transform, glm::radians(mAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(mAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(mAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));

	//缩放和旋转是本地坐标系下  平移是世界坐标系
	transform = glm::translate(glm::mat4(1.0f), mPosition) * transform * parentMatrix;

	return transform;
}

glm::mat4 Object::getNormalMat() const
{
	return glm::transpose(glm::inverse(getModelMat()));
}

void Object::addChild(Object* child)
{
	//检查是否加入过这个孩子
	auto iter = std::find(mChildren.begin(),mChildren.end(),child);
	if (iter != mChildren.end()) {
		std::cerr << "Dupicated Child added" << std::endl;
		return;
	}
	//加入孩子
	mChildren.push_back(child);

	//告诉孩子父亲是谁
	child->mParent = this;
	
}

std::vector<Object*> Object::getChildren()
{
	return mChildren;
}

Object* Object::getParent()
{
	return mParent;
}
