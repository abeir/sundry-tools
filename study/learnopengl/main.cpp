#include "main.h"


int main() {
    const char *resDir = Util::Res::Path();
    std::cout << "PROJECT_RES_DIR: " << resDir << std::endl;
    const char *textureDir = Util::Res::TexturePath();
    std::cout << "PROJECT_TEXTURE_DIR: " << textureDir << std::endl;

//    // 绘制三角形
//    triangle2();
//    // 绘制纹理
//    textureContainer();
//    // 变换
//    transformations();
//    // 坐标系
//    coordinateSystems();
    // 坐标系 深度
    coordinateSystemsDepth();

    return 0;
}
