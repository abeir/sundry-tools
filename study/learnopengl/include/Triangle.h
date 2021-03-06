//
// Created by abeir on 2020/7/19.
//

#ifndef LEARNOPENGL_TRIANGLE_H
#define LEARNOPENGL_TRIANGLE_H

#include "AbstractDraw.h"

namespace Draw {

    /**
     * 单色三角形
     */
    class Triangle : public AbstractDraw {
    public:
        void preRender() override;
        void render() override;
        void release() override;

    private:
        int shaderProgram;
        unsigned int VBO;
        unsigned int VAO;
    };
}


#endif //LEARNOPENGL_TRIANGLE_H
