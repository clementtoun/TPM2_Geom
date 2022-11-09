#include "Drawable.h"

void Drawable::translate(const glm::vec3 translation, const int index) {
    if(index < (int)m_model.size())
        m_model[index] = glm::translate(m_model[index], translation);
    else
        std::cout << "m_model[index] doesn't exist !" << std::endl;
}

void Drawable::rotate(const float angle, const glm::vec3 axe, const int index) {
    if(index < (int)m_model.size())
        m_model[index] = glm::rotate(m_model[index], glm::radians(angle), axe);
    else
        std::cout << "m_model[index] doesn't exist !" << std::endl;
}

void Drawable::scale(const glm::vec3 scale, const int index) {
    if(index < (int)m_model.size())
        m_model[index] = glm::scale(m_model[index], scale);
    else
        std::cout << "m_model[index] doesn't exist !" << std::endl;
}

void Drawable::setModel(const glm::mat4 &model, const int index) {
    if(index < (int)m_model.size())
        m_model[index] = model;
    else
        std::cout << "m_model[index] doesn't exist !" << std::endl;
}

Drawable::~Drawable(){
    m_model.clear();
}

void Drawable::add_model(const glm::mat4 &model) {
    m_model.emplace_back(model);
}