//
// Created by abeir on 2020/7/29.
//

#include "AbstractCamera.h"
#include "util/utils.h"
#include "util/Shader.h"
#include "util/Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Camera {
    class AbstractCameraPrivate {
    public:
        ~AbstractCameraPrivate();
        unsigned int VBO, VAO;
        Util::Shader *shader;
        // 纹理ID数组
        unsigned int* textureIds;
        // 顶点坐标数组
        float *vertices;
        // 纹理信息数组
        TextureInfo *textureInfos;
        // 立方体位置坐标数组
        CubePositionInfo *cubePositions;
    };

    AbstractCameraPrivate::~AbstractCameraPrivate() {
        delete shader;
        delete []textureIds;
        delete []vertices;
        delete []textureInfos;
        delete []cubePositions;
    }

    AbstractCamera::AbstractCamera() : d(new AbstractCameraPrivate){

    }

    AbstractCamera::~AbstractCamera() {
        delete d;
    }

    void AbstractCamera::preRender() {
        // 启用深度测试
        glEnable(GL_DEPTH_TEST);
        // 编译着色器
        const char* vShaderPath = shaderVertexPath();
        const char* fShaderPath = shaderFragmentPath();
        d->shader = new Util::Shader(vShaderPath, fShaderPath);

        d->cubePositions = cubePositionArray();

        // 顶点和纹理坐标
        d->vertices = vertexArray();

        glGenVertexArrays(1, &(d->VAO));
        glGenBuffers(1, &(d->VBO));

        glBindVertexArray(d->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, d->VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexArrayLength() * sizeof(float), d->vertices, GL_STATIC_DRAW);

        // 位置属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) nullptr);
        glEnableVertexAttribArray(0);
        // 纹理属性
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // 创建纹理
        int textureQuantity = textureInfoQuantity();
        d->textureIds = new unsigned int[textureQuantity];
        d->textureInfos = textureInfo();

        for(int i=0; i<textureQuantity; i++){
            glGenTextures(1, &(d->textureIds[i]));
            glBindTexture(GL_TEXTURE_2D, d->textureIds[i]);
            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // 加载纹理图片
            TextureInfo textureInfo = d->textureInfos[i];
            Util::Texture utilTexture;
            utilTexture.SetFullFilename(textureInfo.filePath);
            switch (textureInfo.format) {
                case RGB:
                    utilTexture.LoadRgb();
                    break;
                case RGBA:
                    utilTexture.LoadRgba();
                    break;
                default:
                    setErrorMessage("texture format invalid");
                    return;
            }
            if(utilTexture.HasError()){
                setErrorMessage(utilTexture.Error());
                return;
            }
        }

        // 告诉opengl每个采样器它属于哪个纹理单元（只需执行一次）
        d->shader->Use();
        for(int i=0; i<textureQuantity; i++){
            std::string argName = "texture";
            argName = argName.append(std::to_string(i+1));
            d->shader->SetInt(argName, i);
        }

        // 创建透视投影矩阵
        ProjectionInfo pInfo = projectionInfo();
        glm::mat4 projection = glm::perspective(glm::radians(pInfo.degrees), pInfo.aspect, pInfo.zNear, pInfo.zFar);
        d->shader->SetMat4("projection", projection);
    }

    void AbstractCamera::render() {
        int textureQuantity = textureInfoQuantity();
        // 绑定纹理
        for(int i=0; i<textureQuantity; i++){
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, d->textureIds[i]);
        }
        // 激活着色器
        d->shader->Use();

        // 摄像机视图变换
        CameraInfo camInfo = cameraInfo();

        glm::mat4 view = glm::mat4(1.0f);       // 创建一个单位矩阵
        view = glm::lookAt(glm::vec3(camInfo.positionX, camInfo.positionY, camInfo.positionZ),
                glm::vec3(camInfo.targetX, camInfo.targetY, camInfo.targetZ),
                glm::vec3(camInfo.upX, camInfo.upY, camInfo.upZ));
        d->shader->SetMat4("view", view);

        // 渲染立方体
        glBindVertexArray(d->VAO);

        int cubeCount = cubeQuantity();
        for(int i=0; i<cubeCount; i++) {
            CubePositionInfo cubePos = d->cubePositions[i];
            // 计算每个对象的模型矩阵，并在绘制之前将其传递给着色器
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(cubePos.x, cubePos.y, cubePos.z));

            CubeRotateInfo cubeRotate = cubeRotateInfo(i);
            model = glm::rotate(model, glm::radians(cubeRotate.angle), glm::vec3(cubeRotate.axisX, cubeRotate.axisY, cubeRotate.axisZ));
            d->shader->SetMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    void AbstractCamera::release() {
        glDeleteVertexArrays(1, &(d->VAO));
        glDeleteBuffers(1, &(d->VBO));
    }

    void AbstractCamera::clearRender() {
        AbstractDraw::clearRender();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    const char *AbstractCamera::shaderVertexPath() {
        return Util::Res::ShaderFile("camera.vs");
    }

    const char *AbstractCamera::shaderFragmentPath() {
        return Util::Res::ShaderFile("camera.fs");
    }

    float* AbstractCamera::vertexArray() {
        int len = vertexArrayLength();
        return new float[len] {
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
    }

    int AbstractCamera::vertexArrayLength() {
        return 5*36;
    }

    TextureInfo *AbstractCamera::textureInfo() {
        int len = textureInfoQuantity();
        return new TextureInfo[len]{
                TextureInfo { Util::Res::TextureFile("container.jpg"), RGB },
                TextureInfo { Util::Res::TextureFile("awesomeface.png"), RGBA },
        };
    }

    int AbstractCamera::textureInfoQuantity() {
        return 2;
    }

    ProjectionInfo AbstractCamera::projectionInfo() {
        return ProjectionInfo {
                45.0f, (float)width() / (float)height(), 0.1f, 100.0f
        };
    }

    CameraInfo AbstractCamera::cameraInfo() {
        float radius = 10.0f;
        double time = glfwGetTime();
        float camX   = sin(time) * radius;
        float camZ   = cos(time) * radius;
        return CameraInfo{
                camX, 0.0f, camZ,
                0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f
        };
    }

    CubePositionInfo *AbstractCamera::cubePositionArray() {
        int len = cubeQuantity();
        return new CubePositionInfo[len] {
                CubePositionInfo{0.0f,  0.0f,  0.0f},
                CubePositionInfo{2.0f,  5.0f, -15.0f},
                CubePositionInfo{-1.5f, -2.2f, -2.5f},
                CubePositionInfo{-3.8f, -2.0f, -12.3f},
                CubePositionInfo{2.4f, -0.4f, -3.5f},
                CubePositionInfo{-1.7f,  3.0f, -7.5f},
                CubePositionInfo{1.3f, -2.0f, -2.5f},
                CubePositionInfo{1.5f,  2.0f, -2.5f},
                CubePositionInfo{1.5f,  0.2f, -1.5f},
                CubePositionInfo{-1.3f,  1.0f, -1.5f}
        };
    }

    int AbstractCamera::cubeQuantity() {
        return 10;
    }

    CubeRotateInfo AbstractCamera::cubeRotateInfo(int cubeIndex) {
        return CubeRotateInfo {
                20.0f * (float)cubeIndex,
                1.0f, 0.3f, 0.5f
        };
    }

}


