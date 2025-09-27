#pragma once 
#include <filesystem>
#include <string> 
#include <any>
#include <vector>


namespace of
{
    using Buffer = std::vector<std::string>;
    
    class Document;

    struct Generic final 
    {
        enum struct Type { None, String, Double, Float };

        Type m_Type{Type::None};
        std::any m_Data{};
        bool m_Valid{false};

        template <typename T>
        [[nodiscard]] T As()
        {
            return std::any_cast<T>(m_Data);
        }

        explicit Generic(const std::string& declaration, const Document& document);
        [[nodiscard]] const bool IsValid() const;
    };

    struct Variable final
    {
        enum struct Type { None, Value, Object, Array,  };

        std::string m_Name{};
        std::any m_Data{}; 
        Type m_Type{Type::None};
        bool m_Valid{false};

        explicit Variable(const std::string& name, const std::string& declaration, const Document& document);
        
        // Object AsObject();
        template <typename T>
        std::vector<T> AsArray() 
        {
            if(m_Type != Type::Array)
                return {};
            
            std::vector<of::Generic>& variables = std::any_cast<std::vector<Generic>&>(m_Data);
            std::vector<T> returnValues;
            for(auto& variable : variables) 
            {
                returnValues.push_back(variable.As<T>());
            }

            return returnValues;
        }

        template <typename T>
        T AsValue()
        {
            if(m_Type != Type::Value) 
                return {};
            
            of::Generic& variable = std::any_cast<Generic&>(m_Data);
            
            return variable.As<T>();
        }

        // template<typename T>
        // T As()
        // {       //std::string, double, float
        //     static_cast(std::is_same<T, std::string>::value
        //                 || std::is_same<T, double>::value
        //                 || std::is_same<T, float>::value);
        
        //     switch(m_Type)
        //     {
        //         case Type::
        //     }
            
        // };  

        [[nodiscard]] const bool IsValid() const;
    };

    struct Object final
    {
        std::vector<Variable> m_Variables;
        std::string m_Name;
        std::string m_Type;

        const Variable& operator[](const std::string& name);

        explicit Object(const std::string& type, const std::string& name, const of::Buffer& buffer, const Document& document);
    };

    class Reader final
    {
        of::Buffer m_Buffer{};
    public:
        explicit Reader(const std::filesystem::path& filePath);
        [[nodiscard]] const of::Buffer& Buffer() const;
    };
    class Writer final {};

    class Document final
    {
        friend Object;
        friend Variable;
        friend Generic;
    public:
        const bool Decompile(const of::Buffer& buffer);
        // [[nodiscard]] Buffer&& Compile() const;  

        const Object& operator[](const std::string& name);

        std::vector<const Object&> GetObjectsOfType(const std::string& type);
        const Object& GetObjectOfName(const std::string& name);

    private:
        std::vector<Variable> m_Constants{};
        std::vector<Object> m_Objects{};

        [[nodiscard]] of::Buffer ToBufferArray(const std::string& line) const; 
    };
}