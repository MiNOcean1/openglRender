#pragma once
#include"core.h"
#include<iostream>
#include<vector>
class Geometry {
public:
	Geometry();
	Geometry(
		const std::vector<float>& position,
		const std::vector<float>& normal,
		const std::vector<float>& uvs,
		const std::vector<unsigned int>& indices
	);
	Geometry(
		const std::vector<float>& position,
		const std::vector<float>& normal,
		const std::vector<float>& color,
		const std::vector<unsigned int>& indices,
		int f
	);
	Geometry(
		const std::vector<float>& position,
		const std::vector<float>& normal,
		const std::vector<float>& uvs,
		const std::vector<float>& color,
		const std::vector<unsigned int>& indices
	);
	~Geometry();
public:
	GLuint mVao{ 0 };
	GLuint mPosVbo{ 0 };
	GLuint mUvVbo{ 0 };
	GLuint mNormalVbo{ 0 };
	GLuint mColorVbo{ 0 };
	GLuint mEbo{ 0 };
	uint32_t mIndicesCount{ 0 };
	GLuint mType{ GL_TRIANGLES };
public:
	static Geometry* creatBox(float size);//创建一个立方体
	static Geometry* creatCameraModel();//创建一个摄像机模型
	static Geometry* creatCameraAxis();
	static Geometry* creatSphere(float size);//创建一个球体
	static Geometry* creatPlane(float width, float height);//创建一个长方形
	static Geometry* creatGear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width, GLint teeth, GLfloat tooth_depth);//创建一个齿轮
	static Geometry* creatPyramid(float h, float w);
	static Geometry* creatPyramidCube(float h, float w);

	static Geometry* creatScreenPlane();
	static Geometry* creatPlaneWithTBN();

	GLuint getVao() const;//加个const保证不能修改成员变量
	GLuint getType() const;
	uint32_t getIndicesCount() const;
};