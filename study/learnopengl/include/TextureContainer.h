//
// Created by abeir on 2020/7/20.
//

#ifndef LEARNOPENGL_TEXTURECONTAINER_H
#define LEARNOPENGL_TEXTURECONTAINER_H

#include "AbstractDraw.h"

namespace Util {
    class Shader;
}

namespace Texture {
    /**
     * 木箱纹理
     */
    class TextureContainer : public Draw::AbstractDraw{
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

#endif //LEARNOPENGL_TEXTURECONTAINER_H
