#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera() = default;

    void setProjection(float left, float right, float bottom, float top);
    void setPosition(const glm::vec2& position);

    const glm::mat4& getProjection() const { return m_projection; }
    const glm::mat4& getView() const { return m_view; }
    const glm::mat4& getViewProjection() const { return m_viewProjection; }
    const glm::vec2& getPosition() const { return m_position; }

private:
    void updateViewMatrix();
    
    glm::mat4 m_projection = glm::mat4(1.0f);
    glm::mat4 m_view = glm::mat4(1.0f);
    glm::mat4 m_viewProjection = glm::mat4(1.0f);
    glm::vec2 m_position = {0.0f, 0.0f};
};
