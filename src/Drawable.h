#ifndef TPM2_GEOM_DRAWABLE_H
#define TPM2_GEOM_DRAWABLE_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include <vector>
#include <iostream>

class Drawable {
public:
    virtual void Draw(Shader &shader, unsigned int primitive_type) = 0;
    virtual ~Drawable();

    void translate(glm::vec3 translation);
    void rotate(float angle, glm::vec3 rotation);
    void scale(glm::vec3 scale);
    void setModel(const glm::mat4 &model);
    glm::mat4 getModel() const;

protected:
    glm::mat4 m_translation{glm::mat4(1.0f)};
    glm::mat4 m_rotation{glm::mat4(1.0f)};
    glm::mat4 m_scaling{glm::mat4(1.0f)};

};


#endif //TPM2_GEOM_DRAWABLE_H
