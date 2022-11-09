#ifndef TPM2_GEOM_SHADER_H
#define TPM2_GEOM_SHADER_H

#include <glm/glm.hpp>
#include <string>
#include <map>

class Shader {
public:
    Shader(std::string vertex_filename, std::string frag_filename);
    bool useShaderProgram();
    void deleteProg();

    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setMat4(const std::string &name, glm::mat4 mat);
    void setVec3(const std::string &name, glm::vec3 value);
    void setVec4(const std::string &name, glm::vec4 value);

private:
    std::string m_loadSourceFromFile(std::string filename);

    unsigned int m_ID;
};


#endif //TPM2_GEOM_SHADER_H
