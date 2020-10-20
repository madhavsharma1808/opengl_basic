#include<ctime>
#include<fstream>
#include<string>
#include<sstream>
#include<iostream>
#include "Render.h"
#include "Shader.h"




Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
	{
    
    ShaderProgramSource source = ParseShader(filepath);
   // std::cout << source.VertexSource << source.FragmentSource; 
		m_RendererID= CreateShader(source.VertexSource, source.FragmentSource);
	}
 unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    //deleting bcz once attached to the progrma they are not needed
    return program;
}
     unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); //copies the src code into shader object
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); //will ewruen the compile status
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}
   
     ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    ShaderType type = ShaderType::NONE;
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];

    while (getline(stream, line))
    {
        if (line.find("shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }

        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(),ss[1].str() };
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetSetUniform(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0,v1,v2,v3));
}

void Shader::SetSetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    GLCall(unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "warning: uniform " << name << "doesn't exist" << std::endl;
        m_UniformLocationCache[name] = location;
    return location;
}
