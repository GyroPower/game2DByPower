#include"shader.h"
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<glad/gl.h>
#include<glm/gtc/type_ptr.hpp>

Shader::Shader(const char* shaderVertexSource, const char* shaderFragmentSource,
	const char* shaderGeometrySource) {

	std::string vertexCode, fragmentCode, geometryCode;

	try {
		std::ifstream vShaderFileSource(shaderVertexSource), fShaderFileSource(shaderFragmentSource);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFileSource.rdbuf();
		fShaderStream << fShaderFileSource.rdbuf();

		vShaderFileSource.close(); fShaderFileSource.close();

		vertexCode = vShaderStream.str(); fragmentCode = fShaderStream.str();

		if (shaderGeometrySource != nullptr) {
			std::ifstream gShaderFileSource(shaderGeometrySource);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFileSource.rdbuf();
			geometryCode = gShaderStream.str();

		}
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER: Failure to read shader files\n";
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	unsigned int sVertex, sFragment, sGeometry;

	sVertex = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(sVertex, 1, &vShaderCode, NULL);
	glCompileShader(sVertex);

	this->checkCompileErrors(sVertex, "VERTEX");

	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fShaderCode, NULL);
	glCompileShader(sFragment);
	this->checkCompileErrors(sFragment, "FRAGMENT");

	if (shaderGeometrySource != nullptr) {
		sGeometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(sGeometry, 1, &gShaderCode, NULL);
		glCompileShader(sGeometry);
		this->checkCompileErrors(sGeometry, "GEOMETRY");
	}
	this->id = glCreateProgram();
	glAttachShader(this->id, sVertex);
	glAttachShader(this->id, sFragment);

	if (shaderGeometrySource != nullptr)
		glAttachShader(this->id, sGeometry);

	glLinkProgram(this->id);
	this->checkCompileErrors(this->id, "PROGRAM");

	glDeleteShader(sVertex);
	glDeleteShader(sFragment);

	if (shaderGeometrySource != nullptr)
		glDeleteShader(sGeometry);

}

Shader& Shader::use() {
	glUseProgram(this->id);
	return *this;
}


void Shader::checkCompileErrors(unsigned int object, std::string type) {
	int success;
	char infoLog[1024];

	if (type != "PROGRAM") {


		glGetShaderiv(object, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- ---------------------------------------------- --"
				<< std::endl;
		}
	}
	else {
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- ---------------------------------------------- --"
				<< std::endl;
		}
	}
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value)const {

	glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value)const {
	int mat4Loc = glGetUniformLocation(this->id, name.c_str());
	glUniformMatrix4fv(mat4Loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string& name, glm::mat3 value)const {
	int mat3Loc = glGetUniformLocation(this->id, name.c_str());
	glUniformMatrix3fv(mat3Loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, glm::vec3 value)const {
	int vec3Loc = glGetUniformLocation(this->id, name.c_str());

	glUniform3f(vec3Loc, value.x, value.y, value.z);
}

void Shader::setVec3(const std::string& name, glm::vec3& value, unsigned int amount) {
	int vec3Loc = glGetUniformLocation(this->id, name.c_str());
	glUniform3fv(vec3Loc, amount, &value[0]);
}

void Shader::setVec4(const std::string& name, glm::vec4 value)const {
	int vec4Loc = glGetUniformLocation(this->id, name.c_str());
	glUniform4f(vec4Loc, value.x, value.y, value.z, value.a);
}

void Shader::setVec2(const std::string& name, glm::vec2 value)const {
	int vec2Loc = glGetUniformLocation(this->id, name.c_str());
	glUniform2f(vec2Loc, value.x, value.y);
}

void Shader::setVec2(const std::string& name, glm::vec2& value, unsigned int amount) {
	int vec2Loc = glGetUniformLocation(this->id, name.c_str());
	glUniform2fv(vec2Loc, amount, &value[0]);
}
