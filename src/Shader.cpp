#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include "../libs/glad/glad.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(std::string vertex_filename, std::string frag_filename) {

    std::string vertex_source = m_loadSourceFromFile(vertex_filename);
    std::string frag_source = m_loadSourceFromFile(frag_filename);

    unsigned int vertex_id, fragment_id;
    int success;
    char infoLog[512];

    const char* tempV = vertex_source.c_str();

    vertex_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_id, 1, &tempV, NULL);
    glCompileShader(vertex_id);
    glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        exit(-1);
    };

    const char* tempF = frag_source.c_str();

    fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_id, 1, &tempF, NULL);
    glCompileShader(fragment_id);

    glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        exit(-1);
    };

    m_ID = glCreateProgram();

    glAttachShader(m_ID, vertex_id);
    glAttachShader(m_ID, fragment_id);
    glLinkProgram(m_ID);

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
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
