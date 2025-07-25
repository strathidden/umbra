#include "../core/Engine.h"
#include "../editor/LevelEditor.h"
#include "../entities/Player.h"
#include <glm/glm.hpp>

class Game : public Engine
{
public:
    void initialize() override;
    void onResize(int width, int height) override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;

    LevelEditor& getLevelEditor() { return *m_levelEditor; }
    Player* getPlayer() { return m_player.get(); }

private:
    std::unique_ptr<LevelEditor> m_levelEditor;
    std::unique_ptr<Player> m_player;
    bool m_editorActive = false;
};
