#include "material.h"
#include "renderer/shader/shader.h"

Material::Material() {
    // Конструктор по умолчанию
}

void Material::set_shader(const std::shared_ptr<Shader>& shader) {
    m_shader = shader;
}

void Material::apply() {
    if (m_shader) {
        m_shader->use();
    }
}