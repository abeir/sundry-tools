//
// Created by abeir on 2020/7/29.
//

#ifndef LEARNOPENGL_ABSTRACTCAMERA_H
#define LEARNOPENGL_ABSTRACTCAMERA_H

#include "AbstractDraw.h"

namespace Camera {

    class AbstractCameraPrivate;
    // 纹理格式
    enum TextureFormat {RGB, RGBA};
    // 纹理
    struct TextureInfo {
        const char* filePath;
        TextureFormat format;
    };
    // 透视投影
    struct ProjectionInfo {
        float degrees;  // 角度
        float aspect;   // 切面
        float zNear;    // z轴近点
        float zFar;     // z轴远点
    };
    // 摄像机信息
    struct CameraInfo {
        float positionX, positionY, positionZ;     // 位置坐标
        float targetX, targetY, targetZ;           // 目标坐标
        float upX, upY, upZ;                       // 上向量
    };
    // 立方体位置坐标
    struct CubePositionInfo {
        float x, y, z;
    };
    // 立方体旋转信息
    struct CubeRotateInfo {
        float angle;                // 角度
        float axisX, axisY, axisZ;  // 旋转轴
    };


    class AbstractCamera : public Draw::AbstractDraw{
    public:
        AbstractCamera();
        ~AbstractCamera() override;

        void preRender() override;
        void render() override;
        void release() override;
        void clearRender() override;

        // 设置顶点着色器文件路径
        virtual const char* shaderVertexPath();
        // 设置片段着色器文件路径
        virtual const char* shaderFragmentPath();
        // 设置顶点坐标数组，返回float数组
        virtual float* vertexArray();
        // 顶点坐标数组长度
        virtual int vertexArrayLength();
        // 设置纹理信息，返回纹理信息结构体数组
        virtual TextureInfo* textureInfo();
        // 纹理数量
        virtual int textureInfoQuantity();
        // 设置透视投影信息
        virtual ProjectionInfo projectionInfo();
        // 设置摄像机变换信息
        virtual CameraInfo cameraInfo();
        // 设置立方体位置坐标数组
        virtual CubePositionInfo* cubePositionArray();
        // 立方体数量
        virtual int cubeQuantity();
        // 设置立方体旋转，cubeIndex: 立方体下标
        virtual CubeRotateInfo cubeRotateInfo(int cubeIndex);

    private:
        AbstractCameraPrivate *d;
    };
}

#endif //LEARNOPENGL_ABSTRACTCAMERA_H
