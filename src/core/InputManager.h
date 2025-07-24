#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include <string>

enum class KeyState { RELEASED, PRESSED, JUST_PRESSED, JUST_RELEASED };

class InputManager
{
public:
    static void init(GLFWwindow* window);
    static void update();

    static KeyState getKey(int key);
    static bool isKeyPressed(int key);
    static bool isKeyJustPressed(int key);

    static glm::vec2 getMousePosition();
    static bool isMouseButtonPressed(int button);
    static float getMouseScroll();

    static void setKeyMapping(const std::string& name, int key);
    static int getMappedKey(const std::string& name);

private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    static GLFWwindow* s_window;
    static std::unordered_map<int, KeyState> s_keyStates;
    static std::unordered_map<std::string, int> s_keyMap;
    static glm::vec2 s_mousePosition;
    static float s_mouseScroll;
};
