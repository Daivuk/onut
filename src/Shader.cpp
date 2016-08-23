// Onut
#include <onut/ContentManager.h>
#include <onut/Shader.h>
#include <onut/Strings.h>

// STL
#include <cassert>
#include <regex>

namespace onut
{
    Shader::VertexElement::VertexElement(uint32_t in_size, const std::string& in_semanticName)
        : size(in_size)
        , semanticName(in_semanticName)
    {
    }

    Shader::Shader()
    {
    }

    Shader::~Shader()
    {
    }

    Shader::Type Shader::getType() const
    {
        return m_type;
    }

    uint32_t Shader::getVertexSize() const
    {
        return m_vertexSize;
    }

    static std::string readShaderFileContent(const std::string& filename)
    {
        FILE* pFic;
        fopen_s(&pFic, filename.c_str(), "rb");
        assert(pFic);
        fseek(pFic, 0, SEEK_END);
        auto filesize = static_cast<size_t>(ftell(pFic));
        fseek(pFic, 0, SEEK_SET);
        std::string content;
        content.resize(filesize + 1);
        fread(const_cast<char*>(content.data()), 1, filesize, pFic);
        content[filesize] = '\0';
        fclose(pFic);

        return std::move(content);
    }

    Shader::ParsedVS::Elements Shader::ParsedVS::parseElement(std::string& content, const std::string& type)
    {
        ParsedVS::Elements ret;

        std::regex inputReg(type + "\\s+([\\w]+)\\s+([\\w]+)\\s*;");
        std::smatch match;
        auto searchStart = content.cbegin();
        size_t position = 0;
        while (std::regex_search(searchStart, content.cend(), match, inputReg))
        {
            ParsedVS::Element element;

            auto type = match[1].str();
            element.name = match[2].str();

            if (type == "float") element.size = 1;
            else if (type == "float2") element.size = 2;
            else if (type == "float3") element.size = 3;
            else if (type == "float4") element.size = 4;
            else assert(false);

            content[position + match.position()] = '/';
            content[position + match.position() + 1] = '*';
            content[position + match.position() + match.length() - 2] = '*';
            content[position + match.position() + match.length() - 1] = '/';

            searchStart += match.position() + match.length();
            position += match.position() + match.length();

            ret.push_back(std::move(element));
        }

        return std::move(ret);
    }

    Shader::ParsedVS Shader::parseVertexShader(const std::string& filename)
    {
        ParsedVS ret;

        std::string content = readShaderFileContent(filename);

        onut::stripOutComments(content);

        ret.inputs = ParsedVS::parseElement(content, "input");
        ret.outputs = ParsedVS::parseElement(content, "output");

        ret.source = std::move(content);

        return std::move(ret);
    }
};

OShaderRef OGetShader(const std::string& name)
{
    return oContentManager->getResourceAs<OShader>(name);
}
