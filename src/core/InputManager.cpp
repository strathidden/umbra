#include "InputManager.h"
#include "Logger.h"

GLFWwindow* InputManager::s_window = nullptr;
std::unordered_map<int, KeyState> InputManager::s_keyStates;
std::unordered_map<std::string, int> InputManager::s_keyMap;
glm::vec2 InputManager::s_mousePosition = {0.0f, 0.0f};
float InputManager::s_mouseScroll = 0.0f;
float InputManager::s_controllerDeadzone = 0.15f;
GLFWgamepadstate InputManager::s_controllerState;

void InputManager::init(GLFWwindow* window)
{
    s_window = window;

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    for (int key = 32; key <= 348; key++)
    {
        s_keyStates[key] = KeyState::RELEASED;
    }

    s_keyMap["MoveLeft"] = GLFW_KEY_A;
    s_keyMap["MoveRight"] = GLFW_KEY_D;
    s_keyMap["Jump"] = GLFW_KEY_SPACE;
    s_keyMap["Dash"] = GLFW_KEY_K;
    s_keyMap["Attack"] = GLFW_KEY_J;

    Logger::info("InputManager initialized");
}

void InputManager::update()
{
    for (auto& [key, state] : s_keyStates)
    {
        if (state == KeyState::JUST_PRESSED)
        {
            state = KeyState::PRESSED;
        }
        else if (state == KeyState::JUST_RELEASED)
        {
            state = KeyState::RELEASED;
        }
    }

    s_mouseScroll = 0.0f;

    glfwGetGamepadState(GLFW_JOYSTICK_1, &s_controllerState);

    glfwPollEvents();
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key < 0) return;

    if (action == GLFW_PRESS)
    {
        s_keyStates[key] = KeyState::JUST_PRESSED;
    }
    else if (action == GLFW_RELEASE)
    {
        s_keyStates[key] = KeyState::JUST_RELEASED;
    }
}

void InputManager::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    s_mousePosition = {static_cast<float>(xpos), static_cast<float>(ypos)};
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    s_mouseScroll = static_cast<float>(yoffset);
}

KeyState InputManager::getKey(int key)
{
    auto it = s_keyStates.find(key);
    return (it != s_keyStates.end()) ? it->second : KeyState::RELEASED;
}

bool InputManager::isKeyPressed(int key)
{
    auto state = getKey(key);
    return state == KeyState::PRESSED || state == KeyState::JUST_PRESSED;
}

bool InputManager::isKeyJustPressed(int key)
{
    return getKey(key) == KeyState::JUST_PRESSED;
}

glm::vec2 InputManager::getMousePosition()
{
    return s_mousePosition;
}

bool InputManager::isMouseButtonPressed(int button)
{
    return glfwGetMouseButton(s_window, button) == GLFW_PRESS;
}

float InputManager::getMouseScroll()
{
    return s_mouseScroll;
}

void InputManager::setKeyMapping(const std::string& name, int key)
{
    s_keyMap[name] = key;
}

int InputManager::getMappedKey(const std::string& name)
{
    auto it = s_keyMap.find(name);
    return (it != s_keyMap.end()) ? it->second : -1;
}

bool InputManager::isControllerConnected(int controller)
{
    return glfwJoystickIsGamepad(controller);
}

bool InputManager::isControllerButtonPressed(int button, int controller)
{
    if (!isControllerConnected(controller)) return false;
    return s_controllerState.buttons[button] == GLFW_PRESS;
}

float InputManager::getControllerAxis(int axis, int controller)
{
    if (!isControllerConnected(controller)) return 0.0f;
    
    float value = s_controllerState.axes[axis];
    return std::abs(value) > s_controllerDeadzone ? value : 0.0f;
}
