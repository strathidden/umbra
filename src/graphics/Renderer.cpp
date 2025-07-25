#include "Renderer.h"
#include "../core/Logger.h"
#include <glad/glad.h>
#include <array>

Renderer::RenderData Renderer::s_data;

void Renderer::init()
{
    glGenVertexArrays(1, &s_data.quadVAO);
    glBindVertexArray(s_data.quadVAO);

    glGenBuffers(1, &s_data.quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_data.quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * s_data.MAX_VERTICES, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoord));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texIndex));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, tilingFactor));

    uint32_t indices[s_data.MAX_INDICES];
    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_data.MAX_INDICES; i += 6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        offset += 4;
    }

    glGenBuffers(1, &s_data.quadIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data.quadIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &s_data.lineVAO);
    glBindVertexArray(s_data.lineVAO);

    glGenBuffers(1, &s_data.lineVBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_data.lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 2000, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    s_data.whiteTexture = std::make_unique<Texture>();
    uint32_t whiteTextureData = 0xffffffff;
    glGenTextures(1, &s_data.whiteTexture->m_id);
    glBindTexture(GL_TEXTURE_2D, s_data.whiteTexture->m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &whiteTextureData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    s_data.shader = std::make_unique<Shader>();
    s_data.lineShader = std::make_unique<Shader>();

    s_data.shader->load("shaders/quad.vert", "shaders/quad.frag");
    s_data.shader->use();

    int samplers[s_data.MAX_TEXTURE_SLOTS];
    for (int i = 0; i < s_data.MAX_TEXTURE_SLOTS; i++)
    {
        samplers[i] = i;
    }

    s_data.shader->setIntArray("u_textures", samplers, s_data.MAX_TEXTURE_SLOTS);

    s_data.lineShader->load("shaders/line.vert", "shaders/line.frag");

    s_data.textureSlots[0] = s_data.whiteTexture->m_id;
    for (uint32_t i = 1; i < s_data.MAX_TEXTURE_SLOTS; i++)
    {
        s_data.textureSlots[i] = 0;
    }

    s_data.quadVertexBufferBase = new Vertex[s_data.MAX_VERTICES];
    s_data.lineVertexBufferBase = new Vertex[2000];

    Logger::info("Renderer initialized");
}

void Renderer::shutdown()
{
    glDeleteVertexArrays(1, &s_data.quadVAO);
    glDeleteBuffers(1, &s_data.quadVBO);
    glDeleteBuffers(1, &s_data.quadIBO);

    glDeleteVertexArrays(1, &s_data.lineVAO);
    glDeleteBuffers(1, &s_data.lineVBO);

    delete[] s_data.quadVertexBufferBase;
    delete[] s_data.lineVertexBufferBase;
}

void Renderer::beginFrame(const Camera& camera)
{
    s_data.shader->use();
    s_data.shader->setMat4("u_viewProjection", camera.getViewProjection());

    s_data.lineShader->use();
    s_data.lineShader->setMat4("u_viewProjection", camera.getViewProjection());

    startBatch();
}

void Renderer::endFrame()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    flush();
    flushLines();
}

