#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

#include "../Game/GameStates/Level.h"

namespace RenderEngine
{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
}

class ResourceManager
{
public:
	
	static void setExecutablePath(const std::string& executablePath);
	static void unloadAllResourses();

	~ResourceManager() = delete;
	ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	static std::shared_ptr<RenderEngine::ShaderProgram> loadShaders(const std::string& shaderProgramName, const std::string& vertexPath, const std::string& fragmentPath);
	static std::shared_ptr<RenderEngine::ShaderProgram> getShaderProgram(const std::string& shaderProgramName);
	static std::shared_ptr<RenderEngine::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
	static std::shared_ptr<RenderEngine::Texture2D> getTexture(const std::string& textureName);

	static std::shared_ptr<RenderEngine::Sprite> loadSprite(const std::string& spriteName, const std::string& textureName,
		const std::string& shaderProgramName, const std::string& subTextureName = "default");

	static std::shared_ptr<RenderEngine::Sprite> getSprite(const std::string& spriteName);

	static std::shared_ptr<RenderEngine::Texture2D> loadTextureAtlas(std::string textureName, std::string texturePath,
		std::vector<std::string> subTextures, const unsigned int subTextureWidth, const unsigned int subTextureHeight);

	static bool loadJSONResources(const std::string& JSONPath);
	static bool loadJSONLevelSettings(const std::string& JSONPath);

	static const std::vector<Level::Settings>& getLevelSettings() { return m_levelSettings; }

private:
	static std::string getFileString(const std::string& relativePath);


	typedef std::map<const std::string, std::shared_ptr<RenderEngine::ShaderProgram>> ShaderProgramsMap;
	static ShaderProgramsMap m_shaderPrograms;

	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Texture2D>> TexturesMap;
	static TexturesMap m_textures;

	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Sprite>> SpritesMap;
	static SpritesMap m_sprites;

	static std::string m_path;

	static std::vector<Level::Settings> m_levelSettings;
};