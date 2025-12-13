#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <string>
#include "renderer/shader/shader.h"

class Material {
public:
    Material();
    virtual ~Material() = default;
    
    void set_shader(const std::shared_ptr<Shader>& shader);
    std::shared_ptr<Shader> get_shader() const { return m_shader; }
    
    void apply();
    
private:
    std::shared_ptr<Shader> m_shader;
};

#endif // MATERIAL_H