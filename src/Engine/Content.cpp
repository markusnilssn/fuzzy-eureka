#include "Content.h"
#include <filesystem>
#include <iostream>
#include <sstream>

// sf::Texture *Content::GetTexture(const std::string &filePath)
// {
//     auto iterator = textures.find(filePath);
//     if(iterator != textures.end()) 
//     {
//         return iterator->second.get();
//     }

//     std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
//     bool success = texture->loadFromFile(filePath);
//     if(!success) 
//     {
//         std::cerr << "Failed to load filePath" << filePath << std::endl;
//         return nullptr;
//     }

//     textures.emplace(filePath, std::move(texture));
//     return texture.get();
// }

// void Content::PreloadTextures(const std::string &folderPath, bool recursive)
// {
//     for(auto& file : std::filesystem::recursive_directory_iterator(folderPath)) 
//     {

//         std::filesystem::path filePath = file.path();

//         if(filePath.extension() == ".png") 
//         {
            
//         }
//     }
// }

Content::Content(const std::string &relativePath)
    : relativePath(relativePath)
{
    
}

void Content::LoadTexture2D(const std::string &filePath, const sf::Vector2i &tileSize)
{
    // if(textures.find(filePath) != textures.end())
    // {
    //     std::cerr << "already contains " << filePath << std::endl;
    //     return;
    // }    

    std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
    bool success = texture->loadFromFile(relativePath + filePath);
    if(!success) 
    {
        std::cerr << "failed to load texture " << filePath << std::endl;
        return;
    }

    // textures.emplace(filePath, texture);
}