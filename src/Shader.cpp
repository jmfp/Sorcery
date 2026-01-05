#include <engine/Shader.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

std::string ReadShaderFile(const std::string& filePath){
    std::ifstream file(filePath);

    if (!file.is_open()){
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::cout << "Succeeded in opening and reading shader " << filePath << std::endl;
    return buffer.str();
}

Shader::Shader(std::string vertexShader, std::string fragmentShader){
    // Shader
    std::string vertexShaderSource = ReadShaderFile(vertexShader);
    const char * sourceCStr = vertexShaderSource.c_str();

    GLuint vertexShaderProgram = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShaderProgram, 1, &sourceCStr, NULL);
    glCompileShader(vertexShaderProgram);

    std::string fragmentShaderSource = ReadShaderFile(fragmentShader);
    const char * sourceCStrFrag = fragmentShaderSource.c_str();

    GLuint fragmentShaderProgram = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderProgram, 1, &sourceCStrFrag, NULL);
    glCompileShader(fragmentShaderProgram);
    // Shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShaderProgram);
    glAttachShader(ID, fragmentShaderProgram);
    glLinkProgram(ID);

    glDeleteShader(fragmentShaderProgram);
    glDeleteShader(vertexShaderProgram);
};

void Shader::Use(){
    glUseProgram(ID);
}

void Shader::Draw(){
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Shader::SetBool(const std::string &name, bool value)
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

void Shader::SetInt(const std::string &name, int value)
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader::SetFloat(const std::string &name, float value)
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

Shader::~Shader(){
    // glDeleteShader(vertexShaderProgram);
    // glDeleteShader(fragmentShaderProgram);
}