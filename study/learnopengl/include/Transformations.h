//
// Created by abeir on 2020/7/23.
//

#ifndef LEARNOPENGL_TRANSFORMATIONS_H
#define LEARNOPENGL_TRANSFORMATIONS_H

#include "AbstractDraw.h"

namespace Util {
    class Shader;
}

namespace Trans {
    /**
     * 变换
     */
    class Transformations : public Draw::AbstractDraw{
    public:
        void preRender() override;
        void render() override;
        void clear() override;
    private:
        unsigned int VBO, VAO, EBO;
        unsigned int texture1, texture2;
        Util::Shader *shader;
    };
}



#endif //LEARNOPENGL_TRANSFORMATIONS_H
