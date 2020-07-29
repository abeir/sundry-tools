//
// Created by abeir on 2020/7/20.
//

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include "util/Shader.h"

#define CHECK_TYPE_VERTEX "VERTEX"
#define CHECK_TYPE_FRAGMENT "FRAGMENT"
#define CHECK_TYPE_PROGRAM "PROGRAM"

using namespace Util;

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        // 读取文件的缓冲内容到数据流中
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        // 转换数据流到string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    } catch (std::ifstream::failure &e) {
        errorMessage = "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
        errorMessage.append(e.what());
        return;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. 编译着色器
    // 顶点着色器
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, CHECK_TYPE_VERTEX);
    // 片段着色器
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, CHECK_TYPE_FRAGMENT);
    // 着色器程序
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, CHECK_TYPE_PROGRAM);
    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() {
    glUseProgram(ID);
}

void Shader::SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

int Shader::GetUniformLocation(const char *name) const {
    return glGetUniformLocation(ID, name);
}

bool Shader::HasError() {
    return !errorMessage.empty();
}

const char* Shader::ErrorMessage() {
    const char* msg = errorMessage.c_str();
    errorMessage.clear();
    return msg;
}

void Shader::checkCompileErrors(unsigned int shader, const std::string &type) {
    int success;
    char infoLog[1024];
    if(type == CHECK_TYPE_PROGRAM){
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            errorMessage = "ERROR::PROGRAM_LINKING_ERROR of type: ";
            errorMessage.append(type);
            errorMessage.append("\n");
            errorMessage.append(infoLog);
        }
    }else{
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            errorMessage = "ERROR::SHADER_COMPILATION_ERROR of type: ";
            errorMessage.append(type);
            errorMessage.append("\n");
            errorMessage.append(infoLog);
        }
    }
}
