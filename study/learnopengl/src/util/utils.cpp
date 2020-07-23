//
// Created by abeir on 2020/7/20.
//
#include <cstring>
#include "util/utils.h"

namespace Util{

    const char PATH_SEQ_CHAR = '/';

    bool StartsWith(const char* str, const char &c) {
        size_t len = strlen(str);
        if(len == 0){
            return false;
        }
        return str[0] == c;
    }

    bool StartsWith(const std::string &str, const char &c) {
        size_t len = str.length();
        if(len == 0){
            return false;
        }
        return str.at(0) == c;
    }

    bool EndsWith(const char* str, const char &c) {
        size_t len = strlen(str);
        if(len == 0){
            return false;
        }
        return str[len - 1] == c;
    }

    bool EndsWith(const std::string &str, const char &c) {
        size_t len = str.length();
        if(len == 0){
            return false;
        }
        return str.at(len - 1) == c;
    }

    const char* JoinPath(const char* prefix, const char* suffix) {
        return JoinPath(std::string(prefix), std::string(suffix));
    }

    const char* JoinPath(const std::string &prefix, const std::string &suffix) {
        std::string newPath;

        if(!prefix.empty()){
            if(EndsWith(prefix, PATH_SEQ_CHAR)){
                newPath.append(prefix);
            }else {
                newPath.append(prefix);
                newPath.push_back(PATH_SEQ_CHAR);
            }
        }
        if(!suffix.empty()){
            if(StartsWith(suffix, PATH_SEQ_CHAR)){
                newPath.append(suffix.substr(1));
            }else {
                newPath.append(suffix);
            }
        }
        char *path = new char[newPath.length() + 1];
        strcpy(path, newPath.c_str());
        return path;
    }

    namespace Res {
        const char* Path() {
#ifdef PROJECT_RES_DIR
            return PROJECT_RES_DIR;
#else
            return "";
#endif
        }

        const char* TexturePath() {
            std::string path = std::string(Path());
            if(path.empty()){
                return "";
            }
            return JoinPath(path, "texture");
        }

        const char* TextureFile(const char* filename) {
            const char* path = TexturePath();
            return JoinPath(path, filename);
        }

        const char* ShaderPath() {
            std::string path = std::string(Path());
            if(path.empty()){
                return "";
            }
            return JoinPath(path, "shader");
        }

        const char* ShaderFile(const char* filename) {
            const char* path = ShaderPath();
            return JoinPath(path, filename);
        }
    }

}
