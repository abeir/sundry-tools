//
// Created by abeir on 2020/7/29.
//

#ifndef LEARNOPENGL_CAMERACIRCLE_H
#define LEARNOPENGL_CAMERACIRCLE_H

#include "AbstractDraw.h"

namespace Util {
    class Shader;
}

namespace Camera {

    class CameraCircle : public Draw::AbstractDraw{
    public:
        void preRender() override;
        void render() override;
        void release() override;
        void clearRender() override;
    private:
        unsigned int VBO, VAO;
        unsigned int texture1, texture2;
        Util::Shader *shader;
    };
}




#endif //LEARNOPENGL_CAMERACIRCLE_H
