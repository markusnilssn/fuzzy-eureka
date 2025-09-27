#include "of.h"
#include <sstream>
#include <iostream>
#include <fstream>

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

    void remove_indentation(std::string& line)
    {
        if(line.size() == 0)
        {
            return;
        }

        if(!::isspace(line[0]))
        {
            return;
        }

        size_t index = std::string::npos;
        for (size_t i = 0; i < line.size(); i++)
        {
            if(!::isspace(line[i]))
            {
                index = i;
                break;
            }
        }

        line = line.substr(index, index - (line.size()));
    }
}

of::Reader::Reader(const std::filesystem::path &filePath)
{
    if(!std::filesystem::is_regular_file(filePath) || filePath.extension() != ".of")
    {
        std::cerr << "File is not the correct format! " << filePath << std::endl;
        return;
    }   

    std::ifstream stream(filePath, std::ifstream::in);
    if(!stream.is_open())
    {
        std::cerr << "Failed to open file " << filePath << std::endl;
        return;
    }

    of::Buffer buffer{};
    std::string line;

    int complexity = 0;
    std::stringstream ss;

    int index;
    while(std::getline(stream, line))
    {           
        if(line.empty()) continue;

        std::trim(line);
        std::remove_indentation(line);

        if(line.rfind('#', 0) == 0) continue; // remove comments
        
        for(size_t i = 0; i < line.size(); i++)
        {
            if(line[i] == '[') ++complexity;
            if(line[i] == ']') --complexity;
        }

        size_t endline = line.find(';');
        if(endline == std::string::npos || complexity > 0)
        {
            ss << line;
            continue;
        }

        if(!ss.str().empty())
        {
            ss << line; // add endline
            std::cout << ss.str() << std::endl;
            buffer.push_back(ss.str());
            ss.str(""); // clear the string content
            
            complexity = 0;
            continue;
        }
        else 
        {
            std::cout << line << std::endl;
            buffer.push_back(line);
        }
    }

    m_Buffer = std::move(buffer);
}

const of::Buffer &of::Reader::Buffer() const
{
    return m_Buffer;
}


static std::tuple<std::string, std::string, std::string> GetContext(const std::string& line)
{
    size_t spacing = line.find_first_of(' ');
    size_t separator = line.find_first_of(':'); // Find that's not :: and only 

    if(separator == std::string::npos)
    {
        std::cerr << "failed to parse line " << line << std::endl;
        return {};
    }

    int additional = 1;
    if(spacing == std::string::npos || spacing > separator)
    {
        additional = 0;
        spacing = 0;
    }

    std::string type = line.substr(0, spacing);
    std::string name = line.substr(spacing + additional, ((separator - spacing) - additional));

    if(name.empty())
    {
        std::cerr << "failed to find name for line " << line << std::endl;
        return {};
    }

    size_t endline = line.find_last_of(';');
    std::string declaration = line.substr(separator + 1, (endline - separator) - 1);
    
    std::remove_indentation(declaration); // remove spacing 

    return std::make_tuple(type, name, declaration);
}

const bool of::Document::Decompile(const of::Buffer &buffer)
{  
    m_Objects.clear();
    m_Constants.clear();

    for(size_t i = 0; i < buffer.size(); i++)
    {
        const std::string line = buffer[i];

        if(line.rfind("import", 0) == 0)
        {
            continue;
        }
        else 
        {
            size_t spacing = line.find_first_of(' ');
            size_t separator = line.find_first_of(':');
        
            if(spacing == std::string::npos || separator == std::string::npos || spacing > separator)
            {
                std::cerr << "failed to parse line " << line << std::endl;
                continue;
            }
        
            std::string type = line.substr(0, spacing);
            std::string name = line.substr(spacing + 1, ((separator - 1) - spacing));
        
            std::string declaration = line.substr(separator + 1, line.find_last_of(';'));
            
            if(type.compare("namespace") == 0) 
            {
                of::Buffer buffer = ToBufferArray(line);

                for(size_t i = 0; i < buffer.size(); i++)
                {
                    std::string& line = buffer[i];
                    size_t deliminator = line.find_first_of(' ');
                    line.insert(deliminator + 1, name + "::");
        
                    std::tuple<std::string, std::string, std::string> context = GetContext(line);
            
                    // const std::string& type = std::get<0>(data);
                    const std::string& name = std::get<1>(context);
                    const std::string& declaration = std::get<2>(context);
                                
                    Variable variable(name, declaration, *this);
                    m_Constants.push_back(variable);
                }


                // of::Buffer data = Unwrap(declaration);
                // for (size_t i = 0; i < data.size(); i++)
                // {
                //     Variable variable(data[i], *this);
                //     constants.push_back(variable);
                // }
            }
            else if(type.compare("const") == 0) 
            {
                // find name and declaration;

                std::tuple<std::string, std::string, std::string> context = GetContext(line);
            
                // const std::string& type = std::get<0>(data);
                const std::string& name = std::get<1>(context);
                const std::string& declaration = std::get<2>(context);
                            
                Variable variable(name, declaration, *this);
                m_Constants.push_back(variable);
            }
            else 
            {
                if(line.find_first_of('[') != std::string::npos && line.find_last_of(']') != std::string::npos) 
                {
                    of::Buffer data = ToBufferArray(declaration);
                    Object object(type, name, data, *this);
                    m_Objects.push_back(object);
                }
                else 
                {
                    of::Buffer data;
                    data.push_back(line);

                    Object object(type, name, data, *this);
                    m_Objects.push_back(object);
                }
            }
        }
    }

    return true;
}

