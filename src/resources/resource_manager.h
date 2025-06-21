#pragma once

#include <string>

#include <memory>

class ShaderProgram;

class ResourceManager
{
public:
	ResourceManager(const std::string& executable_path);
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> load_shader

private:
};