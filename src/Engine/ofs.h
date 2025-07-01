/*
    ofs - Object File Structure
    A classic File Format but with namespaces, comments and variables
*/

#pragma once 

#include <filesystem>
#include <string> 
#include <unordered_map>
#include <any>
#include <fstream>
#include <iostream>
#include <set>
#include <string_view>


namespace std
{
    void trim(std::string& line)
    {
        line.erase(0, line.find_first_not_of("\t"));
        line.erase(line.find_last_not_of("\t\r\n") + 1);
    }

    void remove_whitespace(std::string& line)
    {
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
    }
}

namespace ofs
{
    struct Vector2 
    {
        float x{};
        float y{};
    };
    struct Point 
    {
        int x{};
        int y{};    
    };

    struct Constant
    {
        enum struct Type { Vector2, Point, String, Double, Short };

        std::string m_Namespace;
        std::string m_Name;

        std::any m_Value;
        Type m_Type;

        explicit Constant(const short value) : m_Value(value), m_Type(Type::Short) { }
        explicit Constant(const double value) : m_Value(value), m_Type(Type::Double) { }

        explicit Constant(const std::string& value) : m_Value(value), m_Type(Type::String) { }
        
        explicit Constant(const Point& value) : m_Value(value), m_Type(Type::Vector2) { }
        explicit Constant(const Vector2& value) : m_Value(value), m_Type(Type::Point) { }
    };

    struct Value { };
    struct Array { };
    struct Object { };

    struct File 
    {
        std::set<size_t> existing;
        std::vector<Constant> constants;
        std::vector<Object> objects;

        explicit File(const std::filesystem::path& path) 
        {
            if(!std::filesystem::is_regular_file(path) || path.extension() != ".ojs")
            {
                std::cerr << "File is not the correct format! " << path << std::endl;
                return;
            }   

            std::ifstream stream(path.c_str(), std::ifstream::in);
            if(!stream.is_open())
            {
                std::cerr << "Failed to open file " << path << std::endl;
                return;
            }

            std::vector<std::string> lines;
            {
                std::string line;
                while(std::getline(stream, line))
                {
                    std::trim(line);
                    
                    if(line.empty()) continue;
                    if(line.rfind('#', 0) == 0) continue;
                    
                    lines.push_back(line);
                }
            }


            
            size_t index = 0;
            while(index < lines.size())
            {
                std::string& line = lines[index];

                if (line.rfind("import", 0) == 0) // todo: implement 
                { 
                    size_t spacing = line.find_first_of(' ');
                    size_t endline = line.find_first_of(';');

                    std::string filename = line.substr(spacing + 1, ((endline - 1) - spacing));
                    
                    ++index;
                    continue; // import file
                }
                else 
                {
                    size_t spacing = line.find_first_of(' ');
                    size_t separator = line.find_first_of(':');

                    if(spacing == std::string::npos || separator == std::string::npos || spacing > separator)
                    {
                        std::cerr << "failed to parse line " << line << std::endl;
                        ++index;
                        continue;
                    }

                    std::string type = line.substr(0, spacing);
                    std::string name = line.substr(spacing + 1, ((separator - 1) - spacing));

                    if(type.empty() || name.empty())
                    {
                        std::cerr << "failed to parse line " << line << std::endl;
                        ++index;
                        continue;
                    }

                    if(type.compare("const") == 0) {}
                    else if(type.compare("namespace") == 0) 
                    {
                        [[maybe_unused]] std::vector<std::string>&& objects = GetObject(lines, index);
                        std::cout << objects.size() << std::endl;
                        continue;
                    }
                    else 
                    {
                        if(line.find_first_of('[') != std::string::npos) 
                        {
                            [[maybe_unused]] std::vector<std::string>&& objects = GetObject(lines, index);
                            std::cout << objects.size() << std::endl;
                            
                            continue;
                        }
                        else 
                        {
                            size_t end = line.find_last_of(';');
                            std::string rvalue = line.substr((separator + 1), (end - 1) - separator);    
                            
                            std::cout << type << " " << name << ": " << rvalue << std::endl;
                        }
                    }
                }

                ++index;
            }

            
            stream.close();
        }

        static std::vector<std::string> GetObject(const std::vector<std::string>& lines, size_t& index)
        {
            size_t currentIndex = index + 1; // start on next line
            short complexity = 0; 

            std::vector<std::string> returnValue;
            returnValue.push_back(lines[index]);

            while(currentIndex < lines.size())
            {
                const std::string& currentLine = lines[currentIndex];
                returnValue.push_back(currentLine);
                
                if(currentLine.find_first_of('[') != std::string::npos) complexity++;
                if(currentLine.find_first_of(']') != std::string::npos) 
                {
                    if(complexity > 0)
                    {
                        complexity--;
                    } 
                    else
                    {
                        // found endline
                        break;
                    }
                }
                
                currentIndex++;
            }

            index += returnValue.size();
            return returnValue;
        }
    };
}