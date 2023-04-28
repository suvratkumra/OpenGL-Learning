#pragma once

#include <string>

struct ShaderSourceString
{
	std::string vertex_source;
	std::string fragment_source;
};

class Shader
{
private:
	unsigned int renderer_id;

public:
	unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderSourceString ParseShader(std::string path);

};

