#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

struct WindowProps
{
    std::string title;
    unsigned int width;
    unsigned int height;
    bool vsync;
};

class Window
{
public:
    using EventCallback = std::function<void()>;
    using ResizeCallback = std::function<void(int, int)>;

    Window(const WindowProps& props);
    ~Window();
    
    void close();

    void onUpdate();
    void setVSync(bool enabled);
    bool isVSync() const;

    unsigned int getWidth() const { return m_props.width; }
    unsigned int getHeight() const { return m_props.height; }

    GLFWwindow* getNativeWindow() const { return m_window; }

    void setCloseCallback(EventCallback callback) { m_closeCallback = callback; }
    void setResizeCallback(ResizeCallback callback) { m_resizeCallback = callback; }

private:
    GLFWwindow* m_window;
    WindowProps m_props;
    EventCallback m_closeCallback;
    ResizeCallback m_resizeCallback;
};
