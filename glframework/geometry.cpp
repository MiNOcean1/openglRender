#include "geometry.h"
Geometry::Geometry()
{
}

Geometry::Geometry(const std::vector<float>& position, const std::vector<float>& normal, const std::vector<float>& uvs, const std::vector<unsigned int>& indices)
{
	
	mIndicesCount = indices.size();
	//posVbo创建
	GLuint& posVbo = mPosVbo, & uvVbo = mUvVbo, & normalVbo = mNormalVbo;

	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * position.size(), position.data(), GL_STATIC_DRAW);
	//uvVbo创建
	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvs.size(), uvs.data(), GL_STATIC_DRAW);//插槽，传入数据个数，需要传入的数据，数据是否频繁修改
	//normalVbo创建
	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normal.size(), normal.data(), GL_STATIC_DRAW);

	GLuint& ebo = mEbo, & vao = mVao;
	

	//4 VAO创建
	//GLuint& ;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//3 EBO创建
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
	
	//绑定posVbo
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，属性段长，此属性在vbo中每个点内起始位置

	//绑定uvVbo
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	//绑定normalVbo
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//绑定ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
}

Geometry::Geometry(const std::vector<float>& position, const std::vector<float>& normal, const std::vector<float>& color, const std::vector<unsigned int>& indices, int f)
{
	mIndicesCount = indices.size();
	//posVbo创建
	GLuint& posVbo = mPosVbo, & colorVbo = mColorVbo, & normalVbo = mNormalVbo;


	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * position.size(), position.data(), GL_STATIC_DRAW);
	//normalVbo创建
	if (normal.size()) {
		glGenBuffers(1, &normalVbo);
		glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normal.size(), normal.data(), GL_STATIC_DRAW);
	}
	//colorVbo的创建
	if (color.size()) {
		glGenBuffers(1, &colorVbo);
		glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * color.size(), color.data(), GL_STATIC_DRAW);
	}
	GLuint& ebo = mEbo, & vao = mVao;

	//4 VAO创建
	//GLuint& ;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//动态获取位置
	//GLuint posLocation = glGetAttribLocation(shader->mProgram, "aPos");
	//3 EBO创建
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), indices.data(), GL_STATIC_DRAW);


	//绑定posVbo
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，属性段长，此属性在vbo中每个点内起始位置

	//绑定normalVbo
	if (normal.size()) {
		glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	}

	//绑定colorVbo
	if (color.size()) {
		glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	}
	//绑定ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
}

Geometry::Geometry(const std::vector<float>& position,
	const std::vector<float>& normal,
	const std::vector<float>& uvs,
	const std::vector<float>& color,
	const std::vector<unsigned int>& indices)
{
	mIndicesCount = indices.size();
	//posVbo创建
	GLuint& posVbo = mPosVbo,&colorVbo=mColorVbo, & uvVbo = mUvVbo, & normalVbo = mNormalVbo;

	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * position.size(), position.data(), GL_STATIC_DRAW);
	//uvVbo创建
	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvs.size(), uvs.data(), GL_STATIC_DRAW);//插槽，传入数据个数，需要传入的数据，数据是否频繁修改
	//normalVbo创建
	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normal.size(), normal.data(), GL_STATIC_DRAW);
	//colorVbo的创建
	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * color.size(), color.data(), GL_STATIC_DRAW);

	GLuint& ebo = mEbo,& vao = mVao;
	//3 EBO创建
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), indices.data(), GL_STATIC_DRAW);

	//4 VAO创建
	//GLuint& ;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//动态获取位置
	//GLuint posLocation = glGetAttribLocation(shader->mProgram, "aPos");

	//绑定posVbo
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，属性段长，此属性在vbo中每个点内起始位置

	//绑定uvVbo
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	//绑定normalVbo
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//绑定colorVbo
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//绑定ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
}

Geometry::~Geometry()
{	
	if(mVao!=0){
		glDeleteVertexArrays(1, &mVao);
	}
	if (mPosVbo != 0) {
		glDeleteBuffers(1, &mPosVbo);
	}
	if (mUvVbo != 0) {
		glDeleteBuffers(1, &mUvVbo);
	}
	if (mEbo != 0) {
		glDeleteBuffers(1, &mEbo);
	}
	if (mNormalVbo != 0) {
		glDeleteBuffers(1, &mNormalVbo);
	}
	if (mColorVbo != 0) {
		glDeleteBuffers(1, &mColorVbo);
	}
}

