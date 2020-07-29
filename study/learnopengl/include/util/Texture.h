//
// Created by abeir on 2020/7/27.
//

#ifndef LEARNOPENGL_TEXTURE_H
#define LEARNOPENGL_TEXTURE_H

namespace Util {
    /**
     * 加载纹理图片
     */
    class Texture{
    public:
        Texture() = default;
        explicit Texture(const char* shortFilename);
        virtual ~Texture();

        void SetFullFilename(const char* fullFilename);

        [[nodiscard]] int GetHeight() const;
        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetChannelsInFile() const;

        void LoadRgb(bool flipVertically = true);
        void LoadRgba(bool flipVertically = true);

        bool HasError();
        const char* Error();
    private:
        void setError(const char*);

        const char* m_filename{};
        int m_height{};
        int m_width{};
        int m_channels{};
        char* m_err = nullptr;
    };
}

#endif //LEARNOPENGL_TEXTURE_H
