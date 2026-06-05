#pragma once
#include"core.h"
#include <string>
class Shader {
public:
	Shader(const char* vertexPath,const char* fragmentPath, const char* geometryPath=nullptr);
	~Shader();
	//开始使用Shader
	void begin();

	//结束使用Shader
	void end();

	void setV1F(const std::string& name,float value);

	void setV3F(const std::string& name, float x,float y,float z);

	void setV3F(const std::string& name, float* values);

	void setV3F(const std::string& name, const glm::vec3 values);

	void setInt(const std::string& name, int value);

	void setMat4(const std::string& name, glm::mat4 value);

	void setMat3(const std::string& name, glm::mat4 value);

	void setMat4Array(const std::string& name, glm::mat4* value,unsigned int count);
private:
	//shader program
	//type:COMPILE LINK
	void checkShaderErrors(GLuint target,std::string type);
private:
	GLuint mProgram{ 0 };

};