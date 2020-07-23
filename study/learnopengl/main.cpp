#include <iostream>
#include "util/utils.h"
#include "Triangle.h"
#include "Triangle2.h"
#include "TextureContainer.h"


int main() {
    const char *resDir = Util::Res::Path();
    std::cout << "PROJECT_RES_DIR: " << resDir << std::endl;
    const char *textureDir = Util::Res::TexturePath();
    std::cout << "PROJECT_TEXTURE_DIR: " << textureDir << std::endl;

//    // 绘制三角形
//    Draw::Triangle2 t;
//    t.Draw();
//    if(t.HasError()){
//        std::cout << t.ErrorMessage() << std::endl;
//    }

    // 绘制纹理
    Texture::TextureContainer t;
    t.Draw();

    return 0;
}
