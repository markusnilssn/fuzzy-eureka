#include "Content.h"
#include <fstream>
#include <any>
#include <unordered_map>
#include <iostream>
#include <filesystem>

#include <string>

#include "ofs.h"


/*

            while(std::getline(stream, line))
            {
                std::trim(line);

                if (line.empty()) continue;
                if (line.rfind('#', 0) == 0) continue; // comment

                if (line.rfind("import", 0) == 0) { // todo: implement 
                    size_t spacing = line.find_first_of(' ');
                    size_t endline = line.find_first_of(';');

                    std::string filename = line.substr(spacing + 1, ((endline - 1) - spacing));
                    
                    continue; // import file
                }
                // else if(line.rfind("const", 0) == 0) {} // const
                // else if(line.rfind("namespace", 0) == 0) {} // const
                // else 
                {
                    // lvalue
                    size_t spacing = line.find_first_of(' ');
                    size_t separator = line.find_first_of(':');

                    if(spacing == std::string::npos || separator == std::string::npos || spacing > separator)
                    {
                        std::cerr << "failed to parse line " << line << std::endl;
                        continue;
                    }

                    std::string type = line.substr(0, spacing);
                    std::string name = line.substr(spacing + 1, ((separator - 1) - spacing));

                    if(type.empty() || name.empty())
                    {
                        std::cerr << "failed to parse line " << line << std::endl;
                        continue;
                    }
                    
                    // rvalue
                    if(line.find_first_of('[') != std::string::npos) 
                    {

                        // Object

                        // iterate through lines 
                    }
                    else 
                    {
                        size_t end = line.find_last_of(';');
                        std::string rvalue = line.substr((separator + 1), (end - 1) - separator);    
                        
                        std::cout << type << " " << name << ": " << rvalue << std::endl;
                    }
                }
            }

*/


/***
 * Content
 */

// struct Property 
// {
//     enum struct Type 
//     {
//         Short,
//         Double,
//         String,
    
//         Point,
//     };

//     Type type;
//     std::any value;

//     explicit Property(sf::Vector2i&& _value) { value = _value; type = Type::Point; }
//     explicit Property(std::string&& _value) { value = _value; type = Type::String; }
//     explicit Property(double _value) { value = _value; type = Type::Double; }
//     explicit Property(int _value) { value = _value; type = Type::Short; }

//     const sf::Vector2i GetPoint() const { return std::any_cast<sf::Vector2i>(value); }
//     const std::string GetString() const { return std::any_cast<std::string>(value); }
//     const double GetDouble() const { return std::any_cast<double>(value); }
//     const short GetShort() const { return std::any_cast<short>(value); }
// };

// struct File 
// {
//     std::unordered_map<std::string, Property> constants;
// };

// const char* ws = " \t\n\r\f\v";

// // https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
// // trim from end of string (right)
// inline std::string& rtrim(std::string& s, const char* t = ws)
// {
//     s.erase(s.find_last_not_of(t) + 1);
//     return s;
// }

// // trim from beginning of string (left)
// inline std::string& ltrim(std::string& s, const char* t = ws)
// {
//     s.erase(0, s.find_first_not_of(t));
//     return s;
// }

// // trim from both ends of string (right then left)
// inline std::string& trim(std::string& s, const char* t = ws)
// {
//     return ltrim(rtrim(s, t), t);
// }

// bool to_point(const std::string& rvalue, sf::Vector2i& outPoint) 
// {
//     size_t start = rvalue.find('{');
//     size_t end = rvalue.find('}');

//     if(start != std::string::npos && end != std::string::npos)
//     {
//         return false;
//     }

//     int values[2]{};
//     size_t pos = std::string::npos;
//     do
//     {  


//     } while(pos != std::string::npos);