Geometry* Geometry::creatBox(float size)
{
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 36;
	float halfSize = size / 2.0f;
	float position[] = {
		// Front face
		-halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize,
		// Back face
		-halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize,
		// Top face
		-halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize,
		// Bottom face
		-halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize,
		// Right face
		halfSize, -halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize,
		// Left face
		-halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize
	};
	float normal[] = {
		// Front face
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		// Back face
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		// Top face
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		// Bottom face
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		// Right face
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		// Left face
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
	};//每个顶点对应的法线向量，相同点在不同面法线可能不一样
	float uvs[] = {
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
	};
	unsigned int indices[] = {
		0, 1, 2, 2, 3, 0,   // Front face
		4, 5, 6, 6, 7, 4,   // Back face
		8, 9, 10, 10, 11, 8,  // Top face
		12, 13, 14, 14, 15, 12, // Bottom face
		16, 17, 18, 18, 19, 16, // Right face
		20, 21, 22, 22, 23, 20  // Left face
	};
	//posVbo创建
	GLuint &posVbo = geometry->mPosVbo, &uvVbo = geometry->mUvVbo,&normalVbo=geometry->mNormalVbo;
	
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	//uvVbo创建
	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);//插槽，传入数据个数，需要传入的数据，数据是否频繁修改
	//normalVbo创建
	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normal), normal, GL_STATIC_DRAW);

	GLuint &ebo = geometry->mEbo,&vao = geometry->mVao;

	//3 VAO创建
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//4 EBO创建
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		
	
	//动态获取位置
	//GLuint posLocation = glGetAttribLocation(shader->mProgram, "aPos");

	//绑定posVbo
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，属性段长，此属性在vbo中每个点内起始位置

	//绑定uvVbo
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	//绑定normalVbo
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//绑定ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
	return geometry;
}

Geometry* Geometry::creatCameraModel()
{	
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 36;
	// 顶点位置数组（x,y,z 顺序，共14个顶点 × 3 = 42个float）
	float position[24] = {
		// 长方体顶点（8个）
		// 前面（z=1）
		1.0f, 1.0f, 1.0f,    // 0
		1.0f, -1.0f, 1.0f,   // 1
		-1.0f, -1.0f, 1.0f,  // 2
		-1.0f, 1.0f, 1.0f,   // 3
		// 后面（z=-1）
		1.0f, 1.0f, -1.0f,   // 4
		1.0f, -1.0f, -1.0f,  // 5
		-1.0f, -1.0f, -1.0f, // 6
		-1.0f, 1.0f, -1.0f,  // 7

	};

	// 顶点颜色数组（r,g,b 顺序，与位置数组一一对应，共14个顶点 × 3 = 42个float）
	float color[24] = {
		// 长方体颜色（灰色系）
		// 前面（稍亮）
		0.8f, 0.8f, 0.8f,    // 0
		0.8f, 0.8f, 0.8f,    // 1
		0.8f, 0.8f, 0.8f,    // 2
		0.8f, 0.8f, 0.8f,    // 3
		// 后面（稍暗）
		0.6f, 0.6f, 0.6f,    // 4
		0.6f, 0.6f, 0.6f,    // 5
		0.6f, 0.6f, 0.6f,    // 6
		0.6f, 0.6f, 0.6f,    // 7
	};

	// 长方体索引数组（三角形，共36个索引）
	unsigned int indices[36] = {
		// 前面 (z=1)
		0, 1, 2,
		2, 3, 0,
		// 后面 (z=-1)
		4, 7, 6,
		6, 5, 4,
		// 右面 (x=1)
		0, 4, 5,
		5, 1, 0,
		// 左面 (x=-1)
		2, 6, 7,
		7, 3, 2,
		// 上面 (y=1)
		0, 3, 7,
		7, 4, 0,
		// 下面 (y=-1)
		1, 5, 6,
		6, 2, 1
	};


	//posVbo创建
	GLuint& posVbo = geometry->mPosVbo, & colorVbo = geometry->mColorVbo;

	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	//uvVbo创建
	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);//插槽，传入数据个数，需要传入的数据，数据是否频繁修改

	GLuint& ebo = geometry->mEbo, & vao = geometry->mVao;
	//3 VAO创建
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//4 EBO创建
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	//动态获取位置
	//GLuint posLocation = glGetAttribLocation(shader->mProgram, "aPos");

	//绑定posVbo
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，属性段长，此属性在vbo中每个点内起始位置

	//绑定colorVbo
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	//绑定ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
	return geometry;

}

Geometry* Geometry::creatCameraAxis()
{
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 6;
	geometry->mType = GL_LINES;
	// 顶点位置数组（x,y,z 顺序，共14个顶点 × 3 = 42个float）
	float position[18] = {
		// 坐标轴顶点（6个）
		// X轴（红色）
		0.0f, 0.0f, 0.0f,    // 8（原点）
		1.2f, 0.0f, 0.0f,    // 9（端点）
		// Y轴（绿色）
		0.0f, 0.0f, 0.0f,    // 10（原点）
		0.0f, 1.2f, 0.0f,    // 11（端点）
		// Z轴（蓝色）
		0.0f, 0.0f, 0.0f,    // 12（原点）
		0.0f, 0.0f, 1.2f     // 13（端点）
	};

	// 顶点颜色数组（r,g,b 顺序，与位置数组一一对应，共14个顶点 × 3 = 42个float）
	float color[18] = {

		// 坐标轴颜色（X红/Y绿/Z蓝）
		1.0f, 0.0f, 0.0f,    // 8（X轴原点）
		1.0f, 0.0f, 0.0f,    // 9（X轴端点）
		0.0f, 1.0f, 0.0f,    // 10（Y轴原点）
		0.0f, 1.0f, 0.0f,    // 11（Y轴端点）
		0.0f, 0.0f, 1.0f,    // 12（Z轴原点）
		0.0f, 0.0f, 1.0f     // 13（Z轴端点）
	};

	// 长方体索引数组（三角形，共36个索引）
	unsigned int indices[6] = {
		0,1,
		2,3,
		4,5
	};


	//posVbo创建
	GLuint& posVbo = geometry->mPosVbo, & colorVbo = geometry->mColorVbo;

	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	
	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);//插槽，传入数据个数，需要传入的数据，数据是否频繁修改

	GLuint& ebo = geometry->mEbo, & vao = geometry->mVao;
	//3 VAO创建
	//GLuint& ;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//4 EBO创建
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	//动态获取位置
	//GLuint posLocation = glGetAttribLocation(shader->mProgram, "aPos");

	//绑定posVbo
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，属性段长，此属性在vbo中每个点内起始位置

	//绑定uvVbo
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	//绑定ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
	return geometry;
}

