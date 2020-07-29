//
// Created by abeir on 2020/7/20.
//

#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <glm/glm.hpp>
#include <iostream>

namespace Util {

    class Shader {
    public:
        // 构造器读取并构建着色器
        Shader(const char* vertexPath, const char* fragmentPath);

        // 使用/激活程序
        void Use();
        // uniform工具函数
        void SetBool(const std::string &name, bool value) const;
        void SetInt(const std::string &name, int value) const;
        void SetFloat(const std::string &name, float value) const;
        void SetMat4(const std::string &name, const glm::mat4 &mat) const;
        // 获取uniform位置
        int GetUniformLocation(const char* name) const;

        bool HasError();
        const char* ErrorMessage();
    private:
        void checkCompileErrors(unsigned int shader, const std::string &type);

        unsigned int ID;
        std::string errorMessage;
    };
}

#endif //LEARNOPENGL_SHADER_H
