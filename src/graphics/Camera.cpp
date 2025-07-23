#include "Camera.h"

void Camera::setProjection(float left, float right, float bottom, float top)
{
    m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    updateViewMatrix();
}

void Camera::setPosition(const glm::vec2& position)
{
    m_position = position;
    updateViewMatrix();
}

void Camera::updateViewMatrix()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_position, 0.0f));
    m_view = glm::inverse(transform);
    m_viewProjection = m_projection * m_view;
}
