#pragma once

#include <string>
#include <map>
namespace ECS { class Component; }

//Creation time : Fri Mar 24 02:12:49 2023
#define ECSreflection_Version 1.0

using namespace ECS;
class ClassReflection{
typedef void(ClassReflection::*ReflectionMethod)(ECS::Component*, std::map<std::string, std::string> const&);

	std::map<std::string, ReflectionMethod> reflectionMethods;

public: 
	static void ReflectImage(ECS::Component* selfComp, std::map<std::string, std::string> const& map);

	static void ReflectTransform(ECS::Component* selfComp, std::map<std::string, std::string> const& map);

};