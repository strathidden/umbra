#pragma once
#include "../levels/TileMap.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class LevelEditor
{
public:
    void init(TileMap* tileMap);
    void update(float deltaTime);
    void renderUI();
    void setActive(bool active) { m_active = active; }
    bool isActive() const { return m_active; }
    void shutdown();

private:
    void saveLevel();
    void loadLevel();
    void handleTileEditing();
    void renderGrid();
    void renderSelection();

    TileMap* m_tileMap = nullptr;
    bool m_active = false;
    int m_selectedTile = 1;
    bool m_showGrid = true;
    bool m_snapToGrid = true;
    bool m_showCollision = true;
    glm::ivec2 m_hoveredTile = {-1, -1};
    std::string m_currentLevel = "test_level.umbra";

    enum class Tool { BRUSH, ERASER, FILL, SELECT };
    Tool m_currentTool = Tool::BRUSH;
    glm::ivec2 m_selectionStart = {-1, -1};
    glm::ivec2 m_selectionEnd = {-1, -1};
};
