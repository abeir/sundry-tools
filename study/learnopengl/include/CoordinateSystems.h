//
// Created by abeir on 2020/7/27.
//

#ifndef LEARNOPENGL_COORDINATESYSTEMS_H
#define LEARNOPENGL_COORDINATESYSTEMS_H

#include "AbstractDraw.h"

namespace Util {
    class Shader;
}

namespace CoordinateSystems {
    /**
     * 坐标系-深度
     */
    class CoordinateSystems : public Draw::AbstractDraw{
    public:
        void preRender() override;
        void render() override;
        void release() override;
    private:
        unsigned int VBO, VAO, EBO;
        unsigned int texture1, texture2;
        Util::Shader *shader;
    };
}

#endif //LEARNOPENGL_COORDINATESYSTEMS_H
