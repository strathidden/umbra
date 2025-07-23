#include "graphics/Camera.h"
#include "graphics/Texture.h"
#include "graphics/Renderer.h"
#include "core/ResourceManager.h"
#include "core/Game.h"
#include "core/Logger.h"
#include <glm/glm.hpp>

class UmbraGame : public Game
{
public:
    void initialize() override
    {
        Logger::info("Initializing game...");

        Renderer::init();

        m_playerTexture = ResourceManager::loadTexture("assets/player.png");
        if (!m_playerTexture)
        {
            Logger::error("Failed to load player texture!");
        }

        m_camera = std::make_unique<Camera>();
        m_camera->setProjection(0.0f, 1280.0f, 720.0f, 0.0f);
    }

    void update(float deltaTime) override
    {
        m_playerPos.x += 100.0f * deltaTime;
        if (m_playerPos.x > 1280) m_playerPos.x = 0;
    }

    void render() override
    {
        Renderer::beginFrame(*m_camera);

        Renderer::drawQuad(m_playerPos, {100.0f, 100.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, m_playerTexture);

        for (int i = 0; i < 5; i++)
        {
            Renderer::drawQuad({i * 200.0f, 500.0f}, {180.0f, 50.0f}, {0.2, 0.8f, 0.3f, 1.0f});
        }

        Renderer::endFrame();
    }

    void shutdown() override
    {
        ResourceManager::clear();
        Renderer::shutdown();
        Logger::info("Shutting down...");
    }

private:
    glm::vec2 m_playerPos = {100.0f, 100.0f};
    Texture* m_playerTexture = nullptr;
    std::unique_ptr<Camera> m_camera;
};

int main()
{
    UmbraGame game;
    game.run();
    return 0;
}
