#include "../graphics/Camera.h"
#include "../graphics/Texture.h"
#include "../graphics/Renderer.h"
#include "../core/ResourceManager.h"
#include "../core/Engine.h"
#include "../core/Logger.h"
#include "../entities/Player.h"
#include "../levels/TileMap.h"
#include <glm/glm.hpp>

class Game : public Engine
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
        m_camera->setProjection(0.0f, 1920.0f, 1080.0f, 0.0f);

        m_tileMap.loadFromFile("assets/level1.csv");

        m_player = std::make_unique<Player>();

        for (const auto& tile : m_tileMap.getTiles())
        {
            if (tile.collidable)
            {
                auto body = PhysicsEngine::createBody(tile.position, glm::vec2(m_tileMap.getTileSize()), true);
                body->collider.layer = 2;
            }
        }

        PhysicsEngine::setCollisionLayers(1, 2, true);
    }

    void onResize(int width, int height) override
    {

    }

    void update(float deltaTime) override
    {
        m_player->update(deltaTime);
        // auto stats = Renderer::getStats();
        // Logger::info("Draw calls: ", stats.drawCalls, " Quads count: ", stats.quadCount, " Index count: ", stats.indexCount, " Vertex count: ", stats.vertexCount);
    }

    void render() override
    {
        Renderer::beginFrame(*m_camera);

        m_tileMap.render();
        m_player->render();

        Renderer::endFrame();
    }

    void shutdown() override
    {
        ResourceManager::clear();
        Renderer::shutdown();
        Logger::info("Shutting down...");
    }

private:
    TileMap m_tileMap;
    std::unique_ptr<Player> m_player;
    Texture* m_playerTexture = nullptr;
    std::unique_ptr<Camera> m_camera;
};
