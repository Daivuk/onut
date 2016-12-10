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
#if defined(WIN32)
        fopen_s(&pFic, filename.c_str(), "rb");
#else
        pFic = fopen(filename.c_str(), "rb");
#endif
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

    Shader::ParsedElements Shader::parseElements(std::string& content, const std::string& type)
    {
        ParsedElements ret;

        std::regex inputReg(type + "\\s+([\\w]+)\\s+([\\w]+)\\s*;");
        std::smatch match;
        auto searchStart = content.cbegin();
        size_t position = 0;
        while (std::regex_search(searchStart, content.cend(), match, inputReg))
        {
            ParsedElement element;

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

    Shader::ParsedUniforms Shader::parseUniforms(std::string& content)
    {
        ParsedUniforms ret;

        // Create uniforms
        std::smatch match;
        size_t offset = 0;
        while (std::regex_search(content.cbegin() + offset, content.cend(), match, std::regex("extern\\s+([\\w]+)\\s+([\\w]+)")))
        {
            ParsedUniform uniform;

            uniform.type = match[1].str();
            uniform.name = match[2].str();

            content[offset + match.position()] = '/';
            content[offset + match.position() + 1] = '*';
            content[offset + match.position() + match.length() - 2] = '*';
            content[offset + match.position() + match.length() - 1] = '/';

            offset += match.position() + match.length();

            ret.push_back(uniform);
        }

        return std::move(ret);
    }

    Shader::ParsedTextures Shader::parseTextures(std::string& content)
    {
        ParsedTextures ret;

        std::smatch match;
        size_t offset = 0;
        while (std::regex_search(content.cbegin() + offset, content.cend(), match, std::regex("Texture([\\d]+)\\s+([\\w]+)\\s+\\{")))
        {
            ParsedTexture texture;

            texture.index = std::stoi(match[1].str());
            texture.name = match[2].str();

            std::smatch match2;
            size_t propsStart = offset + match.position() + match.length();
            if (std::regex_search(content.cbegin() + propsStart, content.cend(), match2, std::regex("\\}")))
            {
                auto propsEnd = propsStart + match2.position() + match2.length();
                std::smatch matchProperty;
                while (std::regex_search(content.cbegin() + propsStart, content.cbegin() + propsEnd, matchProperty, std::regex("([\\w]+)\\s*=\\s*([\\w]+)\\s*;")))
                {
                    const auto& propName = matchProperty[1].str();
                    const auto& propValue = matchProperty[2].str();
                    if (propName == "filter")
                    {
                        if (propValue == "nearest")
                        {
                            texture.filter = ParsedTexture::Filter::Nearest;
                        }
                        else if (propValue == "linear")
                        {
                            texture.filter = ParsedTexture::Filter::Linear;
                        }
                        else if (propValue == "bilinear")
                        {
                            texture.filter = ParsedTexture::Filter::Bilinear;
                        }
                        else if (propValue == "trilinear")
                        {
                            texture.filter = ParsedTexture::Filter::Trilinear;
                        }
                        else if (propValue == "anisotropic")
                        {
                            texture.filter = ParsedTexture::Filter::Anisotropic;
                        }
                        else assert(false); // Invalid filter
                    }
                    else if (propName == "repeat")
                    {
                        if (propValue == "clamp")
                        {
                            texture.repeatX = texture.repeatY = ParsedTexture::Repeat::Clamp;
                        }
                        else if (propValue == "wrap")
                        {
                            texture.repeatX = texture.repeatY = ParsedTexture::Repeat::Wrap;
                        }
                        else assert(false); // Invalid repeat
                    }
                    else if (propName == "repeatX")
                    {
                        if (propValue == "clamp")
                        {
                            texture.repeatX = ParsedTexture::Repeat::Clamp;
                        }
                        else if (propValue == "wrap")
                        {
                            texture.repeatX = ParsedTexture::Repeat::Wrap;
                        }
                        else assert(false); // Invalid repeat
                    }
                    else if (propName == "repeatY")
                    {
                        if (propValue == "clamp")
                        {
                            texture.repeatY = ParsedTexture::Repeat::Clamp;
                        }
                        else if (propValue == "wrap")
                        {
                            texture.repeatY = ParsedTexture::Repeat::Wrap;
                        }
                        else assert(false); // Invalid repeat
                    }
                    else assert(false); // Invalid texture sampler property

                    propsStart += matchProperty.position() + matchProperty.length();
                }

                propsStart = propsEnd;
            }
            else assert(false); // No matching closing } to texture sample

            content[offset + match.position()] = '/';
            content[offset + match.position() + 1] = '*';
            content[propsStart - 2] = '*';
            content[propsStart - 1] = '/';

            offset += match.position() + match.length() + match2.position() + match2.length();

            ret.push_back(texture);
        }

        return std::move(ret);
    }

    Shader::ParsedVS Shader::parseVertexShader(const std::string& filename)
    {
        ParsedVS ret;

        std::string content = readShaderFileContent(filename);

        onut::stripOutComments(content);

        ret.inputs = parseElements(content, "input");
        ret.outputs = parseElements(content, "output");
        ret.uniforms = parseUniforms(content);
        ret.source = std::move(content);

        return std::move(ret);
    }

    Shader::ParsedPS Shader::parsePixelShader(const std::string& filename)
    {
        ParsedPS ret;

        std::string content = readShaderFileContent(filename);

        onut::stripOutComments(content);

        ret.inputs = parseElements(content, "input");
        ret.uniforms = parseUniforms(content);
        ret.textures = parseTextures(content);
        ret.source = std::move(content);

        return std::move(ret);
    }
};

OShaderRef OGetShader(const std::string& name)
{
    return oContentManager->getResourceAs<OShader>(name);
}
