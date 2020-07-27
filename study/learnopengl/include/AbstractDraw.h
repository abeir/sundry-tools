//
// Created by abeir on 2020/7/19.
//

#ifndef LEARNOPENGL_ABSTRACTDRAW_H
#define LEARNOPENGL_ABSTRACTDRAW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Draw {

    class AbstractDrawPrivate;

    class ErrorInfo {
    public:
        ErrorInfo() = default;
        explicit ErrorInfo(ErrorInfo *err);

        bool isEmpty();
        void clear();
        const char* toData();
        std::string toString();

        ErrorInfo& operator=(const char *msg);
        ErrorInfo& operator=(const std::string &msg);
        ErrorInfo& operator<<(const char *msg);
        ErrorInfo& operator<<(const std::string &msg);
    private:
        std::string message;
    };

    class AbstractDraw {
    public:
        typedef void (*ProcessInputFunc)(GLFWwindow*);
        typedef void (*FramebufferSizeCallbackFunc)(GLFWwindow*, int, int);

        AbstractDraw();
        virtual ~AbstractDraw();

        virtual void preRender() =0;
        virtual void render() =0;
        virtual void clear() =0;

        void Draw();

        AbstractDraw& setVersion(int major, int minor);
        AbstractDraw& setTitle(const char *title);
        AbstractDraw& setWidth(int width);
        AbstractDraw& setHeight(int height);
        AbstractDraw& setBackgroundColor(float r, float g, float b, float a);

        AbstractDraw& setProcessInput(ProcessInputFunc func);
        AbstractDraw& setFramebufferSizeCallback(FramebufferSizeCallbackFunc func);

        int width();
        int height();

        ErrorInfo& setErrorMessage(const char* msg);

        bool HasError();
        const char* ErrorMessage();

    protected:
        ErrorInfo& Error();
    private:
        AbstractDrawPrivate *d;
    };
}

#endif //LEARNOPENGL_ABSTRACTDRAW_H
