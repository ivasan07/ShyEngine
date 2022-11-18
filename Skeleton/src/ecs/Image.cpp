#include "Image.h"
#include "Entity.h"
#include "Transform.h"
#include <SDL_image.h>

namespace ECS {

	Image::Image(const std::string& fileName) {
		renderer = nullptr; texture = nullptr; transform = nullptr; rotationPoint = nullptr;
		width = height = 0;

		flipX = flipY = false;

		this->fileName = fileName;
	}

	Image::~Image() {
		if (texture != nullptr)
			SDL_DestroyTexture(texture);

		delete rotationPoint;
	}
	
	void Image::init() {

		renderer = RendererManager::RendererManager::instance()->getRenderer();

		// Surface and texture
		SDL_Surface* surface = IMG_Load(fileName.c_str());
		assert(surface != nullptr, "Couldn't load image: " + fileName);

		texture = SDL_CreateTextureFromSurface(renderer, surface);
		assert(texture != nullptr, "Couldn't load image: " + fileName);

		width = surface->w; height = surface->h;
		SDL_FreeSurface(surface);

		// Flip
		flipMode();

		// Rotation point
		rotationPoint = new SDL_Point({ width / 2, height / 2 });
	}

	void Image::start() {
		transform = getEntity()->getComponent<Transform>();
	}

	void Image::render() {

		srcRect = { 0, 0, width, height };

		int x = std::round(transform->getPosition().getX());
		int y = std::round(transform->getPosition().getY());
		int w = std::round(transform->getScale().getX() * width);
		int h = std::round(transform->getScale().getY() * height);

		dstRect = { x, y, w, h };

		SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, transform->getRotation(), rotationPoint, mode);
	}

	int Image::getWidth() {
		return width;
	}

	int Image::getHeight() {
		return height;
	}

	void Image::setWidth(float width) {
		this->width = width;
	}

	void Image::setHeight(float height) {
		this->height = height;
	}

	void Image::setSize(float width, float height) {
		this->width = width; this->height = height;
	}

	void Image::setFlipX(bool flip) {
		flipX = flip;
		flipMode();
	}

	void Image::setFlipY(bool flip) {
		flipY = flip;
		flipMode();
	}

	void Image::setRotaionPoint(int x, int y) {
		rotationPoint->x = x;
		rotationPoint->y = y;
	}

	void Image::flipMode() {
		if (!flipX && !flipY) mode = SDL_FLIP_NONE;
		else {

			assert(!flipX && flipY || flipX && !flipY, "Image can not be fliped horizontally and vertically.");

			if (flipX) mode = SDL_FLIP_HORIZONTAL;
			else mode = SDL_FLIP_VERTICAL;
		}
	}

}