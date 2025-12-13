#include "material.h"

Material::Material()
{
}

void Material::set_shader(const std::shared_ptr<Shader>& shader) {
    m_shader = shader;
}

void Material::apply() {
    if (m_shader) {
        m_shader->use();
    }
}