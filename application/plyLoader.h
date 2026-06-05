#pragma once
#include"../glframework/core.h"
#include"../glframework/object.h"
#include"../glframework/mesh/mesh.h"
#include"../glframework/texture.h"
class PlyLoader {
public:
	PlyLoader();
	~PlyLoader();
public:
	static Object* load(const std::string& path);
};