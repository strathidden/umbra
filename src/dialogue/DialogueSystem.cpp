#include "DialogueSystem.h"
#include "../graphics/Renderer.h"
#include "../core/Logger.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool DialogueSystem::loadFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        Logger::error("Failed to open dialogue file: ", filepath);
        return false;
    }

    try
    {
        json data = json::parse(file);

        for (const auto& node : data["nodes"])
        {
            DialogueNode dn;
            dn.id = node["id"];
            dn.speaker = node["speaker"];
            dn.text = node["text"];

            for (const auto& choice : node["choices"])
            {
                dn.choices.emplace_back(choice["text"], choice["next"]);
            }

            m_nodes[dn.id] = dn;
        }

        Logger::info("Loaded dialogue: ", filepath);
        return true;
    }
    catch (const json::exception& e)
    {
        Logger::error("JSON parse error: ", e.what());
        return false;
    }
}

void DialogueSystem::startDialogue(const std::string& startNode)
{
    auto it = m_nodes.find(startNode);
    if (it == m_nodes.end())
    {
        Logger::error("Dialogue node not found: ", startNode);
        return;
    }

    m_currentNode = &it->second;
    m_active = true;
    m_choiceButtons.clear();

    float yPos = 450.0f;
    for (size_t i = 0; i < m_currentNode->choices.size(); i++)
    {
        auto button = std::make_unique<Button>(m_currentNode->choices[i].first);
        button->position = {540.0f, yPos};
        button->size = {200.0f, 40.0f};

        std::string nextNode = m_currentNode->choices[i].second;
        button->setOnClick([this, nextNode]() {
            if (m_choiceCallback)
            {
                m_choiceCallback(nextNode);
            }
            startDialogue(nextNode);
        });

        m_choiceButtons.push_back(std::move(button));
        yPos += 50.0f;
    }

    for (auto& btn : m_choiceButtons)
    {
        UIManager::addElement(btn.get());
    }
}

void DialogueSystem::render()
{
    if (!m_active || !m_currentNode) return;

    Renderer::drawQuad({180, 350}, {1080, 200}, {0.1f, 0.1f, 0.1f, 0.9f});

    // todo: render speaker name and text;
}
