//
// Created by abeir on 2020/7/20.
//

#ifndef LEARNOPENGL_TRIANGLE2_H
#define LEARNOPENGL_TRIANGLE2_H

#include "AbstractDraw.h"

namespace Draw {

    /**
     * 多色三角形
     */
    class Triangle2 : public AbstractDraw{
    public:
        void preRender() override;
        void render() override;
        void clear() override;
    private:
        int shaderProgram;
        unsigned int VBO;
        unsigned int VAO;
    };
}

#endif //LEARNOPENGL_TRIANGLE2_H
