#include "OverlayImage.h"
#include "OverlayElement.h"
#include "Entity.h"
#include "ResourcesManager.h"
#include "Texture.h"
#include "RendererManager.h"

ECS::OverlayImage::OverlayImage()
{
	//TODO: Textura por defecto del juego
	path = "ground.png";
	texture = nullptr;
	overlay = nullptr;
}

void ECS::OverlayImage::start()
{
	//TODO: Ver si deberia dar error o a�adirse uno automaticamente
	overlay = entity->getComponent<OverlayElement>();

	if (overlay == nullptr) {
		printError("La entidad no tiene overlay", "OverlayImage");

	}
	assert(overlay != nullptr, "La entidad debe contener un componente Overlay");

	SetTexture(path);


}

void ECS::OverlayImage::render()
{
	SDL_Rect destination = { 0, 0, 0, 0 };
	
	overlay->GetRenderRect(destination.x, destination.y, destination.w, destination.h);

	SDL_RenderCopy(Renderer::RendererManager::instance()->getRenderer(),
		texture->getSDLTexture(), NULL, &destination);
}


void ECS::OverlayImage::SetTexture(std::string newpath)
{
	path = newpath;

	texture = Resources::ResourcesManager::instance()->addTexture(path);
}


std::string ECS::OverlayImage::GetTexture()
{
	return path;
}