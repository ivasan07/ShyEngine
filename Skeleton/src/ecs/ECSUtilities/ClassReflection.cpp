#include "ClassReflection.h"

#include "Components/Animation.h"
#include "Components/BoxBody.h"
#include "Components/ChainBody.h"
#include "Components/CircleBody.h"
#include "Components/EdgeBody.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlayButton.h"
#include "Components/OverlayImage.h"
#include "Components/OverlayText.h"
#include "Components/PhysicBody.h"
#include "Components/Transform.h"



ClassReflection::ClassReflection(){

	reflectionMethods["Animation"] = &ClassReflection::ReflectAnimation;
	reflectionMethods["BoxBody"] = &ClassReflection::ReflectBoxBody;
	reflectionMethods["ChainBody"] = &ClassReflection::ReflectChainBody;
	reflectionMethods["CircleBody"] = &ClassReflection::ReflectCircleBody;
	reflectionMethods["EdgeBody"] = &ClassReflection::ReflectEdgeBody;
	reflectionMethods["Image"] = &ClassReflection::ReflectImage;
	reflectionMethods["Overlay"] = &ClassReflection::ReflectOverlay;
	reflectionMethods["OverlayButton"] = &ClassReflection::ReflectOverlayButton;
	reflectionMethods["OverlayImage"] = &ClassReflection::ReflectOverlayImage;
	reflectionMethods["OverlayText"] = &ClassReflection::ReflectOverlayText;
	reflectionMethods["PhysicBody"] = &ClassReflection::ReflectPhysicBody;
	reflectionMethods["Transform"] = &ClassReflection::ReflectTransform;

}ClassReflection::~ClassReflection(){

}


	void ClassReflection::ReflectComponent(std::string const& component, ECS::Component* pointer, std::unordered_map<std::string, std::string> const& map){
	if(reflectionMethods.contains(component))
		(this->*reflectionMethods[component])(pointer, map);

}
	void ClassReflection::ReflectAnimation(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		Animation* self = static_cast<Animation*>(selfComp);
		if(map.contains("speed"))
			self->speed = std::stof(map.at("speed"));
		if(map.contains("currentFrame"))
			self->currentFrame = std::stoi(map.at("currentFrame"));
		if(map.contains("currentAnimation"))
			self->currentAnimation = std::stoi(map.at("currentAnimation"));
		if(map.contains("animationColumns"))
			self->animationColumns = std::stoi(map.at("animationColumns"));
		if(map.contains("animationRows"))
			self->animationRows = std::stoi(map.at("animationRows"));
		if(map.contains("animationSheetStyle"))
			self->animationSheetStyle = std::stoi(map.at("animationSheetStyle"));
		if(map.contains("animationLenght"))
			self->animationLenght = std::stoi(map.at("animationLenght"));

}
	void ClassReflection::ReflectBoxBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		BoxBody* self = static_cast<BoxBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));
		if(map.contains("trigger"))
			self->trigger = map.at("trigger") == "true" ? true : false;

}
	void ClassReflection::ReflectChainBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		ChainBody* self = static_cast<ChainBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));
		if(map.contains("trigger"))
			self->trigger = map.at("trigger") == "true" ? true : false;

}
	void ClassReflection::ReflectCircleBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		CircleBody* self = static_cast<CircleBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));
		if(map.contains("trigger"))
			self->trigger = map.at("trigger") == "true" ? true : false;

}
	void ClassReflection::ReflectEdgeBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		EdgeBody* self = static_cast<EdgeBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));
		if(map.contains("trigger"))
			self->trigger = map.at("trigger") == "true" ? true : false;

}
	void ClassReflection::ReflectImage(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		Image* self = static_cast<Image*>(selfComp);
		if(map.contains("fileName"))
			self->fileName = map.at("fileName");

}
	void ClassReflection::ReflectOverlay(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		Overlay* self = static_cast<Overlay*>(selfComp);
		if(map.contains("placement"))
			self->placement = std::stoi(map.at("placement"));
		if(map.contains("position"))
			self->position = map.at("position");
		if(map.contains("size"))
			self->size = map.at("size");
		if(map.contains("anchor"))
			self->anchor = map.at("anchor");
		if(map.contains("left"))
			self->left = std::stoi(map.at("left"));
		if(map.contains("top"))
			self->top = std::stoi(map.at("top"));
		if(map.contains("right"))
			self->right = std::stoi(map.at("right"));
		if(map.contains("bottom"))
			self->bottom = std::stoi(map.at("bottom"));
		if(map.contains("color"))
			self->color = Utilities::Color::CreateColor(map.at("color"));
		if(map.contains("renderScale"))
			self->renderScale = std::stof(map.at("renderScale"));
		if(map.contains("interactable"))
			self->interactable = map.at("interactable") == "true" ? true : false;

}
	void ClassReflection::ReflectOverlayButton(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		OverlayButton* self = static_cast<OverlayButton*>(selfComp);
		if(map.contains("defaultColor"))
			self->defaultColor = Utilities::Color::CreateColor(map.at("defaultColor"));
		if(map.contains("hoverColor"))
			self->hoverColor = Utilities::Color::CreateColor(map.at("hoverColor"));
		if(map.contains("downColor"))
			self->downColor = Utilities::Color::CreateColor(map.at("downColor"));
		if(map.contains("scale"))
			self->scale = std::stof(map.at("scale"));

}
	void ClassReflection::ReflectOverlayImage(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		OverlayImage* self = static_cast<OverlayImage*>(selfComp);
		if(map.contains("path"))
			self->path = map.at("path");

}
	void ClassReflection::ReflectOverlayText(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		OverlayText* self = static_cast<OverlayText*>(selfComp);
		if(map.contains("pointSize"))
			self->pointSize = std::stoi(map.at("pointSize"));
		if(map.contains("path"))
			self->path = map.at("path");
		if(map.contains("text"))
			self->text = map.at("text");
		if(map.contains("horizontalAlignment"))
			self->horizontalAlignment = std::stoi(map.at("horizontalAlignment"));
		if(map.contains("verticalAlignment"))
			self->verticalAlignment = std::stoi(map.at("verticalAlignment"));
		if(map.contains("fit"))
			self->fit = std::stoi(map.at("fit"));
		if(map.contains("lineSpacing"))
			self->lineSpacing = std::stoi(map.at("lineSpacing"));

}
	void ClassReflection::ReflectPhysicBody(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		PhysicBody* self = static_cast<PhysicBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));
		if(map.contains("trigger"))
			self->trigger = map.at("trigger") == "true" ? true : false;

}
	void ClassReflection::ReflectTransform(ECS::Component* selfComp, std::unordered_map<std::string, std::string> const& map){
		Transform* self = static_cast<Transform*>(selfComp);
		if(map.contains("localPosition"))
			self->localPosition = map.at("localPosition");
		if(map.contains("localScale"))
			self->localScale = map.at("localScale");
		if(map.contains("localRotation"))
			self->localRotation = std::stod(map.at("localRotation"));

}
