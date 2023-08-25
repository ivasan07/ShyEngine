#include "OverlayText.h"
#include "ResourcesManager.h"
#include "RendererManager.h"

#include "Entity.h"
#include "Font.h"
#include "Texture.h"
#include "Overlay.h"
#include "SDL.h"
#include "SDL_ttf.h"

#include "CheckML.h"


ECS::OverlayText::OverlayText()
{
	fontSize = 32;
	font = "Assets\\Default.ttf";

	fontPtr = nullptr;
	texture = nullptr;

	fit = 0;
	horizontalAlignment = 0;
	verticalAlignment = 0;

	lineSpacing = 50;

	text = "MUY BUENOS DIAS MI CABALLERO";
}

ECS::OverlayText::~OverlayText()
{
	freeTexture();
}

void ECS::OverlayText::init()
{
	overlay = entity->getComponent<ECS::Overlay>();

	if (overlay == nullptr) {
		printError("Entity does not contain overlay component. Removing component", "OverlayImage");
		this->remove();
		return;
	}

	if (fontPtr == nullptr)
		createFont();
	if (texture == nullptr)
		createTexture();
}

void ECS::OverlayText::createFont()
{
	fontPtr = Resources::ResourcesManager::instance()->addFont(font, fontSize);
}

void ECS::OverlayText::freeTexture()
{
	if (texture != nullptr) {
		delete texture;
		texture = nullptr;
	}
}

void ECS::OverlayText::createTexture()
{
	freeTexture();

	if ((Fit)fit == Fit::WrapClamp || (Fit)fit == Fit::WrapOverflow)
		texture = fontPtr->CreateWrappedText(text, overlay->GetSize().getX());
	else
		texture = fontPtr->CreateText(text);
}

void ECS::OverlayText::render()
{
	if (texture == nullptr)
		return;

	SDL_Rect destination = { 0, 0, 0, 0 };

	overlay->GetRenderRect(destination.x, destination.y, destination.w, destination.h);

	SDL_Rect source = { 0, 0, texture->getWidth(), texture->getHeight() };

	switch ((Fit)fit) {

	case Fit::WrapClamp:
	case Fit::Clamp: {

		int w = std::min(source.w, destination.w);
		int h = std::min(source.h, destination.h);

		if (horizontalAlignment != (int)HorizontalAlignment::Left) {

			float mult = horizontalAlignment == (int)HorizontalAlignment::Center ? 0.5f : 1;

			source.x = std::max(0, source.w - w) * mult;
			destination.x += std::max(0, destination.w - w) * mult;
		}

		if (verticalAlignment != (int)VerticalAlignment::Top) {

			float mult = verticalAlignment == (int)VerticalAlignment::Center ? 0.5f : 1;

			source.y = std::max(0, source.h - h) * mult;
			destination.y += std::max(0, destination.h - h) * mult;
		}

		source.w = w;
		source.h = h;

		destination.w = source.w;
		destination.h = source.h;

	}

	case Fit::WrapOverflow:
	case Fit::Overflow: {

		float multx = horizontalAlignment == (int)HorizontalAlignment::Left ? 0 : horizontalAlignment == (int)HorizontalAlignment::Center ? 0.5f : 1;
		float multy = verticalAlignment == (int)VerticalAlignment::Top ? 0 : verticalAlignment == (int)VerticalAlignment::Center ? 0.5f : 1;

		destination.x += (destination.w - source.w) * multx;
		destination.y += (destination.h - source.h) * multy;

		destination.h = source.h;

		destination.w = source.w;
		break;
	}

	default:
		break;
	}


	overlay->RenderTexture(texture, &source, &destination);
}


int ECS::OverlayText::GetFit()
{
	return fit;
}

void ECS::OverlayText::SetFit(int fit)
{
	if (fontPtr != nullptr && this->fit / 2 != fit / 2)
	{
		this->fit = fit;
		createTexture();
	}
	else
		this->fit = fit;
}

int ECS::OverlayText::GetVerticalAlignment()
{
	return verticalAlignment;
}

int ECS::OverlayText::GetHorizontalAlignment()
{
	return horizontalAlignment;
}

void ECS::OverlayText::SetVerticalAlignment(int align)
{
	verticalAlignment = align;
}

void ECS::OverlayText::SetHorizontalAlignment(int align)
{
	horizontalAlignment = align;
}

void ECS::OverlayText::SetFont(std::string font)
{
	font = font;
	if (this->fontPtr != nullptr) {
		createFont();
		createTexture();
	}
}

std::string ECS::OverlayText::GetFont()
{
	return font;
}

std::string ECS::OverlayText::GetText()
{
	return text;
}

void ECS::OverlayText::SetText(std::string text)
{
	this->text = text;

	if (fontPtr != nullptr) {
		createTexture();
	}
}

int ECS::OverlayText::GetPointSize()
{
	return fontSize;
}

void ECS::OverlayText::SetPointSize(int size)
{
	fontSize = size;
	if (fontPtr != nullptr) {
		createFont();
		createTexture();
	}
}
