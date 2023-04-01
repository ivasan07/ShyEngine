#include "OverlayElement.h"
#include "RendererManager.h"

#include "OverlayManager.h"

ECS::OverlayElement::OverlayElement() {

	parent = nullptr;
	SetPositioned({ 0, 0 }, { 100, 100 });
	SetAnchorCenter();

	render_x = render_y = render_w = render_h = 0;

	OverlayManager::instance()->AddElement(this);
}

ECS::OverlayElement::~OverlayElement()
{
	OverlayManager::instance()->RemoveElement(this);
}


int ECS::OverlayElement::GetPlacement()
{
	return placement;
}


void ECS::OverlayElement::SetDirty()
{
	OverlayManager::instance()->SetDirty();
}

void ECS::OverlayElement::SetPositioned(Utilities::Vector2D position, Utilities::Vector2D size)
{
	placement = 0;

	this->position = position;
	this->size = size;

	SetDirty();
}

void ECS::OverlayElement::SetStreched(int left, int top, int right, int bottom)
{
	placement = 1;


	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;

	SetDirty();
}

void ECS::OverlayElement::Move(Utilities::Vector2D position)
{
	SetPositioned(this->position + position, size);
}

void ECS::OverlayElement::MoveTo(Utilities::Vector2D position)
{
	SetPositioned(position, size);
}

void ECS::OverlayElement::SetSize(Utilities::Vector2D size)
{
	this->size = size;

	if (placement == (int)Placement::Positioned)
		SetDirty();
}

Utilities::Vector2D ECS::OverlayElement::GetPosition()
{
	return position;
}

Utilities::Vector2D ECS::OverlayElement::GetSize()
{
	return size;
}

int ECS::OverlayElement::GetTop()
{
	return top;
}

int ECS::OverlayElement::GetLeft()
{
	return left;
}

int ECS::OverlayElement::GetRight()
{
	return right;
}

int ECS::OverlayElement::GetBottom()
{
	return bottom;
}

void ECS::OverlayElement::SetTop(int pos)
{
	this->top = pos;

	if (placement == (int)Placement::Stretched) {
		SetDirty();
	}
}

void ECS::OverlayElement::SetLeft(int pos)
{
	this->left = pos;

	if (placement == (int)Placement::Stretched) {
		SetDirty();
	}
}

void ECS::OverlayElement::SetRight(int pos)
{
	this->right = pos;

	if (placement == (int)Placement::Stretched) {
		SetDirty();
	}
}

void ECS::OverlayElement::SetBottom(int pos)
{
	this->bottom = pos;

	if (placement == (int)Placement::Stretched) {
		SetDirty();
	}
}

Utilities::Vector2D ECS::OverlayElement::GetAnchor()
{
	return anchor;
}

void ECS::OverlayElement::SetAnchor(Utilities::Vector2D anchor)
{
	this->anchor = anchor;

	SetDirty();
}

void ECS::OverlayElement::SetAnchorCenter()
{
	SetAnchor({ 0.5f, 0.5f });
}

void ECS::OverlayElement::SetAnchorTopLeft()
{
	SetAnchor({ 0, 0 });
}

void ECS::OverlayElement::SetAnchorTopRight()
{
	SetAnchor({ 1, 0 });
}

void ECS::OverlayElement::SetAnchorBottomLeft()
{
	SetAnchor({ 0, 1 });
}

void ECS::OverlayElement::SetAnchorBottomRight()
{
	SetAnchor({ 1, 1 });
}

void ECS::OverlayElement::SetParent(OverlayElement* element)
{
	parent = element;

	SetDirty();
}

void ECS::OverlayElement::RecalculatePosition()
{
	CalculateRenderRect(render_x, render_y, render_w, render_h);
}

void ECS::OverlayElement::GetRenderRect(int& x, int& y, int& w, int& h)
{
	x = render_x;
	y = render_y;
	w = render_w;
	h = render_h;
}

void ECS::OverlayElement::CalculateRenderRect(int& x, int& y, int& w, int& h)
{

	switch ((OverlayElement::Placement)placement)
	{
	case OverlayElement::Placement::Positioned:
	{
		x = (int)std::round(position.getX());
		y = (int)std::round(position.getY());
		w = (int)std::round(size.getX());
		h = (int)std::round(size.getY());

		x -= anchor.getX() * size.getX();
		y -= anchor.getY() * size.getY();

		if (parent != nullptr) {
			auto parentPos = parent->CalculateCenterPosition();
			x += parentPos.getX();
			y += parentPos.getY();
		}

		break;
	}
	case OverlayElement::Placement::Stretched:
	{

		int parent_left, parent_top, parent_width, parent_height;

		if (parent != nullptr) {

			parent->CalculateRenderRect(parent_left, parent_top, parent_width, parent_height);
		}
		else {

			auto rend = Renderer::RendererManager::instance();
			parent_left = 0;
			parent_top = 0;
			parent_width = rend->getWidth();
			parent_height = rend->getHeight();
		}

		x = parent_left + left;
		y = parent_top + top;
		w = parent_width - right - left;
		h = parent_height - bottom - top;

		break;
	}
	default:
		//TODO: error
		
		print("Entidad con posicionamiento invalido", "OverlayElement");
		break;
	}


}

Utilities::Vector2D ECS::OverlayElement::CalculateCenterPosition()
{
	Utilities::Vector2D center;
	switch ((OverlayElement::Placement)placement)
	{
	case OverlayElement::Placement::Positioned:

		center = position;

		if (parent != nullptr)
			center += parent->CalculateCenterPosition();

		break;

	case OverlayElement::Placement::Stretched:

		int x, y, w, h;
		CalculateRenderRect(x, y, w, h);

		center = { x + w * anchor.getX(), y + h * anchor.getY() };

		break;

	}
	return center;
}