//
// Created by abeir on 2020/7/27.
//

#include "CoordinateSystems.h"
#include "util/utils.h"
#include "util/Shader.h"
#include "util/Texture.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace CoordinateSystems {

    void CoordinateSystems::preRender() {
        // 编译着色器
        const char* vShaderPath = Util::Res::ShaderFile("coordinate_systems.vs");
        const char* fShaderPath = Util::Res::ShaderFile("coordinate_systems.fs");
        shader = new Util::Shader(vShaderPath, fShaderPath);

        // 顶点和纹理坐标
        float vertices[] = {
                // positions          // texture coords
                0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
                0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
                -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left
        };
        // 顶点索引
        unsigned int indices[] = {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // 位置属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) nullptr);
        glEnableVertexAttribArray(0);
        // 纹理坐标属性
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // 创建纹理
        // 纹理1
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 加载纹理1图片
        Util::Texture utilTexture1("container.jpg");
        utilTexture1.LoadRgb();
        if(utilTexture1.HasError()){
            setErrorMessage(utilTexture1.Error());
            return;
        }

        // 纹理2
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 加载纹理2图片
        Util::Texture utilTexture2("awesomeface.png");
        utilTexture2.LoadRgba();
        if(utilTexture2.HasError()){
            setErrorMessage(utilTexture2.Error());
            return;
        }

        // 告诉opengl每个采样器它属于哪个纹理单元（只需执行一次）
        shader->Use();
        shader->SetInt("texture1", 0);
        shader->SetInt("texture2", 1);
    }

    void CoordinateSystems::render() {
        // 绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // 激活着色器
        shader->Use();

        // 创建转换
        glm::mat4 model         = glm::mat4(1.0f); // 确保初始的为单元矩阵
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        // 沿x轴旋转
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // 沿z轴位移
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // 创建透视投影矩阵
        projection = glm::perspective(glm::radians(45.0f), (float)width() / (float)height(), 0.1f, 100.0f);
        // 获取uniform参数位置
        unsigned int modelLoc = shader->GetUniformLocation("model");
        unsigned int viewLoc  = shader->GetUniformLocation("view");

        // 下面使用三种方式设置着色器参数
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // 注意：目前我们在每一帧设置投影矩阵，但是由于投影矩阵很少更改，所以最好的做法是只在主循环之外设置一次。
        shader->SetMat4("projection", projection);

        // 渲染
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void CoordinateSystems::release() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        delete shader;
    }
}