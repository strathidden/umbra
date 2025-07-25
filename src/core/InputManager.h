#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include <string>

enum class KeyState { RELEASED, PRESSED, JUST_PRESSED, JUST_RELEASED };

enum class ControllerButton
{
    CONTROLLER_A,
    CONTROLLER_B,
    CONTROLLER_X,
    CONTROLLER_Y,
    CONTROLLER_LEFT_BUMPER,
    CONTROLLER_RIGHT_BUMPER,
    CONTROLLER_BACK,
    CONTROLLER_START,
    CONTROLLER_LEFT_STICK,
    CONTROLLER_RIGHT_STICK,
    CONTROLLER_DPAD_UP,
    CONTROLLER_DPAD_RIGHT,
    CONTROLLER_DPAD_DOWN,
    CONTROLLER_DPAD_LEFT
};

enum class ControllerAxis
{
    CONTROLLER_AXIS_LEFT_X,
    CONTROLLER_AXIS_LEFT_Y,
    CONTROLLER_AXIS_RIGHT_X,
    CONTROLLER_AXIS_RIGHT_Y,
    CONTROLLER_AXIS_LEFT_TRIGGER,
    CONTROLLER_AXIS_RIGHT_TRIGGER
};

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

    static bool isControllerConnected(int controller = 0);
    static bool isControllerButtonPressed(int button, int controller = 0);
    static float getControllerAxis(int axis, int controller = 0);
    static void setControllerDeadzone(float deadzone) { s_controllerDeadzone = deadzone; }

private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    static GLFWwindow* s_window;
    static std::unordered_map<int, KeyState> s_keyStates;
    static std::unordered_map<std::string, int> s_keyMap;
    static glm::vec2 s_mousePosition;
    static float s_mouseScroll;

    static float s_controllerDeadzone;
    static GLFWgamepadstate s_controllerState;
};