const of::Object& of::Document::operator[](const std::string &name)
{
    auto predicate = [name](const Object& object) -> bool
    {
        return object.m_Name == name;
    };

    auto iterator = std::find_if(m_Objects.begin(), m_Objects.end(), predicate);
    if(iterator == m_Objects.end()) 
    {
        throw std::runtime_error("Failed to find object");
    }
    
    return (*iterator);
}

of::Buffer of::Document::ToBufferArray(const std::string &line) const
{
    size_t begin = line.find_first_of('[');
    size_t end = line.find_last_of(']');

    std::string data = line.substr(begin + 1, (end - begin) - 1);

    of::Buffer returnValue;
    std::stringstream ss(data);
    std::string item;

    while(std::getline(ss, item, ';')) 
    {
        // removed support for "," within a object
        // size_t count = std::count(item.begin(), item.end(), ',');
        // if(count > 0)
        // {
        //     size_t amount = 0;
        //     size_t position = 0;
        //     for(size_t i = 0; i < item.size(); i++)
        //     {
        //         char& c = item[i];
        //         if(c == ',')
        //         {
        //             returnValue.push_back(item.substr(position, i - 1));
        //             position = i + 1;
        //             ++amount;
        //             if((i + 1) < item.size() && count == amount) // missing , at end of object
        //             {
        //                 returnValue.push_back(item.substr(position, item.size()));
        //             }
        //         }
        //     }
        // }
        // else 
        // {
            item += ';';
            returnValue.push_back(item);
        // }
    }

    return returnValue;
}

of::Object::Object(const std::string &type, const std::string &name, const of::Buffer &buffer, const Document &document)
    : m_Type(type)
    , m_Name(name)
{
    for (size_t i = 0; i < buffer.size(); i++)
    {
        const std::string& line = buffer[i];
        
        std::tuple<std::string, std::string, std::string> data = GetContext(line);

        const std::string& name = std::get<1>(data);
        const std::string& declaration = std::get<2>(data); 
        
        Variable variable = Variable(name, declaration, document);
        if(variable.IsValid()) 
        {
            m_Variables.emplace_back(std::move(variable));
        }
    }   
}

const of::Variable& of::Object::operator[](const std::string& name) 
{
    auto predicate = [name](const of::Variable& variable) -> bool 
    {
        return variable.m_Name == name;
    };

    auto iterator = std::find_if(m_Variables.begin(), m_Variables.end(), predicate);
    if(iterator == m_Variables.end())
    {
        throw std::runtime_error("Failed to find variable");
    }

    return (*iterator);
}

of::Variable::Variable(const std::string& name, const std::string& declaration, const Document& document)
{
    // check if variable?
    auto& constants = document.m_Constants;
    auto predicate = [declaration](const of::Variable variable) -> bool
    {
        // handle namespace here! 
        return variable.m_Name == declaration;
    };
    auto iterator = std::find_if(constants.begin(), constants.end(), predicate);
    if(iterator != constants.end())
    {
        auto variable = (*iterator);
        m_Type = variable.m_Type;
        m_Data = variable.m_Data;
        m_Valid = true;
        m_Name = name;
    }
    else 
    {
        size_t start, end;

        start = declaration.find_first_of('{');
        end = declaration.find_last_of('}');

        if(start != std::string::npos && end != std::string::npos)  
        {      
            // Get array
            std::string data = declaration.substr(start + 1, (end - start) - 1);

            std::stringstream ss(data);
            std::string item;

            std::vector<Generic> values;
            while(std::getline(ss, item, ',')) 
            {   
                // item += ','; // this looks really wierd but i want it to be the same for all declaration values to have the endline within the text
                std::remove_indentation(item);
                values.push_back(Generic(item, document));
            }

            m_Name = name;
            m_Type = Type::Array;
            m_Data = values;
            m_Valid = true;

            // parse values as generic 
            return;
        } 

        start = declaration.find_first_of('[');
        end = declaration.find_last_of(']');    

        if(start != std::string::npos && end != std::string::npos) 
        {
            m_Name = name;
            m_Type = Type::Object;
            // m_Data = values;
            m_Valid = false;
            std::cerr << "object not yet available" << std::endl;
            return;
        }

        // parse regular expression

        m_Name = name;
        m_Type = Type::Value;
        m_Data = Generic(declaration, document);
        m_Valid = true;
    }
}

const bool of::Variable::IsValid() const 
{
    return m_Valid;
}

of::Generic::Generic(const std::string& declaration, const Document& document)
{
    if(std::count(declaration.begin(), declaration.end(), '"') == 2) // String
    {
        size_t first = declaration.find_first_of('"');
        size_t end = declaration.find_last_of('"');

        std::string data = declaration.substr(first + 1, (end - first) - 1);

        std::remove_indentation(data);

        m_Type = Type::String;
        m_Valid = true;
        m_Data = data;
    }   
    else
    {
        if(declaration.find('.') != std::string::npos)
        {
            m_Data = std::stof(declaration);
            m_Type = Type::Float;
            m_Valid = true;
        }
        else 
        {
            m_Data = std::stod(declaration);
            m_Type = Type::Double;
            m_Valid = true;
        }
    }
}

const bool of::Generic::IsValid() const 
{
    return m_Valid;
}