Geometry* Geometry::creatSphere(float size)
{
	Geometry* geometry = new Geometry();
	
	std::vector<float>position, uvs;//球体的法线方向就是position
	std::vector<GLuint>indices;
	int longitudeNumber = 128;//经线数量
	int latitudeNumber = 128;//纬线数量
	
	for (int i = 0;i <= latitudeNumber;i++) {
		for (int j = 0;j <= longitudeNumber;j++) {
			float alpha = glm::pi<float>()*(float)i/(float)latitudeNumber;
			float theta = 2.0f*glm::pi<float>() * (float)j / (float)longitudeNumber;
			float x = size * sin(alpha) * cos(theta);
			float y = size * cos(alpha);
			float z = size * sin(alpha) * sin(theta);
			position.push_back(x);
			position.push_back(y);
			position.push_back(z);

			uvs.push_back((float)i / (float)latitudeNumber);
			uvs.push_back((float)j / (float)longitudeNumber);

			if (i !=latitudeNumber &&j!=longitudeNumber) {
				int nowPos = i * (longitudeNumber +1) + j;
				indices.push_back(nowPos);
				indices.push_back(nowPos + longitudeNumber +1);
				indices.push_back(nowPos + 1);

				indices.push_back(nowPos + 1);
				indices.push_back(nowPos + longitudeNumber +1);
				indices.push_back(nowPos + longitudeNumber +1 + 1);
			}
		}
	}
	geometry->mIndicesCount = indices.size();
	
	GLuint& posVbo = geometry->mPosVbo, & uvVbo = geometry->mUvVbo,& normalVbo=geometry->mNormalVbo;
	//posVbo创建
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, position.size()*sizeof(float), position.data(), GL_STATIC_DRAW);
	//uvVbo创建
	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size()*sizeof(float), uvs.data(), GL_STATIC_DRAW);//插槽，传入数据大小，需要传入的数据，数据是否频繁修改
	//normalVbo创建
	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_STATIC_DRAW);

	//3 VAO创建
	GLuint& ebo = geometry->mEbo;GLuint &vao = geometry->mVao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//4 EBO创建
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(float), indices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
	//
	//GLuint& ;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//动态获取位置
	//GLuint posLocation = glGetAttribLocation(shader->mProgram, "aPos");

	//绑定posVbo
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，属性段长，此属性在vbo中每个点内起始位置

	//绑定uvVbo
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	//绑定normalVbo
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//绑定ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);

	return geometry;
}

Geometry* Geometry::creatPlane(float width, float height)
{
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount =6;
	float position[] = {
		-width / 2.0f,-height / 2.0f,0.0f,
		width / 2.0f,-height / 2.0f,0.0f,
		-width / 2.0f,height / 2.0f,0.0f,
		width / 2.0f,height / 2.0f,0.0f
	};
	float normal[] = {
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
	};
	float uvs[] = {
		0.0f,0.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f
	};
	unsigned int indices[] = {
		0,1,2,
		2,1,3
	};

	
	GLuint& posVbo = geometry->mPosVbo, & uvVbo = geometry->mUvVbo,&normalVbo=geometry->mNormalVbo;

	//posVbo创建
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	//uvVbo创建
	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);//插槽，传入数据个数，需要传入的数据，数据是否频繁修改
	//normalVbo创建
	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normal), normal, GL_STATIC_DRAW);

	GLuint& ebo = geometry->mEbo, & vao = geometry->mVao;

	//3 VAO创建
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//4 EBO创建
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	
	//动态获取位置
	//GLuint posLocation = glGetAttribLocation(shader->mProgram, "aPos");

	//绑定posVbo
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，属性段长，此属性在vbo中每个点内起始位置

	//绑定uvVbo
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	//绑定normalVbo
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//绑定ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
	return geometry;
}

