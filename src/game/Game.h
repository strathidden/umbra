#include "../core/Engine.h"
#include "../editor/LevelEditor.h"
#include <glm/glm.hpp>

class Game : public Engine
{
public:
    void initialize() override;
    void onResize(int width, int height) override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;

private:
    std::unique_ptr<LevelEditor> m_editor;
};
