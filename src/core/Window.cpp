#include "Window.h"
#include "Logger.h"

Window::Window(const WindowProps& props)
    : m_window(nullptr)
    , m_props(props)
{
    if (!glfwInit())
    {
        Logger::error("GLFW initialization failed!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    m_window = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), props.title.c_str(), nullptr, nullptr);

    if (!m_window)
    {
        Logger::error("Window creation failed!");
        glfwTerminate();
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::error("Failed to initialize GLAD");
        glfwTerminate();
    }

    Logger::info("OpenGL version: ", glGetString(GL_VERSION));

    setVSync(props.vsync);

    glfwSetWindowUserPointer(m_window, this);

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win->m_closeCallback) win->m_closeCallback();
    });

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        win->m_props.width = width;
        win->m_props.height = height;
        if (win->m_resizeCallback) win->m_resizeCallback();
    });

    Logger::info("Window created: ", props.title, " (", props.width, "x", props.height, ")");
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::onUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(m_window);
}

void Window::setVSync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
    m_props.vsync = enabled;
}

bool Window::isVSync() const
{
    return m_props.vsync;
}
