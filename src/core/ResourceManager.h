#pragma once
#include "../graphics/Texture.h"
#include <unordered_map>
#include <string>
#include <memory>

class ResourceManager
{
public:
    static Texture* loadTexture(const std::string& path);
    static void clear();

private:
    static std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
};
