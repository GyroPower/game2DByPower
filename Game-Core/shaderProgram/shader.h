#pragma once
#include<glm/glm.hpp>
#include<string>

class Shader {
private:
	unsigned int id;

public:
	Shader() {};
	Shader(const char* shaderVertexSource, const char* shaderFragmentSource,
		const char* shaderGeometrySource = nullptr);

	Shader& use();

	void checkCompileErrors(unsigned int object, std::string type);

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	void setMat4(const std::string& name, glm::mat4 value) const;
	void setMat3(const std::string& name, glm::mat3 value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec3(const std::string& name, glm::vec3& value, unsigned int amount);
	void setVec2(const std::string& name, glm::vec2 value) const;
	void setVec2(const std::string& name, glm::vec2& value, unsigned int amount);
	void setVec4(const std::string& name, glm::vec4 value) const;


};