//     return true;
// };
// bool to_string(const std::string& rvalue, std::string& outString) { return false; };
// bool to_double(const std::string& rvalue, double& outDouble) { return false; };
// bool to_short(const std::string& rvalue, short& outShort) { return false; };






 Content::Content(const std::filesystem::path& contentJsonPath)
    : relativePath("resources/")
{
    std::filesystem::path currentPath = std::filesystem::current_path() / "Content" / "content.ojs";
    [[maybe_unused]] ofs::File file(currentPath);

    // const std::filesystem::path currentPath = std::filesystem::current_path() / "Content";
    // for(const std::filesystem::path& path : std::filesystem::recursive_directory_iterator(currentPath))
    // {
    //     std::cout << path.c_str() << std::endl;
    //     if(!std::filesystem::is_regular_file(path) || path.extension() != extention)
    //     {
    //         continue;
    //     }   

    //     std::unordered_map<std::string, Property> constants;

    //     std::string currentNamespace;
    //     size_t lineIndex;
    //     std::string line;
    //     std::ifstream stream(path.c_str(), std::ifstream::in);
    //     while(std::getline(stream, line))
    //     {
    //         // introduced in c++20
    //         auto start_with = [line](const std::string& type, size_t& outEndIndex) -> bool
    //         {
    //             outEndIndex = type.size() + 1;
    //             return line.rfind(type, 0) == 0;
    //         };

    //         line.erase(0, line.find_first_not_of("\t"));
    //         line.erase(line.find_last_not_of("\t\r\n") + 1);

    //         // remove everything until there is like a ; 

    //         if(line.empty()) continue;
            
    //         // skip comments
    //         size_t startIndex = 0;
    //         if (start_with("#", startIndex)) 
    //         {
    //             continue;
    //         }

    //         if (start_with("namespace", startIndex)) 
    //         {
    //             size_t nsStart = line.find(" ") + 1;
    //             size_t nsEnd = line.find(":");
    //             currentNamespace = line.substr(nsStart, nsEnd - nsStart);
    //             continue;
    //         }

    //         // Handle end of namespace: ]
    //         if (line == "];" || line == "]") 
    //         {
    //             currentNamespace.clear();
    //             continue;
    //         }

    //         if(start_with("const", startIndex))
    //         {
    //             size_t endIndex = line.find(":");
    //             std::string name = line.substr(startIndex, endIndex - startIndex);
                
    //             size_t start = endIndex + 1;
    //             std::string rvalue = line.substr(start,line.find_last_of(';') - start);

    //             bool parsed = false;
    //             sf::Vector2i pointValue;
    //             if(to_point(rvalue, pointValue)) 
    //             {
    //                 parsed = true;
    //                 constants.insert({name, Property(std::move(pointValue))});
    //             }
                
    //             std::string stringValue;
    //             if(to_string(rvalue, stringValue)) 
    //             {
    //                 parsed = true;
    //                 constants.insert({name, Property(std::move(stringValue))});
    //             }
                
    //             double doubleValue;
    //             if(to_double(rvalue, doubleValue)) 
    //             {
    //                 parsed = true;
    //                 constants.insert({name, Property(doubleValue)});
    //             }
                
    //             short shortValue;
    //             if(to_short(rvalue, shortValue)) 
    //             {
    //                 parsed = true;
    //                 constants.insert({name, Property(shortValue)});
    //             }

    //             if(!parsed)
    //             {
    //                 std::cerr << "failed to parse line " << lineIndex << std::endl;
    //             }
    //         }

    //         if(start_with("sprite-sheet", startIndex))
    //         {

    //         }

    //         if(start_with("animation", startIndex)) {}
    //         if(start_with("texture", startIndex)) {}

    //         lineIndex++;
    //     }
        
    //     std::cout << constants.size() << std::endl;

    // }

    
    // auto recursivePath = std::filesystem::recursive_directory_iterator(resourcePath);
    // for(auto& path : recursivePath)
    // {
    //     if(std::filesystem::is_directory(path))
    //     {
    //         continue;
    //     }

    //     const auto& fullPath = path.path();
    //     const auto& extention = fullPath.extension();
    //     if(extention == ".png")
    //     {

    //     }
    // }


}

sf::Texture* Content::GetTexture(const std::string &texturePath)
{
    size_t hashCode = std::hash<std::string>{}(texturePath);
    auto iterator = textures.find(hashCode);
    if(iterator != textures.end())
    {
        return (iterator->second).get();
    }

    std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>(GetRelativePath(texturePath));
    sf::Texture* returnValue = texture.get();
    textures[hashCode] = std::move(texture);
    return returnValue;
}

sf::Font* Content::GetFont(const std::string &fontPath)
{
    size_t hashCode = std::hash<std::string>{}(fontPath);
    auto iterator = fonts.find(hashCode);
    if(iterator != fonts.end())
    {
        return (iterator->second).get();
    }

    std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>(GetRelativePath(fontPath));
    sf::Font* returnValue = font.get();
    fonts[hashCode] = std::move(font);
    return returnValue;
}

sf::Texture *Content::CombineTextures(const std::string& name, sf::Texture* front, sf::Texture* back)
{
    return nullptr;
}

std::string Content::GetRelativePath(const std::string &filePath)
{
    return relativePath + filePath;
}


/***
 * Animation
 */
Animation::Animation(sf::Texture *texture, const sf::Vector2i &frameSize)
    : texture(texture)
    , frameSize(frameSize)
{
    
}

void Animation::Cut(const std::string &name, const sf::Vector2i &start, const sf::Vector2i &end)
{
    constexpr int offset = 1;

    int width = (end.x - start.x) + offset;
    int height = (end.y - start.y) + offset;

    int frames = width * height;

    Sequence sequence{};
    for (int i = 0; i < frames; i++)
    {
        int x = i / height;
        int y = i % height;

        sequence.emplace_back(sf::IntRect(sf::Vector2i(x * frameSize.x, y * frameSize.y), frameSize));
    }
    
    size_t hashCode = std::hash<std::string>{}(name);
    sequences[hashCode] = std::move(sequence);
}

const Animation::Sequence &Animation::GetSequence(const std::string &name)
{
    size_t hashCode = std::hash<std::string>{}(name);

    auto iterator = sequences.find(hashCode);
    if(iterator == sequences.end())
    {
        throw std::out_of_range("Failed to find sequence");
    }

    return sequences[hashCode];
}

/***
 * SpriteSheet
 */
SpriteSheet::SpriteSheet(sf::Texture *texture, const sf::Vector2i &tileSize)
    : texture(texture)
    , tileSize(tileSize)
{

}

void SpriteSheet::Cut(const std::string &name, const sf::Vector2i &index)
{
    sf::IntRect rect(sf::Vector2i(index.x * tileSize.x, index.y * tileSize.y), tileSize);
    
    size_t hashCode = std::hash<std::string>{}(name);
    tiles[hashCode] = std::move(rect);
}

const SpriteSheet::Tile &SpriteSheet::GetTile(const std::string &name)
{
    size_t hashCode = std::hash<std::string>{}(name);

    auto iterator = tiles.find(hashCode);
    if(iterator == tiles.end())
    {
        throw std::out_of_range("Failed to find sequence");
    }

    return tiles[hashCode];
}
