#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <raylib.h>
#include <unordered_map>
#include <string>
#include <iomanip>
#include <map>

class ResourceManager {
private:
    static std::unordered_map<std::string, Texture2D> textures;
    static std::map<std::string, Music> musicTracks;

public:
    // Loads a texture from a file and gives it a simple name
    static void loadTexture(const std::string& name, const std::string& fileName);
    
    // Retrieves a loaded texture
    static Texture2D getTexture(const std::string& name);
    
    // Cleans up RAM when the game closes
    static void unloadAll();

    static void loadMusic(const std::string& name, const std::string& path);
    static Music getMusic(const std::string& name);
};

#endif