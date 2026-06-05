#include "checkError.h"
#include<iostream>
#include<string>
#include<assert.h>

#include<glad/glad.h>
void checkError() {
	GLenum errorCode = glGetError();
	if (errorCode != GL_NO_ERROR) {
		std::string error = "";
		switch (errorCode)
		{
		case GL_INVALID_ENUM:error = "GL_INVALID_ENUM"; break;
		case GL_INVALID_VALUE:error = "GL_INVALID_VALUE";break;
		case GL_INVALID_OPERATION:error = "GL_INVALID_OPERATION";break;
		case GL_OUT_OF_MEMORY:error = "GL_OUT_OF_MEMORY";break;
		default:
			error = "UNKNOWN";
			break;
		}
		std::cout << error << std::endl;
		assert(false);
	}

}
