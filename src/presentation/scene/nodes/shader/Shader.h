#ifndef SHADER_H
#define SHADER_H

#include "../../../../../lib/glad/glad_glfw.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>
#include <variant>
#include <stack>
#include <map>
#include "../../tree/SceneNode.h"

using ShaderType = std::variant<bool, int, float, glm::vec3, glm::vec4, glm::mat4>;
using UniformMap = std::map<std::string, std::stack<ShaderType>>;

class Shader : public SceneNode {
public:
    unsigned int ID;
    std::string vertexPath;
    std::string geometryPath;
    std::string fragmentPath;
    int maxUniformNameLength;
    int uniformCount;
    std::unique_ptr<char[]> uniformNameBuffer;
    bool active;

    Shader(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath) : SceneNode(std::move(name)),
    vertexPath(std::move(vertexShaderPath)), geometryPath(), fragmentPath(std::move(fragmentShaderPath)) {
        initShader(vertexPath, geometryPath, fragmentPath);
    }
    Shader(std::string name, std::string vertexShaderPath, std::string geometryShaderPath, std::string fragmentShaderPath) :
    SceneNode(std::move(name)), vertexPath(std::move(vertexShaderPath)), geometryPath(std::move(geometryShaderPath)),
    fragmentPath(std::move(fragmentShaderPath)) {
        initShader(vertexPath, geometryPath, fragmentPath);
    }

    std::string getTypeName() override { return "Shader"; };

    void setActive(bool toggle) { active = toggle; };

    unsigned int loadShader(const std::string& path, int type, const std::string& name) {
        unsigned int shader;
        std::string code;
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try{
            file.open(path);
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();
            code = stream.str();
        }
        catch (std::ifstream::failure &e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char *pCode = code.c_str();
        shader = glCreateShader(type);
        glShaderSource(shader, 1, &pCode, NULL);
        glCompileShader(shader);
        checkCompileErrors(shader, name);

        return shader;
    }

    void initShader(const std::string& vertexShaderPath, const std::string& geometryShader, const std::string& fragmentShaderPath) {
        unsigned int vertex, geometry, fragment;
        vertex = loadShader(vertexShaderPath, GL_VERTEX_SHADER, "VERTEX");
        if(!geometryShader.empty()) geometry = loadShader(geometryShader, GL_GEOMETRY_SHADER, "GEOMETRY");
        fragment = loadShader(fragmentShaderPath, GL_FRAGMENT_SHADER, "FRAGMENT");
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        if(!geometryShader.empty()) glAttachShader(ID, geometry);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        glDeleteShader(vertex);
        if(!geometryShader.empty()) glDeleteShader(geometry);
        glDeleteShader(fragment);
        initFields();
    }

    void initFields() {
        glGetProgramiv(ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
        glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &uniformCount);
        uniformNameBuffer = std::unique_ptr<char[]>(new char[maxUniformNameLength]);
        active = true;
    }

    // ------------------------------------------------------------------------
    // Activate the shader
    void use() const {
        glUseProgram(ID);
    }

    // ------------------------------------------------------------------------
    // Hot-reload the shader
    void hotReload() {
        glDeleteProgram(ID);
        initShader(vertexPath, geometryPath, fragmentPath);
        // What about uniforms that have to be set on init (like color in flat).
    }
    // ------------------------------------------------------------------------
    // uniform setter

    void setUniforms(UniformMap &uniforms) const {
        int length, size;
        unsigned int type;

        use();
        for (int i = 0; i < uniformCount; i++) {
            glGetActiveUniform(ID, (GLuint) i, maxUniformNameLength, &length, &size, &type, uniformNameBuffer.get());
//            std::cerr <<"name: " << uniformNameBuffer << ", length: " << length << ", size: " << size << ", type: " << type << std::endl;
            auto stack = uniforms[uniformNameBuffer.get()];
            if (stack.empty()) continue;
            setUniform(uniformNameBuffer.get(), stack.top());
        }
    }

    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };

    void setUniform(const std::string &name, ShaderType &value) const {
        std::visit(overloaded{
                [&](bool &v) { glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) v); },
                [&](int &v) { glUniform1i(glGetUniformLocation(ID, name.c_str()), v); },
                [&](float &v) { glUniform1f(glGetUniformLocation(ID, name.c_str()), v); },
                [&](glm::vec3 &v) { glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &v[0]); },
                [&](glm::vec4 &v) { glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &v[0]); },
                [&](glm::mat4 &v) { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &v[0][0]); }
        }, value);
    };

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                          << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                          << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    int acceptVisit(SceneNodeVisitor &visitor) override {
        return visitor.visitShader(*this);
    }

    int acceptLeave(SceneNodeVisitor &visitor) override {
        return visitor.leaveShader(*this);
    }

};

#endif