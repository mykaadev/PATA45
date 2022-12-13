#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
	unsigned int m_RendererID;
	std::string m_Filepath;
	//std::unordered_map<std::>

public:
	Shader(const std::string& filepath);
	~Shader();


	void Bind() const;
	void Unbind() const;
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);

	//Set Uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
private:

	unsigned int GetUniformLocation(const std::string& name);
};
