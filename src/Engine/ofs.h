// /*
//     ofs - Object File Structure
//     A classic File Format but with namespaces, comments and variables
// */

// #pragma once 

// #include <filesystem>
// #include <string> 
// #include <unordered_map>
// #include <any>
// #include <fstream>
// #include <iostream>
// #include <set>
// #include <string_view>
// #include <sstream>
// #include <tuple>
// #include <utility>
// #include <iomanip> // for std::quoted

// namespace std
// {
//     void trim(std::string& line)
//     {
//         line.erase(0, line.find_first_not_of("\t"));
//         line.erase(line.find_last_not_of("\t\r\n") + 1);
//     }

//     void remove_whitespace(std::string& line)
//     {
//         line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
//     }

//     void remove_indentation(std::string& line)
//     {
//         if(line.size() == 0)
//         {
//             return;
//         }

//         if(!::isspace(line[0]))
//         {
//             return;
//         }

//         size_t index = std::string::npos;
//         for (size_t i = 0; i < line.size(); i++)
//         {
//             if(!::isspace(line[i]))
//             {
//                 index = i;
//                 break;
//             }
//         }

//         line = line.substr(index, index - (line.size()));
//     }
// }

// namespace ofs
// {
//     struct Vector2 
//     {
//         float x{};
//         float y{};
//     };
//     struct Point 
//     {
//         int x{};
//         int y{};    
//     };

//     struct Constant
//     {
//         enum struct Type { Vector2, Point, String, Double, Short, Any };

//         std::string m_Namespace;
//         std::string m_Name;

//         std::any m_Value;
//         Type m_Type;
//         bool m_Valid;

//         explicit Constant(const std::string& line, const std::string& prefix)
//             : m_Valid(false)
//         {
//             size_t spacing = line.find_first_of(' ');
//             size_t separator = line.find_first_of(':');

//             if(spacing == std::string::npos || separator == std::string::npos || spacing > separator)
//             {
//                 std::cerr << "failed to parse name " << line << std::endl;
//                 return;
//             }

//             std::string name = line.substr(spacing + 1, ((separator - 1) - spacing));
            
//             size_t endline = line.find_last_of(';');
//             if(endline == std::string::npos)
//             {
//                 std::cerr << "failed to find endline" << std::endl;
//                 return;
//             }
            
//             std::string rvalue = line.substr(separator + 1, (endline - separator) - 1);
            
//             std::remove_indentation(rvalue);

//             if(rvalue.find('{') != std::string::npos && rvalue.find('}') != std::string::npos) 
//             {

//                 for(size_t i = 0; i < rvalue.size(); i++)
//                 {
//                     ::isnumber(rvalue[i]);
//                 }
//             } 
//             else if(rvalue.find('[') != std::string::npos && rvalue.find(']') != std::string::npos) 
//             {

//             } 
//             else if(std::count(rvalue.begin(), rvalue.end(), '"') > 2) 
//             {
//                 m_Value = rvalue;
//                 m_Type = Type::String;
//                 m_Valid = true;
//                 m_Name = name;
//             }
//             else 
//             {
//                 // parse int 
//             }
            
//         }

//         [[nodiscard]] const bool IsValid() const { return m_Valid; }
//     };

//     struct Value { };
//     struct Array { };
//     struct Object { };

//     static size_t ToUUID(const std::string& string)
//     {
//         return std::hash<std::string>{}(string);
//     }

//     struct File 
//     {
//         std::set<size_t> existing; // string to uuid 
//         std::vector<Constant> constants;
//         std::vector<Object> objects;

//         explicit File(const std::filesystem::path& path) 
//         {
//             if(!std::filesystem::is_regular_file(path) || path.extension() != ".ojs")
//             {
//                 std::cerr << "File is not the correct format! " << path << std::endl;
//                 return;
//             }   

//             std::ifstream stream(path.c_str(), std::ifstream::in);
//             if(!stream.is_open())
//             {
//                 std::cerr << "Failed to open file " << path << std::endl;
//                 return;
//             }

//             std::vector<std::string> lines;
//             {
//                 std::string line;
//                 while(std::getline(stream, line))
//                 {
//                     std::trim(line);
                    
//                     if(line.empty()) continue;
//                     if(line.rfind('#', 0) == 0) continue;
                    
//                     std::remove_indentation(line);

