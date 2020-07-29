//
// Created by abeir on 2020/7/27.
//

#ifndef LEARNOPENGL_COORDINATESYSTEMSDEPTH_H
#define LEARNOPENGL_COORDINATESYSTEMSDEPTH_H

#include "AbstractDraw.h"

namespace Util {
    class Shader;
}

namespace CoordinateSystems {

    class CoordinateSystemsDepth : public Draw::AbstractDraw {
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




#endif //LEARNOPENGL_COORDINATESYSTEMSDEPTH_H
