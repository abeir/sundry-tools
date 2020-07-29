#include <iostream>
#include "util/utils.h"
#include "Triangle.h"
#include "Triangle2.h"
#include "TextureContainer.h"
#include "Transformations.h"
#include "CoordinateSystems.h"
#include "CoordinateSystemsDepth.h"

// 绘制三角形2
void triangle2(){
    Draw::Triangle2 t;
    t.Draw();
    if(t.HasError()){
        std::cout << t.ErrorMessage() << std::endl;
    }
}

// 绘制纹理
void textureContainer(){
    Texture::TextureContainer t;
    t.Draw();
}

// 变换
void transformations(){
    Trans::Transformations t;
    t.Draw();
}

// 坐标系
void coordinateSystems(){
    CoordinateSystems::CoordinateSystems c;
    c.Draw();
    if(c.HasError()){
        std::cout << "Error: " << c.ErrorMessage() << std::endl;
    }
}

// 坐标系 深度
void coordinateSystemsDepth(){
    CoordinateSystems::CoordinateSystemsDepth c;
    c.Draw();
    if(c.HasError()){
        std::cout << "Error: " << c.ErrorMessage() << std::endl;
    }
}