//
// Created by abeir on 2020/7/27.
//
#include "util/Texture.h"
#include "util/stb_image.h"
#include "util/utils.h"
#include <glad/glad.h>
#include <cstring>

namespace Util {


    Texture::Texture(const char *shortFilename)
            : m_filename(Util::Res::TextureFile(shortFilename)) {
        m_height = 0;
        m_width = 0;
        m_channels = 0;
    }

    Texture::~Texture() = default;

    int Texture::GetHeight() const {
        return m_height;
    }

    int Texture::GetWidth() const {
        return m_width;
    }

    int Texture::GetChannelsInFile() const {
        return m_channels;
    }

    void Texture::LoadRgb(bool flipVertically) {
        //告诉stb_image.h在y轴上翻转加载的纹理。
        stbi_set_flip_vertically_on_load(flipVertically);
        unsigned char *data = stbi_load(m_filename, &m_width, &m_height, &m_channels, 0);
        if(data){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }else{
            setError("Failed to load rgb texture");
            return;
        }
        stbi_image_free(data);
    }

    void Texture::LoadRgba(bool flipVertically) {
        //告诉stb_image.h在y轴上翻转加载的纹理。
        stbi_set_flip_vertically_on_load(flipVertically);
        unsigned char *data = stbi_load(m_filename, &m_width, &m_height, &m_channels, 0);
        if(data){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }else{
            setError("Failed to load rgba texture");
            return;
        }
        stbi_image_free(data);
    }

    bool Texture::HasError() {
        return m_err != nullptr && std::strlen(m_err) != 0;
    }

    const char *Texture::Error() {
        return m_err;
    }

    void Texture::setError(const char* err) {
        delete []m_err;
        if(err == nullptr){
            return;
        }
        m_err = new char[std::strlen(err)+1];
        std::strcpy(m_err, err);
    }

    void Texture::SetFullFilename(const char *fullFilename) {
        m_filename = fullFilename;
    }
}
