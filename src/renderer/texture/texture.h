#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>

class Texture {
public:
    Texture();
    explicit Texture(const std::string& path);
    ~Texture();
    
    bool load_from_file(const std::string& path);
    void create(int width, int height, GLenum format = GL_RGBA);
    void create_from_data(int width, int height, const unsigned char* data, GLenum format = GL_RGBA);
    
    void bind(unsigned int unit = 0) const;
    void unbind() const;
    
    GLuint get_id() const { return m_id; }
    glm::ivec2 get_size() const { return m_size; }
    int get_width() const { return m_size.x; }
    int get_height() const { return m_size.y; }
    
    void set_filter_min(GLenum filter);
    void set_filter_mag(GLenum filter);
    void set_wrap_s(GLenum wrap);
    void set_wrap_t(GLenum wrap);
    
    static std::shared_ptr<Texture> load(const std::string& path);
    
private:
    GLuint m_id = 0;
    glm::ivec2 m_size = glm::ivec2(0, 0);
    GLenum m_format = GL_RGBA;
    
    void setup_default_parameters();
};

#endif // TEXTURE_H