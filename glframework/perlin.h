#pragma once
#include"core.h"
#include<vector>
class Perlin {
public:
	Perlin();
	~Perlin();
public:
	unsigned int SEED;
	std::vector<unsigned char> GenerateNoise(int width, int height, float scale);
	float Smoothstep(float t);
	float Interpolate(float a0, float a1, float w, bool isLinear);
	glm::vec2 RandomGradient(int ix, int iy);
	float SampleNoise(float x, float y);
	float DotGridGradient(int ix, int iy, float x, float y);
};