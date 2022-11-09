#ifndef TPM2_GEOM_DRAWABLE_H
#define TPM2_GEOM_DRAWABLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <vector>
#include <iostream>

class Drawable {
public:
    virtual void Draw(Shader &shader) = 0;
    virtual ~Drawable();

    void translate(glm::vec3 translation, int index = 0);
    void rotate(float angle, glm::vec3 rotation, int index = 0);
    void scale(glm::vec3 scale, int index = 0);
    void setModel(const glm::mat4 &model, int index = 0);
    void add_model(const glm::mat4 &model);

protected:
    std::vector<glm::mat4> m_model{glm::mat4(1.0f)};
};


#endif //TPM2_GEOM_DRAWABLE_H
