#include "ResourceManager.hpp"

std::map<std::string, Music> ResourceManager::musicTracks;
std::unordered_map<std::string, Texture2D> ResourceManager::textures;

void ResourceManager::loadTexture(const std::string& name, const std::string& fileName) {
    textures[name] = LoadTexture(fileName.c_str());
}

Texture2D ResourceManager::getTexture(const std::string& name) {
    return textures[name];
}

void ResourceManager::loadMusic(const std::string& name, const std::string& path) {
    musicTracks[name] = LoadMusicStream(path.c_str());
}

Music ResourceManager::getMusic(const std::string& name) {
    return musicTracks[name];
}

void ResourceManager::unloadAll() {
    // 1. Clean up all Textures
    for (auto& pair : textures) {
        UnloadTexture(pair.second);
    }
    textures.clear();

    // 2. Clean up all Music
    for (auto& pair : musicTracks) {
        UnloadMusicStream(pair.second);
    }
    musicTracks.clear();
}