void Renderer::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, Texture* texture, float tilingFactor, const glm::vec2& uvMin, const glm::vec2& uvMax)
{
    constexpr float rotation = 0.0f;
    const glm::vec2 positions[4] = {
        { -0.5f, -0.5f },
        {  0.5f, -0.5f },
        {  0.5f,  0.5f },
        { -0.5f,  0.5f }
    };

    const glm::vec2 texCoords[4] = {
        { uvMin.x, uvMin.y },
        { uvMax.x, uvMin.y },
        { uvMax.x, uvMax.y },
        { uvMin.x, uvMax.y }
    };

    if (s_data.quadIndexCount + 6 >= RenderData::MAX_INDICES || (s_data.quadVertexBufferPtr + 4) > (s_data.quadVertexBufferBase + RenderData::MAX_VERTICES))
    {
        nextBatch();
    }

    float textureIndex = 0.0f;
    if (texture)
    {
        for (uint32_t i = 1; i < s_data.textureSlotIndex; i++)
        {
            if (s_data.textureSlots[i] == texture->m_id)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            if (s_data.textureSlotIndex >= RenderData::MAX_TEXTURE_SLOTS)
            {
                nextBatch();
            }

            textureIndex = (float)s_data.textureSlotIndex;
            s_data.textureSlots[s_data.textureSlotIndex] = texture->m_id;
            s_data.textureSlotIndex++;
        }
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

    for (int i = 0; i < 4; i++)
    {
        s_data.quadVertexBufferPtr->position = transform * glm::vec4(positions[i], 0.0f, 1.0f);
        s_data.quadVertexBufferPtr->texCoord = texCoords[i];
        s_data.quadVertexBufferPtr->color = color;
        s_data.quadVertexBufferPtr->texIndex = textureIndex;
        s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_data.quadVertexBufferPtr++;
    }

    s_data.quadIndexCount += 6;
    s_data.stats.quadCount++;
}

void Renderer::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, Texture* texture, float tilingFactor, const glm::vec2& uvMin, const glm::vec2& uvMax)
{
    const glm::vec2 positions[4] = {
        { -0.5f, -0.5f },
        {  0.5f, -0.5f },
        {  0.5f,  0.5f },
        { -0.5f,  0.5f }
    };

    const glm::vec2 texCoords[4] = {
        { uvMin.x, uvMin.y },
        { uvMax.x, uvMin.y },
        { uvMax.x, uvMax.y },
        { uvMin.x, uvMax.y }
    };

    if (s_data.quadIndexCount + 6 >= RenderData::MAX_INDICES || (s_data.quadVertexBufferPtr + 4) > (s_data.quadVertexBufferBase + RenderData::MAX_VERTICES))
    {
        nextBatch();
    }

    float textureIndex = 0.0f;
    if (texture)
    {
        for (uint32_t i = 1; i < s_data.textureSlotIndex; i++)
        {
            if (s_data.textureSlots[i] == texture->m_id)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            if (s_data.textureSlotIndex >= RenderData::MAX_TEXTURE_SLOTS)
            {
                nextBatch();
            }

            textureIndex = (float)s_data.textureSlotIndex;
            s_data.textureSlots[s_data.textureSlotIndex] = texture->m_id;
            s_data.textureSlotIndex++;
        }
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

    for (int i = 0; i < 4; i++)
    {
        s_data.quadVertexBufferPtr->position = transform * glm::vec4(positions[i], 0.0f, 1.0f);
        s_data.quadVertexBufferPtr->texCoord = texCoords[i];
        s_data.quadVertexBufferPtr->color = color;
        s_data.quadVertexBufferPtr->texIndex = textureIndex;
        s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_data.quadVertexBufferPtr++;
    }

    s_data.quadIndexCount += 6;
    s_data.stats.quadCount++;
}

void Renderer::drawLine(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color, float thickness)
{
    if (s_data.lineVertexCount + 4 > 2000) {
        flushLines();
    }

    glm::vec2 dir = glm::normalize(p1 - p0);
    glm::vec2 normal = glm::vec2(-dir.y, dir.x) * thickness * 0.5f;

    s_data.lineVertexBufferPtr->position = {p0.x - normal.x, p0.y - normal.y, 0.0f};
    s_data.lineVertexBufferPtr->color = color;
    s_data.lineVertexBufferPtr++;
    
    s_data.lineVertexBufferPtr->position = {p0.x + normal.x, p0.y + normal.y, 0.0f};
    s_data.lineVertexBufferPtr->color = color;
    s_data.lineVertexBufferPtr++;
    
    s_data.lineVertexBufferPtr->position = {p1.x - normal.x, p1.y - normal.y, 0.0f};
    s_data.lineVertexBufferPtr->color = color;
    s_data.lineVertexBufferPtr++;
    
    s_data.lineVertexBufferPtr->position = {p1.x + normal.x, p1.y + normal.y, 0.0f};
    s_data.lineVertexBufferPtr->color = color;
    s_data.lineVertexBufferPtr++;

    s_data.lineVertexCount += 4;
}

const RenderStats& Renderer::getStats()
{
    return s_data.stats;
}

void Renderer::resetStats()
{
    memset(&s_data.stats, 0, sizeof(RenderStats));
}

void Renderer::flush()
{
    if (s_data.quadIndexCount == 0) return;

    s_data.shader->use();

    GLsizeiptr size = (uint8_t*)s_data.quadVertexBufferPtr - (uint8_t*)s_data.quadVertexBufferBase;
    glBindBuffer(GL_ARRAY_BUFFER, s_data.quadVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_data.quadVertexBufferBase);

    for (uint32_t i = 0; i < s_data.textureSlotIndex; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, s_data.textureSlots[i]);
    }

    glBindVertexArray(s_data.quadVAO);
    glDrawElements(GL_TRIANGLES, s_data.quadIndexCount, GL_UNSIGNED_INT, nullptr);
    s_data.stats.drawCalls++;

    s_data.stats.vertexCount += s_data.quadIndexCount * 4 / 6;
    s_data.stats.indexCount += s_data.quadIndexCount;
}

void Renderer::flushLines()
{
    if (s_data.lineVertexCount == 0) return;

    GLsizeiptr size = (uint8_t*)s_data.lineVertexBufferPtr - (uint8_t*)s_data.lineVertexBufferBase;
    glBindBuffer(GL_ARRAY_BUFFER, s_data.lineVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_data.lineVertexBufferBase);

    s_data.lineShader->use();
    glBindVertexArray(s_data.lineVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, s_data.lineVertexCount);
    s_data.stats.drawCalls++;

    s_data.lineVertexBufferPtr = s_data.lineVertexBufferBase;
    s_data.lineVertexCount = 0;
}

void Renderer::startBatch()
{
    s_data.quadIndexCount = 0;
    s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
    s_data.textureSlotIndex = 1;

    s_data.lineVertexCount = 0;
    s_data.lineVertexBufferPtr = s_data.lineVertexBufferBase;
}

void Renderer::nextBatch()
{
    flush();
    startBatch();
}