Geometry* Geometry::creatGear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width, GLint teeth, GLfloat tooth_depth)
{
	//Geometry* geometry = new Geometry();
	std::vector<float>position;
	std::vector<float>normal;
	std::vector<GLuint>indices;
	GLint i;
	GLfloat r0, r1, r2;
	GLfloat angle, da;
	static bool bFirstTime = true;
	glm::vec3 p00, p01, p10, p11, p20, p21;
	glm::vec4 q;
	const double PI = 3.14159265;

	r0 = inner_radius;
	r1 = outer_radius - tooth_depth / 2.0;
	r2 = outer_radius + tooth_depth / 2.0;

	da = 2.0 * PI / teeth / 4.0;


	for (i = 0; i < teeth; i++)
	{
		angle = i * 2.0 * PI / teeth;

		//p00(r0 * cos(angle), r0 * sin(angle), width * 0.5);
		position.push_back(r0 * cos(angle));
		position.push_back(r0 * sin(angle));
		position.push_back(width * 0.5);

		normal.push_back(0);
		normal.push_back(0);
		normal.push_back(1.0f);

		//p01.Set(r0 * cos(angle + 3 * da), r0 * sin(angle + 3 * da), width * 0.5);
		//g_Pos.push_back(p01);
		position.push_back(r0 * cos(angle + 3 * da));
		position.push_back(r0 * sin(angle + 3 * da));
		position.push_back(width * 0.5);

		normal.push_back(0);
		normal.push_back(0);
		normal.push_back(1.0f);


		//p10.Set(r1 * cos(angle), r1 * sin(angle), width * 0.5);
		//g_Pos.push_back(p10);
		position.push_back(r1 * cos(angle));
		position.push_back(r1 * sin(angle));
		position.push_back(width * 0.5);

		normal.push_back(0);
		normal.push_back(0);
		normal.push_back(1.0f);

		//p11.Set(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
		//g_Pos.push_back(p11);
		position.push_back(r1 * cos(angle + 3 * da));
		position.push_back(r1 * sin(angle + 3 * da));
		position.push_back(width * 0.5);

		normal.push_back(0);
		normal.push_back(0);
		normal.push_back(1.0f);

		//p20.Set(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
		//g_Pos.push_back(p20);
		position.push_back(r2 * cos(angle + da));
		position.push_back(r2 * sin(angle + da));
		position.push_back(width * 0.5);

		normal.push_back(0);
		normal.push_back(0);
		normal.push_back(1.0f);

		//p21.Set(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
		//g_Pos.push_back(p21);
		position.push_back(r2 * cos(angle + 2 * da));
		position.push_back(r2 * sin(angle + 2 * da));
		position.push_back(width * 0.5);

		normal.push_back(0);
		normal.push_back(0);
		normal.push_back(1.0f);

		//p00.Set(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
		//g_Pos.push_back(p00);
		position.push_back(r0 * cos(angle));
		position.push_back(r0 * sin(angle));
		position.push_back(-width * 0.5);

		normal.push_back(0);
		normal.push_back(0);
		normal.push_back(-1.0f);

		//p01.Set(r0 * cos(angle + 3 * da), r0 * sin(angle + 3 * da), -width * 0.5);
		//g_Pos.push_back(p01);
		position.push_back(r0 * cos(angle + 3 * da));
		position.push_back(r0 * sin(angle + 3 * da));
		position.push_back(-width * 0.5);

		normal.push_back(0);
		normal.push_back(0);
		normal.push_back(-1.0f);

		//p10.Set(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
		//g_Pos.push_back(p10);
		position.push_back(r1 * cos(angle));
		position.push_back(r1 * sin(angle));
		position.push_back(-width * 0.5);

		normal.push_back(0);
		normal.push_back(0);
		normal.push_back(-1.0f);

		//p11.Set(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
		//g_Pos.push_back(p11);
		position.push_back(r1 * cos(angle + 3 * da));
		position.push_back(r1 * sin(angle + 3 * da));
		position.push_back(-width * 0.5);

		normal.push_back(0);
		normal.push_back(0);
		normal.push_back(-1.0f);

		//p20.Set(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
		//g_Pos.push_back(p20);
		position.push_back(r2 * cos(angle + da));
		position.push_back(r2 * sin(angle + da));
		position.push_back(-width * 0.5);

		normal.push_back(0);
		normal.push_back(0);
		normal.push_back(-1.0f);

		//p21.Set(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
		//g_Pos.push_back(p21);
		position.push_back(r2 * cos(angle + 2 * da));
		position.push_back(r2 * sin(angle + 2 * da));
		position.push_back(-width * 0.5);

		normal.push_back(0);
		normal.push_back(0);
		normal.push_back(-1.0f);

	}

	for (i = 0; i < teeth; i++)
	{
		//indices of front face
		//g_Face.push_back(Quad(12 * i, 12 * i + 2, 12 * i + 3, 12 * i + 1));
		indices.push_back(12 * i);
		indices.push_back(12 * i + 2);
		indices.push_back(12 * i + 3);

		indices.push_back(12 * i);
		indices.push_back(12 * i + 3);
		indices.push_back(12 * i + 1);

		//indices of back face
		indices.push_back(12 * i + 6);
		indices.push_back(12 * i + 2 + 6);
		indices.push_back(12 * i + 3 + 6);

		indices.push_back(12 * i + 6);
		indices.push_back(12 * i + 3 + 6);
		indices.push_back(12 * i + 1 + 6);

		//indices of side face
		//side1
		indices.push_back(12 * i + 4);
		indices.push_back(12 * i + 2);
		indices.push_back(12 * i + 8);

		indices.push_back(12 * i + 4);
		indices.push_back(12 * i + 8);
		indices.push_back(12 * i + 10);

		//side2
		indices.push_back(12 * i + 5);
		indices.push_back(12 * i + 4);
		indices.push_back(12 * i + 10);

		indices.push_back(12 * i + 5);
		indices.push_back(12 * i + 10);
		indices.push_back(12 * i + 11);


		//side3
		indices.push_back(12 * i + 3);
		indices.push_back(12 * i + 5);
		indices.push_back(12 * i + 11);

		indices.push_back(12 * i + 3);
		indices.push_back(12 * i + 11);
		indices.push_back(12 * i + 9);

		

		if (i == (teeth - 1)) {
			//g_Face.push_back(Quad(12 * i + 1, 12 * i + 3, 2, 0));

			indices.push_back(12 * i + 1);
			indices.push_back(12 * i + 3);
			indices.push_back(2);

			indices.push_back(12 * i + 1);
			indices.push_back(2);
			indices.push_back(0);

			//indices of back face
			indices.push_back(12 * i + 1 + 6);
			indices.push_back(12 * i + 3 + 6);
			indices.push_back(2 + 6);

			indices.push_back(12 * i + 1 + 6);
			indices.push_back(2 + 6);
			indices.push_back(0 + 6);
			
			//indices of side face side4
			indices.push_back(2);
			indices.push_back(12 * i + 3);
			indices.push_back(12 * i + 9);

			indices.push_back(2);
			indices.push_back(12 * i + 9);
			indices.push_back(8);

		}
		else {
			//g_Face.push_back(Quad(12 * i + 1, 12 * i + 3, 12 * (i + 1) + 2, 12 * (i + 1)));

			indices.push_back(12 * i+1);
			indices.push_back(12 * i + 3);
			indices.push_back(12 * (i + 1) + 2);

			indices.push_back(12 * i+1);
			indices.push_back(12 * (i + 1) + 2);
			indices.push_back(12 * (i + 1));

			//indices of back face
			indices.push_back(12 * i + 1 + 6);
			indices.push_back(12 * i + 3 + 6);
			indices.push_back(12 * (i + 1) + 2 + 6);

			indices.push_back(12 * i + 1 + 6);
			indices.push_back(12 * (i + 1) + 2 + 6);
			indices.push_back(12 * (i + 1) + 6);

			//indices of side face side4
			indices.push_back(12 * i + 14);
			indices.push_back(12 * i + 3);
			indices.push_back(12 * i + 9);

			indices.push_back(12 * i + 14);
			indices.push_back(12 * i + 9);
			indices.push_back(12 * i + 20);
		}
		//indices of front sides of teeth
		//g_Face.push_back(Quad(12 * i + 2, 12 * i + 4, 12 * i + 5, 12 * i + 3));
		indices.push_back(12 * i + 2);
		indices.push_back(12 * i + 4);
		indices.push_back(12 * i + 5);

		indices.push_back(12 * i + 2);
		indices.push_back(12 * i + 5);
		indices.push_back(12 * i + 3);

		indices.push_back(12 * i + 2 + 6);
		indices.push_back(12 * i + 4 + 6);
		indices.push_back(12 * i + 5 + 6);

		indices.push_back(12 * i + 2 + 6);
		indices.push_back(12 * i + 5 + 6);
		indices.push_back(12 * i + 3 + 6);

		//indices of inside radius cylinder
		//g_Face.push_back(Quad(12 * i, 12 * i + 1, 12 * i + 7, 12 * i + 6));
		indices.push_back(12 * i);
		indices.push_back(12 * i + 1);
		indices.push_back(12 * i + 7);

		indices.push_back(12 * i);
		indices.push_back(12 * i + 7);
		indices.push_back(12 * i + 6);

		if (i == (teeth - 1)) {
			//g_Face.push_back(Quad(12 * i + 1, 0, 6, 12 * i + 7));
			indices.push_back(12 * i + 1);
			indices.push_back(0);
			indices.push_back(6);

			indices.push_back(12 * i + 1);
			indices.push_back(6);
			indices.push_back(12 * i + 7);

			
		}
		else {
			//g_Face.push_back(Quad(12 * i + 1, 12 * (i + 1), 12 * (i + 1) + 6, 12 * i + 7));
			indices.push_back(12 * i + 1);
			indices.push_back(12 * (i + 1));
			indices.push_back(12 * (i + 1) + 6);

			indices.push_back(12 * i + 1);
			indices.push_back(12 * (i + 1) + 6);
			indices.push_back(12 * i + 7);
			
		}
	}
	//geometry->mIndicesCount = indices.size();
	//SaveGearModel("gear.ply");



	std::vector<float>color;
	return new Geometry(position, normal, color, indices);
}

