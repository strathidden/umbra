#include "ResourceManager.h"

std::unordered_map<std::string, std::unique_ptr<Texture>> ResourceManager::m_textures;

Texture* ResourceManager::loadTexture(const std::string& path)
{
    auto it = m_textures.find(path);
    if (it != m_textures.end())
    {
        return it->second.get();
    }

    auto texture = std::make_unique<Texture>();
    if (texture->loadFromFile(path))
    {
        m_textures[path] = std::move(texture);
        return m_textures[path].get();
    }

    return nullptr;
}

void ResourceManager::clear()
{
    m_textures.clear();
}
