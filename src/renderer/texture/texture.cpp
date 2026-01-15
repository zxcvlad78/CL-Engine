    #include "renderer/texture/texture.h"
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
    #include "resources/resource_loader.h"
    #include <iostream>


    Texture::Texture() {}

    Texture::Texture(const std::string& path) {
        load_from_file(path);
    }

    Texture::~Texture() {
        if (m_id != 0) {
            glDeleteTextures(1, &m_id);
        }
    }

    bool Texture::load_from_file(const std::string& relative_path) {
        std::string absolute_path = ResourceLoader::get_absolute_path(relative_path);
        stbi_set_flip_vertically_on_load(true);
        
        int width, height, nr_channels;
        unsigned char* data = stbi_load(absolute_path.c_str(), &width, &height, &nr_channels, 0);
        
        if (!data) {
            std::cerr << "Failed to load texture: " << relative_path << std::endl;
            return false;
        }
        
        GLenum format;
        if (nr_channels == 1)
            format = GL_RED;
        else if (nr_channels == 3)
            format = GL_RGB;
        else if (nr_channels == 4)
            format = GL_RGBA;
        else {
            stbi_image_free(data);
            return false;
        }
        
        create_from_data(width, height, data, format);
        
        stbi_image_free(data);
        return true;
    }

    void Texture::create(int width, int height, GLenum format) {
        m_size = glm::ivec2(width, height);
        m_format = format;
        
        if (m_id == 0) {
            glGenTextures(1, &m_id);
        }
        
        glBindTexture(GL_TEXTURE_2D, m_id);
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
        
        setup_default_parameters();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::create_from_data(int width, int height, const unsigned char* data, GLenum format) {
        m_size = glm::ivec2(width, height);
        m_format = format;
        
        if (m_id == 0) {
            glGenTextures(1, &m_id);
        }
        
        glBindTexture(GL_TEXTURE_2D, m_id);
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        setup_default_parameters();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::bind(unsigned int unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void Texture::unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::set_filter_min(GLenum filter) {
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::set_filter_mag(GLenum filter) {
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::set_wrap_s(GLenum wrap) {
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::set_wrap_t(GLenum wrap) {
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::setup_default_parameters() {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    std::shared_ptr<Texture> Texture::load(const std::string& path) {
        auto texture = std::make_shared<Texture>();
        if (texture->load_from_file(path)) {
            return texture;
        }
        return nullptr;
    }