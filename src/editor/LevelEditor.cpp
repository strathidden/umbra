#include "LevelEditor.h"
#include "../core/InputManager.h"
#include "../core/ResourceManager.h"
#include "../core/Logger.h"
#include "../core/Engine.h"
#include "../graphics/Renderer.h"
#include "../graphics/Camera.h"
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

void LevelEditor::init(TileMap* tileMap)
{
    m_tileMap = tileMap;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(Engine::getInstance()->getWindow().getNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    loadLevel();
}

void LevelEditor::update(float deltaTime)
{
    if (!m_active) return;

    auto& window = Engine::getInstance()->getWindow();
    auto mousePos = InputManager::getMousePosition();
    auto cameraPos = Engine::getInstance()->getCamera().getPosition();

    glm::vec2 worldPos = {
        mousePos.x + cameraPos.x,
        mousePos.y + cameraPos.y
    };

    m_hoveredTile = {
        static_cast<int>(worldPos.x / m_tileMap->getTileSize().x),
        static_cast<int>(worldPos.y / m_tileMap->getTileSize().y)
    };

    if (ImGui::GetIO().WantCaptureMouse) return;
    handleTileEditing();
}

void LevelEditor::renderUI()
{
    if (!m_active) return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::Begin("LevelEditor", &m_active, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New"))
            {
                // m_tileMap->clear();
            }
            if (ImGui::MenuItem("Open"))
            {
                loadLevel();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                saveLevel();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Text("Tools");
    if (ImGui::Button("Brush (B)")) m_currentTool = Tool::BRUSH;
    ImGui::SameLine();
    if (ImGui::Button("Eraser (E)")) m_currentTool = Tool::ERASER;
    ImGui::SameLine();
    if (ImGui::Button("Fill (F)")) m_currentTool = Tool::FILL;
    ImGui::SameLine();
    if (ImGui::Button("Select (S)")) m_currentTool = Tool::SELECT;

    ImGui::Text("Tiles");
    const auto& tileset = m_tileMap->getTileset();
    ImVec2 buttonSize(40, 40);
    int tilesPerRow = tileset->getWidth() / m_tileMap->getTileSize().x;

    for (int i = 0; i < 64; i++)
    {
        int tileX = i % tilesPerRow;
        int tileY = i / tilesPerRow;

        float u0 = static_cast<float>(tileX * m_tileMap->getTileSize().x) / tileset->getWidth();
        float v0 = static_cast<float>(tileY * m_tileMap->getTileSize().y) / tileset->getHeight();
        float u1 = u0 + static_cast<float>(m_tileMap->getTileSize().x) / tileset->getWidth();
        float v1 = v0 + static_cast<float>(m_tileMap->getTileSize().y) / tileset->getHeight();

        ImGui::PushID(i);
        if (ImGui::ImageButton("##TileBtn", (void*)(intptr_t)tileset->m_id, buttonSize, ImVec2(u0, v0), ImVec2(u1, v1), ImVec4(0,0,0,0), (m_selectedTile == i) ? ImVec4(1,1,1,1) : ImVec4(0.5f,0.5f,0.5f,1)))
        {
            m_selectedTile = i;
        }
        ImGui::PopID();

        if ((i + 1) % 8 != 0) ImGui::SameLine();
    }

    ImGui::Text("Options");
    ImGui::Checkbox("Show Grid", &m_showGrid);
    ImGui::Checkbox("Snap to Grid", &m_snapToGrid);
    ImGui::Checkbox("Show Collision", &m_showCollision);

    ImGui::Text("Level: %s", m_currentLevel.c_str());
    ImGui::Text("Tiles: %zu", m_tileMap->getTiles().size());

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LevelEditor::renderGrid()
{
    if (!m_showGrid) return;

    auto cameraPos = Engine::getInstance()->getCamera().getPosition();
    int tileSize = m_tileMap->getTileSize().x;
    int screenWidth = Engine::getInstance()->getWindow().getWidth();
    int screenHeight = Engine::getInstance()->getWindow().getHeight();

    int startX = static_cast<int>(cameraPos.x) / tileSize;
    int startY = static_cast<int>(cameraPos.y) / tileSize;
    int endX = (static_cast<int>(cameraPos.x) + screenWidth) / tileSize + 1;
    int endY = (static_cast<int>(cameraPos.y) + screenHeight) / tileSize + 1;

    for (int x = startX; x < endX; x++)
    {
        float posX = x * tileSize;
        Renderer::drawLine({posX, startY * tileSize}, {posX, endY * tileSize}, {0.3f, 0.3f, 0.3f, 0.5f});
    }

    for (int y = startY; y < endY; y++)
    {
        float posY = y * tileSize;
        Renderer::drawLine({startX * tileSize, posY}, {endX * tileSize, posY}, {0.3f, 0.3f, 0.3f, 0.5f});
    }
}

void LevelEditor::handleTileEditing()
{
    if (m_currentTool == Tool::SELECT)
    {
        if (InputManager::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            m_selectionStart = m_hoveredTile;
        }

        if (InputManager::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            m_selectionEnd = m_hoveredTile;
        }
        return;
    }

    if (InputManager::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        if (m_currentTool == Tool::BRUSH)
        {
            m_tileMap->setTile(m_hoveredTile.x, m_hoveredTile.y, m_selectedTile, true);
        }
        else if (m_currentTool == Tool::ERASER)
        {
            m_tileMap->setTile(m_hoveredTile.x, m_hoveredTile.y, 0);
        }
        else if (m_currentTool == Tool::FILL)
        {
            // todo: fill implementation
        }
    }
}

void LevelEditor::saveLevel()
{
    json j;
    j["width"] = m_tileMap->getMapSize().x;
    j["height"] = m_tileMap->getMapSize().y;
    j["tile_size"] = m_tileMap->getTileSize().x;
    j["tileset"] = "assets/tileset.png";

    json tilesArray = json::array();
    for (const auto& tile : m_tileMap->getTiles())
    {
        json t;
        t["x"] = tile.worldPosition.x;
        t["y"] = tile.worldPosition.y;
        t["id"] = tile.id;
        t["collidable"] = tile.collidable;
        tilesArray.push_back(t);
    }
    j["tiles"] = tilesArray;

    std::ofstream file("assets/levels/" + m_currentLevel);
    if (file.is_open())
    {
        file << j.dump(4);
        Logger::info("Level saved: ", m_currentLevel);
    }
    else
    {
        Logger::error("Failed to save level: ", m_currentLevel);
    }
}

void LevelEditor::loadLevel()
{
    std::ifstream file("assets/levels/" + m_currentLevel);
    if (!file.is_open())
    {
        Logger::warn("Level file not found: ", m_currentLevel);
        return;
    }

    try
    {
        json j = json::parse(file);
        int width = j["width"];
        int height = j["height"];
        int tileSize = j["tile_size"];

        m_tileMap->createEmpty(width, height, tileSize);

        for (const auto& t : j["tiles"])
        {
            int x = t["x"];
            int y = t["y"];
            int id = t["id"];
            bool collidable = t["collidable"];

            m_tileMap->setTile(x, y, id, collidable);
        }

        Logger::info("Level loaded: ", m_currentLevel);
    }
    catch (const json::exception& e)
    {
        Logger::error("Level load error: ", e.what());
    }
}

void LevelEditor::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
