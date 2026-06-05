#include "shader.h"
#include "shader.h"
#include"../wrapper/checkError.h"
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	//声明装入Shader代码字符串的两个string
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	//声明用于读取vs和fs文件的inFileStream
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	//保证ifstream遇到问题的时候可以抛出异常
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		//将文件输入流当中的字符串输入到stringStream里面
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//关闭文件
		vShaderFile.close();
		fShaderFile.close();
		//将字符串从stringStream当中读取出来，转化到code String当中
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR :SHADER FILE ERROR:" << e.what() << std::endl;
	}

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();
	//创建Shader程序（vs、fs）
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	//为shader程序输入shader代码
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);//shader,字符串数组有多少个字符串，要输入的n个字符串头指针，这n字符串的每个字符串长度
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	//执行shader代码编译
	glCompileShader(vertex);
	//查看是否正确编译vertex
	checkShaderErrors(vertex,"COMPILE");


	glCompileShader(fragment);
	//查看是否正确编译fragment
	checkShaderErrors(fragment, "COMPILE");

	GLuint geometry;
	if (geometryPath != nullptr)
	{
		const char* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkShaderErrors(geometry, "COMPILE");
	}

	//创建一个program壳子
	mProgram = glCreateProgram();


	//将编译好的vs和fs结果放到program这个壳子里
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);
	if (geometryPath != nullptr)
		glAttachShader(mProgram, geometry);

	//执行program链接操作，形成最终可执行的shader程序
	glLinkProgram(mProgram);
	//检查链接错误
	checkShaderErrors(mProgram, "LINK");

	//清理
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
}
Shader::~Shader() 
{

}

void Shader::begin() 
{
	GL_CALL(glUseProgram(mProgram);)
}

void Shader::end() 
{
	GL_CALL(glUseProgram(0);)
}

void Shader::setV1F(const std::string& name, float value)
{
	//通过名称拿到Uniform变量的位置Location
	GLint location = glGetUniformLocation(mProgram, name.c_str());
	
	//通过Location更新Uniform变量的值
	glUniform1f(location, value);
}

void Shader::setV3F(const std::string& name, float x, float y, float z)
{

	GLint location = glGetUniformLocation(mProgram, name.c_str());
	glUniform3f(location,x,y,z);
}

void Shader::setV3F(const std::string& name, float* values)
{
	GLint location = glGetUniformLocation(mProgram, name.c_str());
	glUniform3fv(location,1,values);//第2个参数：你当前要更新的Uniform变量如果是数组，数组里面包括多少个向量vec3
}

void Shader::setV3F(const std::string& name, const glm::vec3 values)
{
	GLint location = glGetUniformLocation(mProgram, name.c_str());
	glUniform3f(location, values.x, values.y, values.z);
}


void Shader::setInt(const std::string& name, int value)
{
	GLint location = glGetUniformLocation(mProgram, name.c_str());
	glUniform1i(location,  value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value)
{
	GLint location = glGetUniformLocation(mProgram, name.c_str());
	//transpose参数：表示是否对传输进去的矩阵数据进行转置，opengl和glm都是列存储,所以不需要转置。最后一个元素为开头的指针。  带v的都是传数组
	glUniformMatrix4fv(location,1,GL_FALSE,glm::value_ptr(value));
}

void Shader::setMat3(const std::string& name, glm::mat4 value)
{	
	//1 通过名称拿到Uniform变量的位置Location
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));

	//2 通过Location更新Uniform变量的值
	//transpose参数：表示是否对传输进去的矩阵数据进行转置
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4Array(const std::string& name, glm::mat4* value, unsigned int count)
{
	GLint location = glGetUniformLocation(mProgram, name.c_str());
	//transpose参数：表示是否对传输进去的矩阵数据进行转置，opengl和glm都是列存储,所以不需要转置。  带v的都是传数组
	glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(value[0] ));
}

void Shader::checkShaderErrors(GLuint target, std::string type)
{
	int success = 0;
	char infoLog[1024];
	if (type == "COMPILE") {
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(target, 1024, NULL, infoLog);
			std::cout << "Error:SHADER COMPLIE ERROP" << "\n" << infoLog << std::endl;
		}
	}
	else if (type == "LINK") {
		glGetProgramiv(target, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(target, 1024, NULL, infoLog);
			std::cout << "Error:SHADER LINK ERROP " << "\n" << infoLog << std::endl;
		}
	}
	else {
		std::cout << "Error:Check shader errors Type is wrong" << std::endl;
	}
}