//                     lines.push_back(line);
//                 }
//             }
            
//             size_t index = 0;
//             while(index < lines.size())
//             {
//                 std::string& line = lines[index];

//                 if (line.rfind("import", 0) == 0) // todo: implement 
//                 { 
//                     size_t spacing = line.find_first_of(' ');
//                     size_t endline = line.find_first_of(';');

//                     std::string filename = line.substr(spacing + 1, ((endline - 1) - spacing));
                    
//                     ++index;
//                     continue; // import file
//                 }
//                 else 
//                 {
//                     size_t spacing = line.find_first_of(' ');
//                     size_t separator = line.find_first_of(':');

//                     if(spacing == std::string::npos || separator == std::string::npos || spacing > separator)
//                     {
//                         std::cerr << "failed to parse line " << line << std::endl;
//                         ++index;
//                         continue;
//                     }

//                     std::string type = line.substr(0, spacing);
//                     std::string name = line.substr(spacing + 1, ((separator - 1) - spacing));

//                     if(type.empty() || name.empty())
//                     {
//                         std::cerr << "failed to parse line " << line << std::endl;
//                         ++index;
//                         continue;
//                     }

//                     if(type.compare("const") == 0) {}
//                     else if(type.compare("namespace") == 0) // Namespace can only contains declaration a
//                     {
//                         [[maybe_unused]] std::vector<std::string> collection = std::move(GetObject(lines, index));
//                         if(collection.size() > 2)
//                         {
//                             std::vector<std::string> variables(collection.begin() + 1, collection.end() - 1); // slice declaration and ending
//                             for (size_t i = 0; i < variables.size(); i++)
//                             {
//                                 Constant constant(variables[i], name);
//                                 if(constant.IsValid())
//                                 {
//                                     constants.emplace_back(std::move(constant));
//                                 }
//                             }
//                         } 

//                         continue;
//                     }
//                     else 
//                     {
//                         if(line.find_first_of('[') != std::string::npos) // Object 
//                         {
//                             [[maybe_unused]] std::vector<std::string> collection = std::move(GetObject(lines, index));
//                             if(collection.size() > 2)
//                             {
//                                 std::vector<std::string> variables(collection.begin() + 1, collection.end() - 1); // slice declaration and ending
//                                 for (size_t i = 0; i < variables.size(); i++)
//                                 {
//                                     Constant constant(variables[i], name);
//                                     if(constant.IsValid())
//                                     {
//                                         constants.emplace_back(std::move(constant));
//                                     }
//                                 }
//                             } 

//                             continue;   
//                         }
//                         else // declaration 
//                         {
//                             size_t end = line.find_last_of(';');
//                             std::string rvalue = line.substr((separator + 1), (end - 1) - separator);

//                             std::cout << type << " " << name << ": " << rvalue << std::endl;
//                         }
//                     }
//                 }

//                 ++index;
//             }

            
//             stream.close();
//         }
        
//         // type, name
//         static std::pair<std::string, std::string> GetDeclaration(const std::string& line) 
//         {
//             size_t split = line.find_first_of(' ');

//             if(split == std::string::npos || split >= line.size())
//             {
//                 return {};
//             }

//             std::string type = line.substr(0, split);
//             std::string name = line.substr(split + 1, (line.size() - split));

//             return std::make_pair<std::string, std::string>(std::move(type), std::move(name));
//         }

//         // Todo: remove declaration and end declaration lines 
//         static std::vector<std::string> GetObject(const std::vector<std::string>& lines, size_t& index)
//         {
//             size_t currentIndex = index + 1; // start on next line
//             short complexity = 0; 

//             std::vector<std::string> returnValue;
//             returnValue.push_back(lines[index]);

//             while(currentIndex < lines.size())
//             {
//                 const std::string& currentLine = lines[currentIndex];
//                 returnValue.push_back(currentLine);
                
//                 if(currentLine.find_first_of('[') != std::string::npos) complexity++;
//                 if(currentLine.find_first_of(']') != std::string::npos) 
//                 {
//                     if(complexity > 0)
//                     {
//                         complexity--;
//                     } 
//                     else
//                     {
//                         // found endline
//                         break;
//                     }
//                 }
                
//                 currentIndex++;
//             }

//             index += returnValue.size();
//             return returnValue;
//         }
//     };
// }