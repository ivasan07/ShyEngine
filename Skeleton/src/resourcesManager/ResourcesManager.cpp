#include "ResourcesManager.h"
#include <RendererManager.h>
#include <SoundEffect.h>
#include <Texture.h>
#include <Music.h>
#include <Font.h>

Resources::ResourcesManager::ResourcesManager() {}

Resources::ResourcesManager::~ResourcesManager() {

	for (auto it = textures.begin(); it != textures.end(); it++) delete it->second;
	textures.clear();

	for (auto it = fonts.begin(); it != fonts.end(); it++) delete it->second;
	fonts.clear();

	for (auto it = sounds.begin(); it != sounds.end(); it++) delete it->second;
	sounds.clear();

	for (auto it = music.begin(); it != music.end(); it++) delete it->second;
	music.clear();
}

Renderer::Texture* Resources::ResourcesManager::addTexture(const std::string& key) {

	if (textures.contains(key))
		return textures.at(key);

	Renderer::Texture* t = new Renderer::Texture(key);

	textures.insert(std::make_pair(key, t));

	return t;

}

Renderer::Font* Resources::ResourcesManager::addFont(std::string const& key, int pointSize) {

	std::string newkey = key + std::to_string(pointSize);

	if (fonts.contains(newkey))
		return fonts.at(newkey);

	Renderer::Font* t = new Renderer::Font(key, pointSize);

	fonts.insert(std::make_pair(newkey, t));

	return t;
}

Sound::SoundEffect* Resources::ResourcesManager::addSound(std::string const& key) {

	if (sounds.contains(key))
		return sounds.at(key);

	Sound::SoundEffect* s = new Sound::SoundEffect(key);

	sounds.insert(std::make_pair(key, s));

	return s;

}

Sound::Music* Resources::ResourcesManager::addMusic(std::string const& key) {

	if (music.contains(key))
		return music.at(key);

	Sound::Music* m = new Sound::Music(key);

	music.insert(std::make_pair(key, m));

	return m;

}
