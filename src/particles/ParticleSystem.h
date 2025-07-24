#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Particle
{
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color;
    float size;
    float life;
    float rotation;
    float rotationSpeed;
};

struct EmitterConfig
{
    glm::vec2 position;
    glm::vec2 velocityMin = {-50, -50};
    glm::vec2 velocityMax = {50, 50};
    glm::vec4 colorStart = {1, 1, 1, 1};
    glm::vec4 colorEnd = {1, 1, 1, 0};
    float sizeStart = 10.0f;
    float sizeEnd = 0.0f;
    float lifeMin = 1.0f;
    float lifeMax = 3.0f;
    float emissionRate = 20.0f;
    float rotationSpeedMin = -180.0f;
    float rotationSpeedMax = 180.0f;
};

class ParticleSystem
{
public:
    ParticleSystem();
    void update(float deltaTime);
    void render();

    void emit(const EmitterConfig& config, int count = 1);
    void setPosition(const glm::vec2& pos) { m_position = pos; }
    void setActive(bool active) { m_active = active; }

private:
    std::vector<Particle> m_particles;
    glm::vec2 m_position;
    float m_emmisionCounter = 0.0f;
    EmitterConfig m_config;
    bool m_active = true;
};
