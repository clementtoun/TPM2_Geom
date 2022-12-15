#include "Drawable.h"

void Drawable::translate(const glm::vec3 translation) {
    m_translation = glm::translate(m_translation, translation);
}

void Drawable::rotate(const float angle, const glm::vec3 axe) {
    m_rotation = glm::rotate(m_rotation, angle, axe);
}

void Drawable::scale(const glm::vec3 scale) {
    m_scaling = glm::scale(m_scaling, scale);
}

glm::mat4 Drawable::getModel() const {
    return m_scaling * m_rotation * m_translation;
}

Drawable::~Drawable() = default;


