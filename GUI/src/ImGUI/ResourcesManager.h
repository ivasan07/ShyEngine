#pragma once

#include <unordered_map>
#include <string>

namespace ShyEditor {

	template<typename T>
	using resources_map = std::unordered_map<std::string, T>;

	class Texture;
	class Font;

	struct Asset {

		std::string name;
		std::string extension;
		std::string path;
		std::string relativePath;

		bool valid = true;
	};

	class ResourcesManager {

	public:

		static const std::string ASSETSFOLDER;

		static ResourcesManager* GetInstance();

		Texture* AddTexture(const std::string& key, bool isEditorResource);
		Font* AddFont(const std::string& key, int pointSize);

		static std::string GetEditorResourcesPath();
		static void SetEditorResourcesPath(const std::string& path);

		static std::string GetEngineResourcesPath();
		static void SetEngineResourcesPath(const std::string& path);


		static void UnselectAsset();
		static void SelectAsset(const Asset& asset);
		static bool IsAnyAssetSelected();
		static const Asset& SelectedAsset();

		static void Init();
		static void Release();

	private:

		ResourcesManager();

		static ResourcesManager* instance;

		std::string editorResourcesPath;
		std::string engineResourcesPath;

		resources_map<Texture*> editorTextures;

		resources_map<Texture*> engineTextures;
		resources_map<Font*> engineFonts;

		Asset currentAsset;
	};

}