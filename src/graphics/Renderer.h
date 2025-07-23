#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include <memory>
#include <array>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec4 color;
    float texIndex;
    float tilingFactor;
};

struct RenderStats
{
    uint32_t drawCalls = 0;
    uint32_t quadCount = 0;
    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
};

class Renderer
{
public:
    static void init();
    static void shutdown();

    static void beginFrame(const Camera& camera);
    static void endFrame();

    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, Texture* texture = nullptr, float tilingFactor = 1.0f, const glm::vec2& uvMin = {0.0f, 0.0f}, const glm::vec2& uvMax = {1.0f, 1.0f});

    static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, Texture* texture = nullptr, float tilingFactor = 1.0f, const glm::vec2& uvMin = {0.0f, 0.0f}, const glm::vec2& uvMax = {1.0f, 1.0f});

    static void drawLine(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f}, float thickness = 1.0f);

    static const RenderStats& getStats();
    static void resetStats();

private:
    struct RenderData
    {
        static const uint32_t MAX_QUADS = 20000;
        static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
        static const uint32_t MAX_INDICES = MAX_QUADS * 6;
        static const uint32_t MAX_TEXTURE_SLOTS = 32;

        GLuint quadVAO = 0;
        GLuint quadVBO = 0;
        GLuint quadIBO = 0;

        GLuint lineVAO = 0;
        GLuint lineVBO = 0;

        uint32_t quadIndexCount = 0;
        Vertex* quadVertexBufferBase = nullptr;
        Vertex* quadVertexBufferPtr = nullptr;

        uint32_t lineVertexCount = 0;
        Vertex* lineVertexBufferBase = nullptr;
        Vertex* lineVertexBufferPtr = nullptr;

        std::array<GLuint, MAX_TEXTURE_SLOTS> textureSlots;
        uint32_t textureSlotIndex = 1; // 0 = white texture

        std::unique_ptr<Shader> shader;
        std::unique_ptr<Shader> lineShader;
        std::unique_ptr<Texture> whiteTexture;

        RenderStats stats;
    };

    static RenderData s_data;

    static void flush();
    static void flushLines();
    static void startBatch();
    static void nextBatch();
};
