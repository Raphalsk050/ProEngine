#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <gtc/type_ptr.hpp>
#include <fstream>
#include "Core/Time.h"
#include "PEPCH.h"
#include "Config.h"

namespace ProEngine
{
    namespace Utils
    {
        static GLenum ShaderTypeFromString(const std::string& type)
        {
            if (type == "vertex") return GL_VERTEX_SHADER;
            if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

            PENGINE_CORE_ASSERT(false, "Unknown shader type!");
            return 0;
        }

        static const char* GLShaderStageToString(GLenum stage)
        {
            switch (stage)
            {
            case GL_VERTEX_SHADER:
                return "GL_VERTEX_SHADER";
            case GL_FRAGMENT_SHADER:
                return "GL_FRAGMENT_SHADER";
            }
            PENGINE_CORE_ASSERT(false);
            return nullptr;
        }
    } // namespace Utils

    OpenGLShader::OpenGLShader(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0)
    {
        PENGINE_PROFILE_FUNCTION();

        PENGINE_CORE_INFO("Loading shader from file: {}", filepath);

        std::string source = ReadFile(filepath);
        auto shaderSources = PreProcess(source);

        CreateProgram(shaderSources);

        // Extract name from filepath
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        m_Name = filepath.substr(lastSlash, count);

        PENGINE_CORE_INFO("Shader '{}' created successfully with ID: {}", m_Name, m_RendererID);
    }

    OpenGLShader::OpenGLShader(const std::string& name,
                               const std::string& vertexSrc,
                               const std::string& fragmentSrc)
        : m_Name(name), m_RendererID(0)
    {
        PENGINE_PROFILE_FUNCTION();

        PENGINE_CORE_INFO("Creating shader '{}' from source strings", name);

        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;

        CreateProgram(sources);

        PENGINE_CORE_INFO("Shader '{}' created successfully with ID: {}", m_Name, m_RendererID);
    }