Geometry* Geometry::creatPyramid(float h, float w)
{	
	Geometry* geometry = new Geometry();
	float d = sqrt(4 * h * h + w * w);
	float position[] = {
		// 第一个三角形（侧面1）
		0.0f,    h,     0.0f,          // v0：顶端
		-w / 2.0f, 0.0f,  w / 2.0f,       // v2：底面左上角
		w / 2.0f,  0.0f,  w / 2.0f,       // v1：底面右上角

		// 第二个三角形（侧面2）
		0.0f,    h,     0.0f,          // v0：顶端
		-w / 2.0f, 0.0f,  -w / 2.0f,      // v3：底面左下角
		-w / 2.0f, 0.0f,  w / 2.0f,       // v2：底面左上角

		// 第三个三角形（侧面3）
		0.0f,    h,     0.0f,          // v0：顶端
		w / 2.0f,  0.0f,  -w / 2.0f,      // v4：底面右下角
		-w / 2.0f, 0.0f,  -w / 2.0f,      // v3：底面左下角

		// 第四个三角形（侧面4）
		0.0f,    h,     0.0f,          // v0：顶端
		w / 2.0f,  0.0f,  w / 2.0f,       // v1：底面右上角
		w / 2.0f,  0.0f,  -w / 2.0f       // v4：底面右下角
	};
	float normal[] = {
		// 第一个三角形（侧面1）→ 法向量n0
		0.0f,        w / d,        2.0f * h / d,
		0.0f,        w / d,        2.0f * h / d,
		0.0f,        w / d,        2.0f * h / d,

		// 第二个三角形（侧面2）→ 法向量n1
		-2.0f * h / d,   w / d,        0.0f,
		-2.0f * h / d,   w / d,        0.0f,
		-2.0f * h / d,   w / d,        0.0f,

		// 第三个三角形（侧面3）→ 法向量n2
		0.0f,        w / d,        -2.0f * h / d,
		0.0f,        w / d,        -2.0f * h / d,
		0.0f,        w / d,        -2.0f * h / d,

		// 第四个三角形（侧面4）→ 法向量n3
		2.0f * h / d,    w / d,        0.0f,
		2.0f * h / d,    w / d,        0.0f,
		2.0f * h / d,    w / d,        0.0f
	};
	float uvs[] = {
		// 第一个三角形（侧面1）
		0.5f, 0.5f,    // v0：顶端纹理坐标
		0.0f, 0.0f,    // v3：底面左下角纹理坐标
		1.0f, 0.0f,    // v2：底面左上角纹理坐标

		// 第二个三角形（侧面2）
		0.0f, 0.0f,    // v0：顶端纹理坐标
		0.0f, 1.0f,    // v3：底面左下角纹理坐标
		1.0f, 1.0f,    // v2：底面左上角纹理坐标

		// 第三个三角形（侧面3）
		0.0f, 0.0f,    // v0：顶端纹理坐标
		0.0f, 1.0f,    // v4：底面右下角纹理坐标
		1.0f, 1.0f,    // v3：底面左下角纹理坐标

		// 第四个三角形（侧面4）
		0.0f, 0.0f,    // v0：顶端纹理坐标
		0.0f, 1.0f,    // v1：底面右上角纹理坐标
		1.0f, 1.0f     // v4：底面右下角纹理坐标
	};
	unsigned int indices[] = {
		0, 1, 2,    // 第一个三角形
		3, 4, 5,    // 第二个三角形
		6, 7, 8,    // 第三个三角形
		9, 10, 11   // 第四个三角形
	};
	geometry->mIndicesCount = 12;
	//posVbo创建
	GLuint& posVbo = geometry->mPosVbo, & uvVbo = geometry->mUvVbo, & normalVbo = geometry->mNormalVbo;

	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	//uvVbo创建
	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);//插槽，传入数据个数，需要传入的数据，数据是否频繁修改
	//normalVbo创建
	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normal), normal, GL_STATIC_DRAW);

	GLuint& ebo = geometry->mEbo, & vao = geometry->mVao;

	//3 VAO创建
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//4 EBO创建
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//动态获取位置
	//GLuint posLocation = glGetAttribLocation(shader->mProgram, "aPos");

	//绑定posVbo
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，属性段长，此属性在vbo中每个点内起始位置

	//绑定uvVbo
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	//绑定normalVbo
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//绑定ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
	return geometry;

}

