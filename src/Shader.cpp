#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include "../libs/glad/glad.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() {
    m_ID = glCreateProgram();
}

bool Shader::useShaderProgram() {
    glUseProgram(m_ID);
    return true;
}

std::string Shader::m_loadSourceFromFile(std::string filename) {
    std::ifstream theFile;
    std::stringstream theFileStream;

    theFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        theFile.open(filename.c_str());

        theFileStream << theFile.rdbuf();

        theFile.close();
    }
    catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    return theFileStream.str();
}

void Shader::setBool(const std::string &name, bool value) {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) {
    int matLoc = glGetUniformLocation(m_ID, name.c_str());
    glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec2(const std::string &name, glm::vec2 value) {
    int vecLoc = glGetUniformLocation(m_ID, name.c_str());
    glUniform2fv(vecLoc, 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, glm::vec3 value) {
    int vecLoc = glGetUniformLocation(m_ID, name.c_str());
    glUniform3fv(vecLoc, 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string &name, glm::vec4 value) {
    int vecLoc = glGetUniformLocation(m_ID, name.c_str());
    glUniform4fv(vecLoc, 1, glm::value_ptr(value));
}

void Shader::deleteProg() {
    glDeleteProgram(m_ID);
}

void Shader::attachShader(std::string shader_filename, unsigned int shader_type) {
    auto type_str = "";

    switch(shader_type){
        case GL_VERTEX_SHADER: {
            type_str = "VERTEX";
            break;
        }
        case GL_GEOMETRY_SHADER: {
            type_str = "GEOMETRY";
            break;
        }
        case GL_FRAGMENT_SHADER: {
            type_str = "FRAGMENT";
            break;
        }
        default:{
            std::cout << "ERROR::SHADER wrong shader type\n" << std::endl;
            exit(-1);
        }
    }

    std::string source = m_loadSourceFromFile(shader_filename);

    unsigned int shader_id;
    int success;
    char infoLog[512];

    const char* tempV = source.c_str();

    shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &tempV, NULL);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << type_str << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        exit(-1);
    };

    glAttachShader(m_ID, shader_id);
    glDeleteShader(shader_id);
}

void Shader::linkProgram() {
    glLinkProgram(m_ID);
}