    OpenGLShader::~OpenGLShader()
    {
        PENGINE_PROFILE_FUNCTION();

        if (m_RendererID != 0)
        {
            glDeleteProgram(m_RendererID);
            PENGINE_CORE_TRACE("Deleted shader program with ID: {}", m_RendererID);
        }
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        PENGINE_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);

        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
                PENGINE_CORE_TRACE("Successfully read shader file: {} ({} bytes)", filepath, size);
            }
            else
            {
                PENGINE_CORE_ERROR("Could not read from file '{}'", filepath);
            }
        }
        else
        {
            PENGINE_CORE_ERROR("Could not open file '{}'", filepath);
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        PENGINE_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);

        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            PENGINE_CORE_ASSERT(eol != std::string::npos, "Syntax error");

            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            PENGINE_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            PENGINE_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");

            pos = source.find(typeToken, nextLinePos);

            std::string shaderSource = (pos == std::string::npos)
                                           ? source.substr(nextLinePos)
                                           : source.substr(nextLinePos, pos - nextLinePos);

            GLenum shaderType = Utils::ShaderTypeFromString(type);
            shaderSources[shaderType] = shaderSource;

            PENGINE_CORE_TRACE("Preprocessed shader type: {} ({} characters)",
                               Utils::GLShaderStageToString(shaderType), shaderSource.length());
        }

        return shaderSources;
    }

    void OpenGLShader::CompileShaders(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        // Esta função não é mais necessária pois a compilação acontece diretamente no CreateProgram
    }

    void OpenGLShader::CreateProgram(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        PENGINE_PROFILE_FUNCTION();

        // Verificar se temos pelo menos vertex e fragment shaders
        if (shaderSources.find(GL_VERTEX_SHADER) == shaderSources.end())
        {
            PENGINE_CORE_ERROR("Missing vertex shader in shader sources");
            return;
        }

        if (shaderSources.find(GL_FRAGMENT_SHADER) == shaderSources.end())
        {
            PENGINE_CORE_ERROR("Missing fragment shader in shader sources");
            return;
        }

        GLuint program = glCreateProgram();
        if (program == 0)
        {
            PENGINE_CORE_ERROR("Failed to create shader program");
            return;
        }

        std::vector<GLuint> shaderIDs;

        // Compilar cada shader
        for (auto&& [stage, source] : shaderSources)
        {
            PENGINE_CORE_TRACE("Compiling shader stage: {}", Utils::GLShaderStageToString(stage));

            GLuint shaderID = glCreateShader(stage);
            if (shaderID == 0)
            {
                PENGINE_CORE_ERROR("Failed to create shader object for stage: {}", Utils::GLShaderStageToString(stage));
                continue;
            }

            const char* sourceCStr = source.c_str();
            glShaderSource(shaderID, 1, &sourceCStr, nullptr);
            glCompileShader(shaderID);

            // Verificar compilação
            GLint isCompiled = 0;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

                if (maxLength > 0)
                {
                    std::vector<GLchar> infoLog(maxLength);
                    glGetShaderInfoLog(shaderID, maxLength, &maxLength, infoLog.data());
                    PENGINE_CORE_ERROR("Shader compilation failed for {}:\n{}",
                                       Utils::GLShaderStageToString(stage), infoLog.data());

                    // Log do código fonte para debug
                    PENGINE_CORE_ERROR("Shader source that failed to compile:\n{}", source);
                }
                else
                {
                    PENGINE_CORE_ERROR("Shader compilation failed with no error message for {}",
                                       Utils::GLShaderStageToString(stage));
                }

                glDeleteShader(shaderID);
                continue;
            }

            PENGINE_CORE_TRACE("Successfully compiled shader: {}", Utils::GLShaderStageToString(stage));
            shaderIDs.push_back(shaderID);
            glAttachShader(program, shaderID);
        }

        if (shaderIDs.empty())
        {
            PENGINE_CORE_ERROR("No shaders compiled successfully");
            glDeleteProgram(program);
            return;
        }

        // Linkar programa
        PENGINE_CORE_TRACE("Linking shader program");
        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            if (maxLength > 0)
            {
                std::vector<GLchar> infoLog(maxLength);
                glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
                PENGINE_CORE_ERROR("Shader linking failed:\n{}", infoLog.data());
            }
            else
            {
                PENGINE_CORE_ERROR("Shader linking failed with no error message");
            }

            glDeleteProgram(program);

            // Limpar shaders individuais
            for (auto id : shaderIDs)
            {
                glDeleteShader(id);
            }
            return;
        }

        PENGINE_CORE_TRACE("Shader program linked successfully");

        // Limpar shaders individuais (já não são necessários após linking)
        for (auto id : shaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }

        m_RendererID = program;
    }

    void OpenGLShader::Bind() const
    {
        PENGINE_PROFILE_FUNCTION();
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        PENGINE_PROFILE_FUNCTION();
        glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        PENGINE_PROFILE_FUNCTION();
        UploadUniformInt(name, value);
    }

    void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
    {
        UploadUniformIntArray(name, values, count);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        PENGINE_PROFILE_FUNCTION();
        UploadUniformFloat(name, value);
    }

    void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
    {
        PENGINE_PROFILE_FUNCTION();
        UploadUniformFloat2(name, value);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        PENGINE_PROFILE_FUNCTION();
        UploadUniformFloat3(name, value);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        PENGINE_PROFILE_FUNCTION();
        UploadUniformFloat4(name, value);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        PENGINE_PROFILE_FUNCTION();
        UploadUniformMat4(name, value);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
#ifdef FENGINE_SHADER_DEBUG
    FENGINE_CORE_WARN("Uniform '{}' not found in shader '{}'", name, m_Name);
#endif
            return;
        }
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
#ifdef FENGINE_SHADER_DEBUG
    FENGINE_CORE_WARN("Uniform '{}' not found in shader '{}'", name, m_Name);
#endif
            return;
        }
        glUniform1iv(location, count, values);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
#ifdef FENGINE_SHADER_DEBUG
    FENGINE_CORE_WARN("Uniform '{}' not found in shader '{}'", name, m_Name);
#endif
            return;
        }
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
#ifdef FENGINE_SHADER_DEBUG
    FENGINE_CORE_WARN("Uniform '{}' not found in shader '{}'", name, m_Name);
#endif
            return;
        }
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
#ifdef FENGINE_SHADER_DEBUG
    FENGINE_CORE_WARN("Uniform '{}' not found in shader '{}'", name, m_Name);
#endif
            return;
        }
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
#ifdef FENGINE_SHADER_DEBUG
    FENGINE_CORE_WARN("Uniform '{}' not found in shader '{}'", name, m_Name);
#endif
            return;
        }
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
#ifdef FENGINE_SHADER_DEBUG
    FENGINE_CORE_WARN("Uniform '{}' not found in shader '{}'", name, m_Name);
#endif
            return;
        }
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
#ifdef FENGINE_SHADER_DEBUG
    FENGINE_CORE_WARN("Uniform '{}' not found in shader '{}'", name, m_Name);
#endif
            return;
        }
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
} // namespace ForgeEngine
