//resource_loader.cpp

#include "resource_loader.h"
#include "../renderer/shader_program.h"

#include "sstream"
#include "fstream"
#include "iostream"
#include <filesystem>
#include <algorithm>

#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__)
    #include <unistd.h>
    #include <limits.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
#endif

ResourceLoader::ResourceLoader(const std::string& executable_path)
{
	size_t found = executable_path.find_last_of("/\\");
	m_path = executable_path.substr(0, found);
}

ResourceLoader::ResourceLoader()
{
	m_path = get_executable_path();
}

std::string ResourceLoader::get_executable_path()
{
    std::string path;

#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    path = buffer;
    
#elif defined(__linux__)
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer)-1);
    if (len != -1) {
        buffer[len] = '\0';
        path = buffer;
    }
    
#elif defined(__APPLE__)
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0) {
        path = buffer;
    }
#endif

    std::filesystem::path exe_path(path);
    path = exe_path.parent_path().string();
    
    std::replace(path.begin(), path.end(), '\\', '/');
    
    return path;
}

std::string ResourceLoader::get_absolute_path(const std::string& relative_file_path)
{
    std::string normalized_relative = relative_file_path;
    std::replace(normalized_relative.begin(), normalized_relative.end(), '\\', '/');
    
    std::filesystem::path full_path = std::filesystem::path( get_executable_path() ) / normalized_relative;
    std::string result = full_path.string();
    
    
    std::replace(result.begin(), result.end(), '\\', '/');
    
    return result;
}

std::string ResourceLoader::get_file_text(const std::string& relative_file_path) const
{
	std::filesystem::path full_path = std::filesystem::path(m_path) / relative_file_path;

	std::ifstream f;
	f.open(full_path, std::ios::in | std::ios::binary );

	if (!f.is_open())
	{
		std::cerr << "Cant open file at path" << " '" << relative_file_path << "'" << std::endl;
		return std::string{};

	}

	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram> ResourceLoader::load_shaders(const std::string& shader_name, const std::string& vertex_path, const std::string& fragment_path)
{
	std::string vertex_string = get_file_text(vertex_path);
	if (vertex_string.empty())
	{
		std::cerr << "Vertex shader is empty" << std::endl;
		return nullptr;
	}
	std::string fragment_string = get_file_text(fragment_path);
	if (fragment_string.empty())
	{
		std::cerr << "Fragment shader is empty" << std::endl;
		return nullptr;
	}

	std::shared_ptr<Renderer::ShaderProgram>& new_shader = m_shader_programs.emplace(shader_name, std::make_shared<Renderer::ShaderProgram>(vertex_string, fragment_string)).first -> second;
	if (!new_shader->is_compiled())
	{
		std::cerr << "Cant load shader program \n"
			<< "Vertex path: " << vertex_path << "\n"
			<< "Fragment path: " << fragment_path << std::endl;

		return nullptr;
	}

	return new_shader;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceLoader::get_shader_program(const std::string& shader_name)
{
	ShaderProgramsMap::const_iterator it = m_shader_programs.find(shader_name);
	if (it != m_shader_programs.end())
	{
		return it->second;
	}

	std::cerr << "Cant find the shader program: " << " '" << shader_name << "'" << std::endl;
	return nullptr;

}