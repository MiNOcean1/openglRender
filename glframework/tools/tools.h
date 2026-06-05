#pragma once
#include"../core.h"
class Tools {
public:
	Tools();
	~Tools();
public:
	//传入一个矩阵，解构其中的位置、旋转信息XYZ、缩放信息
	static void decompose(glm::mat4 matrix,glm::vec3 &position,glm::vec3 &eulerAngle,glm::vec3 &scale);
	//static void calTBN(glm::vec3 point0,glm::vec3 point1,glm::vec3 point2,glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2,glm::vec3 &T,glm::vec3 &B);
};