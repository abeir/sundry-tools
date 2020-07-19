#include <iostream>
#include "Triangle.h"

int main() {

    Draw::Triangle t;
    t.Draw();
    if(t.HasError()){
        std::cout << t.ErrorMessage() << std::endl;
    }

    return 0;
}