Geometry* Geometry::creatPyramidCube(float h, float w)
{
	float s = 1.068f;         // 底座缩放因子（比金字塔底面大）
	float thickness = 5.0f; // 底座厚度（y方向高度）
	Geometry* geometry = new Geometry();
	float d = sqrt(4 * h * h + w * w);
	float position[] = {
		// -------------------------- 顶面（y=0，法向量(0,1,0)）--------------------------
		s * w / 2.0f,  0.0f,        s * w / 2.0f,       // 0: 顶面右上
		-s * w / 2.0f, 0.0f,        s * w / 2.0f,       // 1: 顶面左上
		-s * w / 2.0f, 0.0f,        -s * w / 2.0f,      // 2: 顶面左下
		s * w / 2.0f,  0.0f,        -s * w / 2.0f,      // 3: 顶面右下

		// -------------------------- 底面（y=-thickness，法向量(0,-1,0)）--------------------------
		s * w / 2.0f,  -thickness,  s * w / 2.0f,       // 4: 底面右上
		-s * w / 2.0f, -thickness,  s * w / 2.0f,       // 5: 底面左上
		-s * w / 2.0f, -thickness,  -s * w / 2.0f,      // 6: 底面左下
		s * w / 2.0f,  -thickness,  -s * w / 2.0f,      // 7: 底面右下

		// -------------------------- 前面（z+，法向量(0,0,1)）--------------------------
		s * w / 2.0f,  0.0f,        s * w / 2.0f,       // 8: 前面右上（复用顶面0）
		-s * w / 2.0f, 0.0f,        s * w / 2.0f,       // 9: 前面左上（复用顶面1）
		-s * w / 2.0f, -thickness,  s * w / 2.0f,       // 10: 前面左下（复用底面5）
		s * w / 2.0f,  -thickness,  s * w / 2.0f,       // 11: 前面右下（复用底面4）

		// -------------------------- 左面（x-，法向量(-1,0,0)）--------------------------
		-s * w / 2.0f, 0.0f,        s * w / 2.0f,       // 12: 左面右上（复用顶面1）
		-s * w / 2.0f, 0.0f,        -s * w / 2.0f,      // 13: 左面右下（复用顶面2）
		-s * w / 2.0f, -thickness,  -s * w / 2.0f,      // 14: 左面左下（复用底面6）
		-s * w / 2.0f, -thickness,  s * w / 2.0f,       // 15: 左面左上（复用底面5）

		// -------------------------- 后面（z-，法向量(0,0,-1)）--------------------------
		-s * w / 2.0f, 0.0f,        -s * w / 2.0f,      // 16: 后面右上（复用顶面2）
		s * w / 2.0f,  0.0f,        -s * w / 2.0f,      // 17: 后面右下（复用顶面3）
		s * w / 2.0f,  -thickness,  -s * w / 2.0f,      // 18: 后面左下（复用底面7）
		-s * w / 2.0f, -thickness,  -s * w / 2.0f,      // 19: 后面左上（复用底面6）

		// -------------------------- 右面（x+，法向量(1,0,0)）--------------------------
		s * w / 2.0f,  0.0f,        -s * w / 2.0f,      // 20: 右面右上（复用顶面3）
		s * w / 2.0f,  0.0f,        s * w / 2.0f,       // 21: 右面左上（复用顶面0）
		s * w / 2.0f,  -thickness,  s * w / 2.0f,       // 22: 右面左下（复用底面4）
		s * w / 2.0f,  -thickness,  -s * w / 2.0f       // 23: 右面右下（复用底面7）
	};
	float normal[] = {
		// -------------------------- 顶面（法向量(0,1,0)）--------------------------
		0.0f, 1.0f, 0.0f,   // 0
		0.0f, 1.0f, 0.0f,   // 1
		0.0f, 1.0f, 0.0f,   // 2
		0.0f, 1.0f, 0.0f,   // 3

		// -------------------------- 底面（法向量(0,-1,0)）--------------------------
		0.0f, -1.0f, 0.0f,  // 4
		0.0f, -1.0f, 0.0f,  // 5
		0.0f, -1.0f, 0.0f,  // 6
		0.0f, -1.0f, 0.0f,  // 7

		// -------------------------- 前面（法向量(0,0,1)）--------------------------
		0.0f, 0.0f, 1.0f,   // 8
		0.0f, 0.0f, 1.0f,   // 9
		0.0f, 0.0f, 1.0f,   // 10
		0.0f, 0.0f, 1.0f,   // 11

		// -------------------------- 左面（法向量(-1,0,0)）--------------------------
		-1.0f, 0.0f, 0.0f,  // 12
		-1.0f, 0.0f, 0.0f,  // 13
		-1.0f, 0.0f, 0.0f,  // 14
		-1.0f, 0.0f, 0.0f,  // 15

		// -------------------------- 后面（法向量(0,0,-1)）--------------------------
		0.0f, 0.0f, -1.0f,  // 16
		0.0f, 0.0f, -1.0f,  // 17
		0.0f, 0.0f, -1.0f,  // 18
		0.0f, 0.0f, -1.0f,  // 19

		// -------------------------- 右面（法向量(1,0,0)）--------------------------
		1.0f, 0.0f, 0.0f,   // 20
		1.0f, 0.0f, 0.0f,   // 21
		1.0f, 0.0f, 0.0f,   // 22
		1.0f, 0.0f, 0.0f    // 23
	};
	float uvs[] = {
		// -------------------------- 顶面（tv2纹理：(0,0),(1,0),(1,1),(0,1)）--------------------------
		0.0f, 0.0f,   // 0
		1.0f, 0.0f,   // 1
		1.0f, 1.0f,   // 2
		0.0f, 1.0f,   // 3

		// -------------------------- 底面（同tv2）--------------------------
		0.0f, 0.0f,   // 4
		1.0f, 0.0f,   // 5
		1.0f, 1.0f,   // 6
		0.0f, 1.0f,   // 7

		// -------------------------- 前面（tv3纹理：(0,0),(1,0),(1,1),(0,1)）--------------------------
		0.0f, 0.0f,   // 8
		1.0f, 0.0f,   // 9
		1.0f, 1.0f,   // 10
		0.0f, 1.0f,   // 11

		// -------------------------- 左面（同tv3）--------------------------
		0.0f, 0.0f,   // 12
		1.0f, 0.0f,   // 13
		1.0f, 1.0f,   // 14
		0.0f, 1.0f,   // 15

		// -------------------------- 后面（同tv3）--------------------------
		0.0f, 0.0f,   // 16
		1.0f, 0.0f,   // 17
		1.0f, 1.0f,   // 18
		0.0f, 1.0f,   // 19

		// -------------------------- 右面（同tv3）--------------------------
		0.0f, 0.0f,   // 20
		1.0f, 0.0f,   // 21
		1.0f, 1.0f,   // 22
		0.0f, 1.0f    // 23
	};
	unsigned int indices[] = {
		// -------------------------- 顶面（2个三角形）--------------------------
		0, 1, 2,    // 顶面三角形1
		0, 2, 3,    // 顶面三角形2

		// -------------------------- 底面（2个三角形）--------------------------
		4, 7, 6,    // 底面三角形1
		4, 6, 5,    // 底面三角形2

		// -------------------------- 前面（2个三角形）--------------------------
		8, 9, 10,   // 前面三角形1
		8, 10, 11,  // 前面三角形2

		// -------------------------- 左面（2个三角形）--------------------------
		12, 13, 14, // 左面三角形1
		12, 14, 15, // 左面三角形2

		// -------------------------- 后面（2个三角形）--------------------------
		16, 17, 18, // 后面三角形1
		16, 18, 19, // 后面三角形2

		// -------------------------- 右面（2个三角形）--------------------------
		20, 21, 22, // 右面三角形1
		20, 22, 23  // 右面三角形2
	};
	geometry->mIndicesCount = 36;
	//posVbo创建
	GLuint& posVbo = geometry->mPosVbo, & uvVbo = geometry->mUvVbo, & normalVbo = geometry->mNormalVbo;

	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	//uvVbo创建
	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);//插槽，传入数据个数，需要传入的数据，数据是否频繁修改
	//normalVbo创建
	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normal), normal, GL_STATIC_DRAW);

	GLuint& ebo = geometry->mEbo, & vao = geometry->mVao;

	//3 VAO创建
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//4 EBO创建
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//动态获取位置
	//GLuint posLocation = glGetAttribLocation(shader->mProgram, "aPos");

	//绑定posVbo
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，属性段长，此属性在vbo中每个点内起始位置

	//绑定uvVbo
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	//绑定normalVbo
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//绑定ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
	return geometry;
}

