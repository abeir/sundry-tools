//
// Created by abeir on 2020/7/19.
//

#include "AbstractDraw.h"


namespace Draw {

    const int DEFAULT_WIDTH = 800;
    const int DEFAULT_HEIGHT = 600;

    void defaultFramebufferSizeCallback(GLFWwindow *window, int width, int height);
    void defaultProcessInput(GLFWwindow *window);


    ErrorInfo::ErrorInfo(ErrorInfo *err) {
        if(err != nullptr){
            message = err->message;
        }
    }

    bool ErrorInfo::isEmpty(){
        return message.empty();
    }

    void ErrorInfo::clear(){
        return message.clear();
    }

    const char* ErrorInfo::toData(){
        return message.c_str();
    }

    std::string ErrorInfo::toString(){
        return message;
    }

    ErrorInfo& ErrorInfo::operator=(const char *msg) {
        message = msg;
        return *this;
    }

    ErrorInfo& ErrorInfo::operator=(const std::string &msg) {
        message = msg;
        return *this;
    }

    ErrorInfo& ErrorInfo::operator<<(const char *msg) {
        message.append(msg);
        return *this;
    }

    ErrorInfo& ErrorInfo::operator<<(const std::string &msg) {
        message.append(msg);
        return *this;
    }


    class AbstractDrawPrivate {
    public:
        void setRGBA(float r, float g, float b, float a);

        int major = 3;
        int minor = 3;
        std::string title;
        int width = DEFAULT_WIDTH;
        int height = DEFAULT_HEIGHT;
        float bgR = 0.2f;
        float bgG = 0.3f;
        float bgB = 0.3f;
        float bgA = 1.0f;
        AbstractDraw::ProcessInputFunc processInputFunc = defaultProcessInput;
        AbstractDraw::FramebufferSizeCallbackFunc framebufferSizeCallbackFunc = defaultFramebufferSizeCallback;
        ErrorInfo err;
    };

    void AbstractDrawPrivate::setRGBA(float r, float g, float b, float a) {
        bgR = r;
        bgG = g;
        bgB = b;
        bgA = a;
    }


    Draw::AbstractDraw::AbstractDraw() : d(new AbstractDrawPrivate){
    }

    Draw::AbstractDraw::~AbstractDraw() {
        delete d;
    }

    void AbstractDraw::clearRender() {
        glClearColor(d->bgR, d->bgG, d->bgB, d->bgA);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Draw::AbstractDraw::Draw() {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, d->major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, d->minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //创建窗口
        GLFWwindow* window = glfwCreateWindow(d->width, d->height, d->title.c_str(), nullptr, nullptr);
        if (window == nullptr) {
            d->err = "Failed to create GLFW window";
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);
        //窗口大小改变时，调整视区大小
        glfwSetFramebufferSizeCallback(window, d->framebufferSizeCallbackFunc);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            d->err = "Failed to initialize GLAD";
            return;
        }
        // 预渲染
        preRender();
        if(HasError()){
            return;
        }

        // 主渲染循环
        while (!glfwWindowShouldClose(window)) {
            //输入
            d->processInputFunc(window);
            // 清除渲染
            clearRender();
            // 渲染
            render();

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        release();

        glfwTerminate();
        d->err.clear();
    }

    Draw::AbstractDraw &Draw::AbstractDraw::setTitle(const char *title) {
        d->title = title;
        return *this;
    }

    Draw::AbstractDraw &Draw::AbstractDraw::setWidth(int width) {
        d->width = width;
        return *this;
    }

    Draw::AbstractDraw &Draw::AbstractDraw::setHeight(int height) {
        d->height = height;
        return *this;
    }

    Draw::AbstractDraw &Draw::AbstractDraw::setBackgroundColor(float r, float g, float b, float a) {
        d->setRGBA(r, g, b, a);
        return *this;
    }

    Draw::AbstractDraw &Draw::AbstractDraw::setProcessInput(Draw::AbstractDraw::ProcessInputFunc func) {
        d->processInputFunc = func;
        return *this;
    }

    Draw::AbstractDraw &
    Draw::AbstractDraw::setFramebufferSizeCallback(Draw::AbstractDraw::FramebufferSizeCallbackFunc func) {
        d->framebufferSizeCallbackFunc = func;
        return *this;
    }

    bool Draw::AbstractDraw::HasError() {
        return !d->err.isEmpty();
    }

    const char* Draw::AbstractDraw::ErrorMessage() {
        const char* msg = d->err.toData();
        d->err.clear();
        return msg;
    }

    AbstractDraw &AbstractDraw::setVersion(int major, int minor) {
        d->major = major;
        d->minor = minor;
        return *this;
    }

    ErrorInfo &AbstractDraw::Error() {
        return d->err;
    }

    ErrorInfo &AbstractDraw::setErrorMessage(const char *msg) {
        return d->err = msg;
    }

    int AbstractDraw::width() {
        return d->width;
    }

    int AbstractDraw::height() {
        return d->height;
    }

    void defaultFramebufferSizeCallback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void defaultProcessInput(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
}