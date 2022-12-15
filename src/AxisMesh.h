#ifndef TPM2_GEOM_AXISMESH_H
#define TPM2_GEOM_AXISMESH_H

#include "Drawable.h"

struct VertexColor {
    glm::vec3 vertices;
    glm::vec3 color;
};

class AxisMesh : public Drawable {

public:
    AxisMesh();

    ~AxisMesh() override;

    void Draw(Shader &shader, unsigned int primitive_type) override;

private:
    unsigned int m_VAO{}, m_VBO{};
    std::vector<VertexColor> m_vertices;
};


#endif //TPM2_GEOM_AXISMESH_H
