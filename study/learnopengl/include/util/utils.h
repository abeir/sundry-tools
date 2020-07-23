//
// Created by abeir on 2020/7/20.
//

#ifndef LEARNOPENGL_UTILS_H
#define LEARNOPENGL_UTILS_H

#include <string>

namespace Util {

    bool StartsWith(const char* str, const char &c);

    bool StartsWith(const std::string &str, const char &c);

    bool EndsWith(const char* str, const char &c);

    bool EndsWith(const std::string &str, const char &c);

    const char* JoinPath(const char* prefix, const char* suffix);

    const char* JoinPath(const std::string &prefix, const std::string &suffix);

    namespace Res {
        /**
         * 项目源码中的res目录路径
         * @return
         */
        const char* Path();
        /**
         * 项目源码中的texture目录路径
         * @return
         */
        const char* TexturePath();
        /**
         * 项目源码中的指定texture文件路径
         * @param filename
         * @return
         */
        const char* TextureFile(const char* filename);
        /**
         * 项目源码中的shader目录路径
         * @return
         */
        const char* ShaderPath();
        /**
         * 项目源码中的指定shader文件路径
         * @param filename
         * @return
         */
        const char* ShaderFile(const char* filename);
    }

}


#endif //LEARNOPENGL_UTILS_H
