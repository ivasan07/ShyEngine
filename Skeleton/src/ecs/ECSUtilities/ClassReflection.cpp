#include "ClassReflection.h"

#include "Components/BoxBody.h"
#include "Components/ChainBody.h"
#include "Components/CircleBody.h"
#include "Components/EdgeBody.h"
#include "Components/Image.h"
#include "Components/OverlayElement.h"
#include "Components/OverlayImage.h"
#include "Components/OverlayText.h"
#include "Components/PhysicBody.h"
#include "Components/Transform.h"



ClassReflection::ClassReflection(){

	reflectionMethods["BoxBody"] = &ClassReflection::ReflectBoxBody;
	reflectionMethods["ChainBody"] = &ClassReflection::ReflectChainBody;
	reflectionMethods["CircleBody"] = &ClassReflection::ReflectCircleBody;
	reflectionMethods["EdgeBody"] = &ClassReflection::ReflectEdgeBody;
	reflectionMethods["Image"] = &ClassReflection::ReflectImage;
	reflectionMethods["OverlayElement"] = &ClassReflection::ReflectOverlayElement;
	reflectionMethods["OverlayImage"] = &ClassReflection::ReflectOverlayImage;
	reflectionMethods["OverlayText"] = &ClassReflection::ReflectOverlayText;
	reflectionMethods["PhysicsBody"] = &ClassReflection::ReflectPhysicsBody;
	reflectionMethods["Transform"] = &ClassReflection::ReflectTransform;

}ClassReflection::~ClassReflection(){

}


	void ClassReflection::ReflectComponent(std::string const& component, ECS::Component* pointer, std::map<std::string, std::string> const& map){
	if(reflectionMethods.contains(component))
		(this->*reflectionMethods[component])(pointer, map);

}
	void ClassReflection::ReflectBoxBody(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		BoxBody* self = static_cast<BoxBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));

}
	void ClassReflection::ReflectChainBody(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		ChainBody* self = static_cast<ChainBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));

}
	void ClassReflection::ReflectCircleBody(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		CircleBody* self = static_cast<CircleBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));

}
	void ClassReflection::ReflectEdgeBody(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		EdgeBody* self = static_cast<EdgeBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));

}
	void ClassReflection::ReflectImage(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		Image* self = static_cast<Image*>(selfComp);
		if(map.contains("fileName"))
			self->fileName = map.at("fileName");

}
	void ClassReflection::ReflectOverlayElement(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		OverlayElement* self = static_cast<OverlayElement*>(selfComp);
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

}
	void ClassReflection::ReflectOverlayImage(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		OverlayImage* self = static_cast<OverlayImage*>(selfComp);
		if(map.contains("path"))
			self->path = map.at("path");

}
	void ClassReflection::ReflectOverlayText(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
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

}
	void ClassReflection::ReflectPhysicsBody(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		PhysicsBody* self = static_cast<PhysicsBody*>(selfComp);
		if(map.contains("bodyType"))
			self->bodyType = std::stoi(map.at("bodyType"));
		if(map.contains("mass"))
			self->mass = std::stof(map.at("mass"));
		if(map.contains("bounciness"))
			self->bounciness = std::stof(map.at("bounciness"));

}
	void ClassReflection::ReflectTransform(ECS::Component* selfComp, std::map<std::string, std::string> const& map){
		Transform* self = static_cast<Transform*>(selfComp);
		if(map.contains("position_"))
			self->position_ = map.at("position_");
		if(map.contains("scale_"))
			self->scale_ = map.at("scale_");
		if(map.contains("rotation_"))
			self->rotation_ = std::stod(map.at("rotation_"));

}
