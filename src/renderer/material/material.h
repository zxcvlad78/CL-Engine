#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <string>

class Shader;

class Material {
public:
    Material();
    virtual ~Material() = default;
    
    void set_shader(const std::shared_ptr<Shader>& shader);
    std::shared_ptr<Shader> get_shader() const { return m_shader; }
    
    virtual void apply() = 0;
    
private:
    std::shared_ptr<Shader> m_shader;
};

#endif // MATERIAL_H