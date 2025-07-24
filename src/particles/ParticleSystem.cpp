#include "ParticleSystem.h"
#include "../core/Timer.h"
#include "../graphics/Renderer.h"
#include <random>

ParticleSystem::ParticleSystem()
{
    m_particles.reserve(1000);
}

void ParticleSystem::update(float deltaTime)
{
    for (auto it = m_particles.begin(); it != m_particles.end();)
    {
        it->position += it->velocity * deltaTime;
        it->life -= deltaTime;
        it->rotation += it->rotationSpeed * deltaTime;

        if (it->life <= 0.0f)
        {
            it = m_particles.erase(it);
        }
        else
        {
            float lifeRatio = 1.0f - (it->life / (m_config.lifeMax - m_config.lifeMin));
            it->color = glm::mix(m_config.colorStart, m_config.colorEnd, lifeRatio);
            it->size = glm::mix(m_config.sizeStart, m_config.sizeEnd, lifeRatio);
            ++it;
        }
    }

    if (m_active)
    {
        m_emmisionCounter += m_config.emissionRate * deltaTime;
        while (m_emmisionCounter >= 1.0f)
        {
            emit(m_config, 1);
            m_emmisionCounter -= 1.0f;
        }
    }
}

void ParticleSystem::emit(const EmitterConfig& config, int count)
{
    static std::mt19937 rng(std::random_device{}());

    std::uniform_real_distribution<float> velXDist(config.velocityMin.x, config.velocityMax.x);
    std::uniform_real_distribution<float> velYDist(config.velocityMin.y, config.velocityMax.y);
    std::uniform_real_distribution<float> lifeDist(config.lifeMin, config.lifeMax);
    std::uniform_real_distribution<float> rotDist(config.rotationSpeedMin, config.rotationSpeedMax);

    for (int i = 0; i < count; i++)
    {
        Particle p;
        p.position = config.position;
        p.velocity = {velXDist(rng), velYDist(rng)};
        p.color = config.colorStart;
        p.size = config.sizeStart;
        p.life = lifeDist(rng);
        p.rotation = 0.0f;
        p.rotationSpeed = rotDist(rng);

        m_particles.push_back(p);
    }
}

void ParticleSystem::render()
{
    for (const auto& p : m_particles)
    {
        Renderer::drawRotatedQuad(p.position, {p.size, p.size}, p.rotation, p.color);
    }
}