Geometry* Geometry::creatScreenPlane()
{
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 6;

	//构建数据的position uvs
	float position[] = {
		-1.0f,1.0f,
		-1.0f,-1.0f,
		1.0f,-1.0f,
		1.0f,1.0f
	};
	float uvs[] = {
		0.0f,1.0f,
		0.0f,0.0f,
		1.0f,0.0f,
		1.0f,1.0f
	};
	unsigned int indices[] = {
		0,1,2,
		0,2,3
	};

	GLuint& posVbo = geometry->mPosVbo, & uvVbo = geometry->mUvVbo;

	//posVbo创建
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	//uvVbo创建
	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);//插槽，传入数据个数，需要传入的数据，数据是否频繁修改


	GLuint& ebo = geometry->mEbo, & vao = geometry->mVao;
	
	//4 VAO创建
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//3 EBO创建
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	

	//绑定posVbo
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);//属性存放在的vao编号，属性个数，属性数字类型，是否标准化，属性段长，此属性在vbo中每个点内起始位置

	//绑定uvVbo
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);


	//绑定ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
	return geometry;
}

Geometry* Geometry::creatPlaneWithTBN()
{
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 6;
	// positions
	glm::vec3 pos1(-1.0, 1.0, 0.0);
	glm::vec3 pos2(-1.0, -1.0, 0.0);
	glm::vec3 pos3(1.0, -1.0, 0.0);
	glm::vec3 pos4(1.0, 1.0, 0.0);
	// texture coordinates
	glm::vec2 uv1(0.0, 1.0);
	glm::vec2 uv2(0.0, 0.0);
	glm::vec2 uv3(1.0, 0.0);
	glm::vec2 uv4(1.0, 1.0);
	// normal vector
	glm::vec3 nm(0.0, 0.0, 1.0);

	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;
	// - triangle 1
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = glm::normalize(tangent1);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent1 = glm::normalize(bitangent1);

	// - triangle 2
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent2 = glm::normalize(tangent2);


	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent2 = glm::normalize(bitangent2);


	GLfloat quadVertices[] = {
		// Positions            // normal         // TexCoords  // Tangent                          // Bitangent
		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
	};
	// Setup plane VAO
	glGenVertexArrays(1, &geometry->mVao);
	glGenBuffers(1, &geometry->mPosVbo);
	glBindVertexArray(geometry->mVao);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->mPosVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
	return geometry;
}

GLuint Geometry::getVao() const
{
	return mVao;
}

GLuint Geometry::getType() const
{
	return mType;
}

uint32_t Geometry::getIndicesCount() const
{
	return mIndicesCount;
}
