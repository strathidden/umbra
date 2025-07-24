#include "ResourceManager.h"

std::unordered_map<std::string, std::unique_ptr<Texture>> ResourceManager::m_textures;

Texture* ResourceManager::loadTexture(const std::string& filepath)
{
    auto it = m_textures.find(filepath);
    if (it != m_textures.end())
    {
        return it->second.get();
    }

    auto texture = std::make_unique<Texture>();
    if (texture->loadFromFile(filepath))
    {
        m_textures[filepath] = std::move(texture);
        return m_textures[filepath].get();
    }

    return nullptr;
}

void ResourceManager::clear()
{
    m_textures.clear();
}
