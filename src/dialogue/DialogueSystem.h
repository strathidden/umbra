#pragma once
#include "../ui/UI.h"
#include "../ui/Button.h"
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>

struct DialogueNode
{
    std::string id;
    std::string speaker;
    std::string text;
    std::vector<std::pair<std::string, std::string>> choices; // text, next node
};

class DialogueSystem
{
public:
    bool loadFromFile(const std::string& filepath);
    void startDialogue(const std::string& startNode);
    void render();

    bool isActive() const { return m_active; }
    void setOnChoiceSelected(std::function<void(const std::string&)> callback) { m_choiceCallback = callback; }

private:
    std::unordered_map<std::string, DialogueNode> m_nodes;
    DialogueNode* m_currentNode = nullptr;
    bool m_active = false;
    std::function<void(const std::string&)> m_choiceCallback;
    std::vector<std::unique_ptr<Button>> m_choiceButtons;
};
