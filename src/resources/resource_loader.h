//resource_loader.h

#pragma once

#include <string>
#include <memory>
#include <map>
#include "object/object.h"


namespace Renderer
{
	class ShaderProgram;
}

class ResourceLoader : Object
{
public:
	ResourceLoader();
	ResourceLoader(const std::string& executable_path);
	~ResourceLoader() = default;

	ResourceLoader(const ResourceLoader&) = delete;
	ResourceLoader& operator=(const ResourceLoader&) = delete;
	ResourceLoader& operator=(const ResourceLoader&&) = delete;
	ResourceLoader(ResourceLoader&&) = delete;

	std::string get_file_text(const std::string& relative_file_path) const;
	std::shared_ptr<Renderer::ShaderProgram> load_shaders(const std::string& shader_name, const std::string& vertex_path, const std::string& fragment_path);
	std::shared_ptr<Renderer::ShaderProgram> get_shader_program(const std::string& shader_name);

    static std::string get_absolute_path(const std::string& relative_path);
	static std::string get_executable_path();

private:

	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shader_programs;

	std::string m_path;

};