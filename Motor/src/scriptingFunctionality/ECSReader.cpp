#include "ECSReader.h"
#include "ClassCreator.h"
#include <filesystem>
#include <StringTrim.h>
#include "EditorExport.h"
#include <Windows.h>

#define _2str(x) #x




ECSReader::ECSReader(std::string const& root) : root(root)
{
	output = root + "\\ecs\\ECSUtilities";
}

void ECSReader::AskForRoot()
{
	AskForPath("root", root);
	output = root + "\\ecs\\ECSUtilities";
}

void ECSReader::AskForOutput()
{
	AskForPath("output directory", output);
}

void ECSReader::AskForPath(std::string const& name, std::string& path)
{
	bool pathAccepted = false;
	while (!pathAccepted)
	{
		std::cout << "Current " << name << ": " << path << std::endl;
		std::cout << "Confirm? (y/n)" << std::endl;

		std::string line;
		std::getline(std::cin, line);

		if (line != "y" && line != "n" && line != "") {
			return AskForPath(name, path);
		}

		if (line == "y" || line.empty())
			break;

		std::cout << "Enter new " << name << ": " << std::endl;
		std::getline(std::cin, path);
	}
}

ECSReader& ECSReader::Read()
{
	ProcessFolder(root);
	ManageInheritance();
	return *this;
}


ECSReader& ECSReader::Method2Function() {

	return CreateFunctionManagerHeader().CreateFunctionManagerSource();
}


void ECSReader::CreateOutputFolder()
{
	std::filesystem::path path(output);
	std::filesystem::create_directories(output);
}


std::string ECSReader::GetDefaultRoot()
{
	return R"(C:\Users\sryoj\Documents\TFG\Skeleton\src)";
}


void ECSReader::ProcessFolder(std::string const& path)
{
	std::filesystem::path currentPath = std::filesystem::path(path);

	for (auto const& it : std::filesystem::directory_iterator(currentPath)) {

		if (it.is_directory() && (it.path().filename() == "scriptingFunctionality") || it.path().string() == output) {
			continue;
		}

		(this->*(it.is_directory() ? &ECSReader::ProcessFolder : &ECSReader::ProcessFile))(it.path().string());
	}
}

void ECSReader::ProcessFile(std::string const& path)
{

	//Just process .h files
	std::string extension = path.substr(path.find("."));

	if (extension != ".h") {
		return;
	}


	std::ifstream stream(path);

	bool containsPublish = false;
	bool fileIncluded = false;
	std::string currentClassName = "";


	bool currentClassIsManager = false;

	while (stream) {
		std::string line;

		std::getline(stream, line);
		line = Utilities::trim(line);

		if (line == "")
			continue;

		if (line.contains("ECS_Version")) {

			ECS_Version = Utilities::trim(line.substr(line.find("ECS_Version") + 12));
			continue;
		}

		if (line.rfind("//", 0) == 0) //Ignora comentarios
			continue;

		if (line == _2str(publish) ":") {
			containsPublish = true;
			continue;
		}

		if (line == "public:" || line == "private:" || line == "protected:" || line == "}" || line == "};") {
			containsPublish = false;
			continue;
		}

		bool forceAddClassName = false;
		std::string textToRemove = "";

		if (line.contains("class")) {

			if (line.contains(";")) //Forward declaration -> ignore
				continue;

			if (line.contains("enum")) //TODO: enum processing
				continue;

			textToRemove = "class";
			currentClassIsManager = false;
		}
		else if (line.contains(_2str(EditorComponent)))
		{
			size_t idx = line.find(_2str(EditorComponent));
			if (idx > 0 && line[idx - 1] == '<')
			{
				//Es un comentario
				continue;
			}

			textToRemove = _2str(EditorComponent);
			forceAddClassName = true;
			currentClassIsManager = false;
		}
		else if (line.contains(_2str(EditorManager)))
		{
			size_t idx = line.find(_2str(EditorManager));
			if (idx > 0 && line[idx - 1] == '<')
			{
				//Es un comentario
				continue;
			}

			textToRemove = _2str(EditorManager);
			currentClassIsManager = true;
		}


		if (textToRemove.size() > 0) {

			std::string className = line.erase(0, line.find(textToRemove) + textToRemove.size());

			if (className.contains("{")) {
				className = className.substr(0, className.find("{"));
			}

			size_t space = className.find(":");
			if (space != std::string::npos)
				className = className.substr(0, space);

			currentClassName = Utilities::trim(className);


			if (forceAddClassName) {

				allComponents.emplace(currentClassName);
			}



			if (line.contains(":") && (line.contains("Component") || line.contains("PhysicBody"))) {

				if (currentClassName == "PhysicBody")
					continue;

				if (line.contains("PhysicBody")) {
					classInheritance[currentClassName] = "PhysicBody";
					if (!fileIncluded) {

						filesToInclude.push_back(path);
						fileIncluded = true;
					}
				}

				componentsWithPaths.push_back(std::make_pair(currentClassName, path));

				if (!allComponents.contains(currentClassName))
					allComponents.emplace(currentClassName);
			}

			continue;
		}

		if (line.rfind(_2str(reflect), 0) == 0) {

			std::string removeReflect = line.substr(8);

			int blankSpace = removeReflect.find(" ");

			std::string attReturnType = removeReflect.substr(0, blankSpace);
			std::string attNameWithComa = removeReflect.substr(blankSpace + 1);
			std::string attName = attNameWithComa.substr(0, attNameWithComa.find(";"));

			attributes[currentClassName].push_back({ attReturnType, attName });

			if (!allComponents.contains(currentClassName))
				allComponents.emplace(currentClassName);

			if (!fileIncluded) {

				filesToInclude.push_back(path);
				fileIncluded = true;
			}

			continue;
		}

		if (currentClassName.size() > 0 && containsPublish) { //Si 

			Method method = CreateMethod(line, currentClassName);

			if (!method.valid)
			{
				HANDLE hConsole;

				hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

				SetConsoleTextAttribute(hConsole, 12);
				std::cout << "Error: ";
				SetConsoleTextAttribute(hConsole, 15);

				std::cout << "Comportamiento no valido.Hay un atributo dentro de la marca " _2str(publish) << "\n";
				std::cout << "Linea: " + line << "\nEn la clase " + currentClassName << std::endl;
				continue;
			}

			if (!currentClassIsManager) {

				methods[currentClassName].push_back(method);

				if (!allComponents.contains(currentClassName))
					allComponents.emplace(currentClassName);
			}

			else
				managerMethods[currentClassName].push_back(method);


			if (!fileIncluded) {

				if (currentClassIsManager)
					managerFiles.push_back(path);

				else filesToInclude.push_back(path);
				fileIncluded = true;
			}
		}
	}


	stream.close();
}


//Cuando un componente hereda de otro, incluimos los atributos de la clase padre en la clase hijo

void ECSReader::ManageInheritance()
{
	for (auto& inheritance : classInheritance) {

		std::string const& childClass = inheritance.first;
		std::string const& parentClass = inheritance.second;

		if (attributes.contains(parentClass)) {


			if (!attributes.contains(childClass)) {

				attributes[childClass];
			}

			for (auto& attr : attributes[parentClass])
			{
				attributes[childClass].push_back(attr);
			}
		}

	}
}



ECSReader::Method ECSReader::CreateMethod(std::string const& line, std::string const& className)
{
	Method method;

	method.className = className;

	std::string l = Utilities::trim(line);

	size_t nextWord = l.find(" ");
	method.returnType = l.substr(0, nextWord);

	l = l.substr(nextWord + 1);
	nextWord = l.find("(");

	if (nextWord == std::string::npos) {

		method.valid = false;
		return method;
	}

	method.methodName = Utilities::trim(l.substr(0, nextWord));

	l = l.substr(nextWord + 1);

	bool isType = true;
	std::string returnType;
	std::string paramName;
	for (char c : l) {
		if (c == ' ') {

			if (returnType.size() > 0)
				isType = false;

			continue;
		}


		if (c == ',') {
			method.input.push_back({ paramName, returnType });
			returnType = "";
			paramName = "";

			isType = true;
			continue;
		}

		if (c == ')') {
			if (returnType.size() > 0)
				method.input.push_back({ paramName, returnType });
			break;
		}

		if (isType)
			returnType += c;
		else paramName += c;
	}

	return method;
}



#define TAB "\t"
#define NEWLINE "\n"
#define VARIABLE "Scripting::Variable"
#define VECTOR "std::vector<" VARIABLE ">const& vec"
#define CAST(className, cast) "static_cast<" + className + "*>(" + cast + ")"


std::string ECSReader::Method::ScriptName()
{
	if (className == "ScriptFunctionality")
		return methodName;

	return className + "_" + methodName;
}

std::string ECSReader::Method::FunctionName()
{
	return className + "_" + methodName;
}

std::string ECSReader::Method::FunctionDeclaration()
{
	std::stringstream declaration;

	declaration << VARIABLE << " ";

	declaration << FunctionName();

	declaration << "(" VECTOR ");" NEWLINE;

	return declaration.str();
}

std::string ECSReader::Method::FunctionDefinition()
{
	std::stringstream definition;


	definition << VARIABLE << " ";

	definition << FunctionName();

	definition << "(" VECTOR "){" NEWLINE NEWLINE;


	//TODO: manejo de errores: { input diferente al necesario } { metodos de managers }

	std::string expectedEntity = "Scripting::Variable::Type::Entity";
	std::string givenEntity = "vec[0].type";

	definition << TAB "if(" << givenEntity << " != " << expectedEntity << "){" NEWLINE;
	//entity, script, function, i, expected, given
	definition << TAB TAB"DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, \"" + ScriptName() <<
		"\", std::to_string(0)" << ", " << "std::string(\"\")" << ", " << " \"\"" << "); " NEWLINE;
	definition << TAB TAB"return Scripting::Variable::Null();" NEWLINE;

	definition << TAB"}" NEWLINE NEWLINE;

	for (int i = 0; i < input.size(); i++) {

		if (!IsVariable(input[i].type)) {

			std::string expected = "Scripting::Variable::Type::" + String2ScriptingEnum(input[i].type);
			std::string given = "vec[" + std::to_string(i + 1) + "].type";

			definition << TAB "if(" << given << " != " << expected << "){" NEWLINE;
			//entity, script, function, i, expected, given
			definition << TAB TAB"DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, \"" + ScriptName() <<
				"\", std::to_string(" << i + 1 << ")" << ", " << "std::string(\"\")" << ", " << " \"\"" << "); " NEWLINE;
			definition << TAB TAB"return Scripting::Variable::Null();" NEWLINE;

			definition << TAB"}" NEWLINE NEWLINE;
		}
	}



	definition << TAB << className << "* self = vec[0].value.entity->getComponent<" + className + ">();" NEWLINE;



	definition << TAB "if(self == nullptr){" NEWLINE;
	definition << TAB TAB"DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, \"" + ScriptName() + "\", vec[0].value.entity->getEntityName(), " + className + ");" NEWLINE;
	definition << TAB TAB"return Scripting::Variable::Null();" NEWLINE;
	definition << TAB"}" NEWLINE;



	definition << TAB;
	if (returnType != "void") {

		definition << returnType << " ret = ";
	}

	definition << "self->" << methodName << "(";

	int i = 1;
	for (auto& in : input) {

		definition << "vec[" << i << "]" << String2ScriptingVariable(in.type) << ", ";

		i++;
	}

	if (input.size() > 0)
		definition.seekp(-2, definition.cur);
	definition << ");" NEWLINE;


	if (returnType != "void")
		definition << TAB "return ret;" NEWLINE;
	else
		definition << TAB "return " VARIABLE "::Null();" NEWLINE;

	definition << "}" NEWLINE;

	return definition.str();
}

std::string ECSReader::Method::ManagerFunctionDeclaration()
{
	std::stringstream declaration;

	declaration << VARIABLE << " ";

	declaration << FunctionName();

	declaration << "(" VECTOR "){" NEWLINE;

	declaration << TAB << className << "* manager = " + className + "::instance();" NEWLINE;

	declaration << TAB;
	if (returnType != "void") {

		declaration << returnType << " ret = ";
	}

	declaration << "manager->" << methodName << "(";

	int i = 0;
	for (auto& in : input) {

		declaration << "vec[" << i << "]" << String2ScriptingVariable(in.type) << ", ";

		i++;
	}

	if (input.size() > 0)
		declaration.seekp(-2, declaration.cur);
	declaration << ");" NEWLINE;


	if (returnType != "void")
		declaration << TAB "return ret;" NEWLINE;
	else
		declaration << TAB "return " VARIABLE "::Null();" NEWLINE;

	declaration << "}" NEWLINE;

	return declaration.str();
}




std::string ECSReader::Method::String2ScriptingVariable(std::string const& in)
{
	if (in == "int")
		return ".value.Float";
	if (in == "float")
		return ".value.Float";
	if (in == "bool")
		return ".value.Bool";
	if (in == "char")
		return ".value.Char";

	if (in == "std::string" || in == "string" || in == "cstring" || in == "cString") {
		return ".str";
	}

	if (in == "Utilities::Vector2D" || in == "Vector2D" || in == "cVector2D") {
		return ".vector";
	}

	if (in == "Utilities::Color" || in == "Color" || in == "cColor") {
		return ".value.color";
	}

	if (in == "Variable" || in == "Scripting::Variable" || in == "cVariable") {
		return "";
	}

	if (in == "Entity*" || in == "ECS::Entity*") {
		return ".value.entity";
	}

	return ".value." + in;
}

std::string ECSReader::Method::String2ScriptingEnum(std::string const& in)
{
	if (in == "int")
		return "Float";
	if (in == "float")
		return "Float";
	if (in == "bool")
		return "Bool";
	if (in == "char")
		return "Char";
	if (in == "std::string" || in == "string" || in == "cstring" || in == "cString") {
		return "String";
	}
	if (in == "Utilities::Vector2D" || in == "Vector2D" || in == "cVector2D") {
		return "Vector2D";
	}
	if (in == "Utilities::Color" || in == "Color" || in == "cColor") {
		return "Color";
	}
	if (in == "Entity*" || in == "ECS::Entity*") {
		return "Entity";
	}

	return "Null";
}

bool ECSReader::Method::IsVariable(std::string const& in)
{
	return in == "Variable" || in == "Scripting::Variable" || in == "cVariable";
}


std::string ECSReader::Attribute::TypeConversion(std::string const& convertName)
{
	if (type == "int") {
		return "std::stoi(" + convertName + ")";
	}

	if (type == "float") {
		return "std::stof(" + convertName + ")";
	}

	if (type == "double") {
		return "std::stod(" + convertName + ")";
	}

	if (type == "bool") {

		return convertName + " == \"true\" ? true : false";
	}

	if (type == "string" || type == "std::string" || type == "cstring" || type == "cString") {
		return convertName;
	}

	if (type == "Utilities::Vector2D" || type == "Vector2D" || type == "cVector2D") {

		return convertName;
	}

	if (type == "Utilities::Color" || type == "Color" || type == "cColor") {

		return "Utilities::Color::CreateColor(" + convertName + ")";
	}

	return convertName;
}


ECSReader& ECSReader::CreateFunctionManagerHeader()
{
	std::ofstream h(output + "/FunctionManager.h");

	h << ClassCreator("FunctionManager")
		.IncludeAbsolutes({ "unordered_map", "string", "vector" })
		.IncludeRelative("Scripting/Variable.h")
		.Empty(3)
		.AddDefine("ECSfunc_Version", ECS_Version)
		.AddComment("Creation time : " + GetTimeStamp())
		.Empty()
		.AddLine("typedef Scripting::Variable(*CallableFunction)(std::vector<Scripting::Variable> const&);")
		.Empty()
		.BeginClass()
		.Public()
		.AddMethod("void", "CreateFunctionMap", { {"std::unordered_map<std::string, CallableFunction>&", "map"} }, "", true)
		.EndClass()
		.Empty(3)
		.Header();

	for (auto& currentClass : methods) {

		for (auto& method : currentClass.second) {

			h << method.FunctionDeclaration();
		}
	}


	for (auto& currentClass : managerMethods) {

		for (auto& method : currentClass.second) {

			h << method.FunctionDeclaration();
		}
	}


	h.close();
	return *this;
}


//TODO: este metodo es una mierda
ECSReader& ECSReader::CreateFunctionManagerSource()
{
	std::ofstream cpp(output + "/FunctionManager.cpp");

	cpp << R"(
#include "FunctionManager.h"
#include "Entity.h"
#include "ConsoleManager.h"

)";

	cpp << "//Creation time: " << GetTimeStamp() << "\n";

	//TODO: por muy epico que sea el codigo realmente quedaria mejor sin los defines estos

	cpp << R"~(#define _Console(info, value) Console::Output::PrintError( info , value )
#define _ErrorInfo(entity, script, function, title) entity + ": " + script + ": " + function + ": " + title + ": "
#define _DebugError(entity, script, function, title, error) _Console(_ErrorInfo(entity, script, function, title), error)

#define _ComponentErrorMessage(entityError, missingComponent) "The entity <" + entityError + "> does not contain <" + missingComponent + "> component"
#define _NullErrorMessage(i, expected) "Empty input given to input(" + i + "). Expected type: <" + expected + ">"
#define _InvalidInputErrorMessage(i, expected, given) "Expected <" + expected + "> but received <" + given + "> for input(" + i + ")"

#define DebugComponentError(entity, script, function, entityError, missingComponent) _DebugError(entity, script, function, "Entity error", _ComponentErrorMessage(entityError, #missingComponent))
#define DebugNullError(entity, script, function, i, expected) _DebugError(entity, script, function, "Empty value", _NullErrorMessage(i, expected))
#define DebugInvalidInputError(entity, script, function, i, expected, given) _DebugError(entity, script, function, "Invalid input", _InvalidInputErrorMessage(i, expected, given))


)~";


	for (auto& file : filesToInclude) {

		auto filename = std::filesystem::path(file);
		cpp << "#include <Components/" << filename.filename().string() << ">\n";
	}


	for (auto& file : managerFiles) {

		auto filename = std::filesystem::path(file);
		cpp << "#include <" << filename.filename().string() << ">\n";
	}


	cpp << R"(

using namespace ECS;
using namespace Physics;
using namespace Input;
using namespace Scripting;
using namespace Sound;
using namespace Renderer;


void FunctionManager::CreateFunctionMap(std::unordered_map<std::string, CallableFunction>& map){

)";

	for (auto& currentClass : methods) {

		for (auto& method : currentClass.second) {

			std::string functionName = method.FunctionName();
			cpp << "\tmap.emplace(\"" << functionName << "\"," << functionName << ");\n";
		}
	}

	for (auto& currentClass : managerMethods) {

		for (auto& method : currentClass.second) {

			cpp << "\tmap.emplace(\"" << method.ScriptName() << "\"," << method.FunctionName() << ");\n";
		}
	}

	cpp << "\n};\n";

	for (auto& currentClass : methods) {

		for (auto& method : currentClass.second) {

			cpp << method.FunctionDefinition();
		}
	}

	for (auto& currentClass : managerMethods) {

		for (auto& method : currentClass.second) {

			cpp << method.ManagerFunctionDeclaration();
		}
	}


	cpp.close();
	return *this;
}

#include <chrono>
#include <ctime>

std::string ECSReader::GetTimeStamp()
{
	char buff[26];
	time_t currtime = std::time(NULL);

	ctime_s(buff, sizeof buff, &currtime);

	return std::string(buff);
}

ECSReader& ECSReader::ClassReflection()
{
	std::ofstream h(output + "/ClassReflection.h  ");
	std::ofstream cpp(output + "/ClassReflection.cpp");

	ClassCreator creator = ClassCreator("ClassReflection", true, true);

	creator.IncludeAbsolutes({ "string" , "unordered_map" });
	creator.AddLine("namespace ECS { class Component; }");

	creator.Empty()
		.AddComment("Creation time : " + GetTimeStamp())
		.AddDefine("ECSreflection_Version", ECS_Version)
		.AddLine()
		.AddLine("using namespace ECS;")
		.BeginClass()
		.AddLine("typedef void(ClassReflection::*ReflectionMethod)(ECS::Component*, std::unordered_map<std::string, std::string> const&);")
		.Empty(1)
		.Private()
		.AddAtribute("std::unordered_map<std::string, ReflectionMethod>", "reflectionMethods")
		.Empty(1);

	std::stringstream constructor;

	constructor << "\n";

	for (auto& className : attributes) {

		constructor << "\treflectionMethods[\"" << className.first << "\"] = &ClassReflection::Reflect" << className.first << ";\n";
	}

	creator.AddConstructor(constructor.str()).Public().AddDestructor();



	std::stringstream reflector;

	reflector << "\tif(reflectionMethods.contains(component))\n";
	reflector << "\t\t(this->*reflectionMethods[component])(pointer, map);\n";

	//TODO: lanzar un error en el else

	creator.AddMethod("void", "ReflectComponent", { {"std::string const&", "component"}, {"ECS::Component*", "pointer"},
			{"std::unordered_map<std::string, std::string> const&", "map"} }, reflector.str(), false);




	for (auto& className : attributes) {

		std::stringstream method;
		method << "\t\t" << className.first << "* self = static_cast<" + className.first + "*>(selfComp);\n";

		for (auto& attribute : className.second) {

			method << "\t\tif(map.contains(\"" << attribute.name << "\"))\n";

			method << "\t\t\tself->" << attribute.name << " = " << attribute.TypeConversion("map.at(\"" + attribute.name + "\")") << ";\n";
		}

		creator.AddMethod("void", "Reflect" + className.first, { {"ECS::Component*", "selfComp"},
			{"std::unordered_map<std::string, std::string> const&", "map"} }, method.str(), false);

		creator.AddLine();
	}



	creator.EndClass();
	for (auto& file : filesToInclude) {

		auto filename = std::filesystem::path(file);
		creator.AddCppInclude("Components/" + filename.filename().string());
	}



	h << creator.Header();
	cpp << creator.Source();

	h.close();
	cpp.close();
	return *this;
}


ECSReader& ECSReader::ComponentFactory()
{
	std::ofstream h(output + "/ComponentFactory.h  ");
	std::ofstream cpp(output + "/ComponentFactory.cpp");

	ClassCreator creator = ClassCreator("ComponentFactory", true, true);

	creator.IncludeAbsolutes({ "string" , "unordered_map" });
	creator.AddLine("namespace ECS { class Component; }");


	std::stringstream constructor;

	constructor << "\n";

	for (auto& component : componentsWithPaths) {

		constructor << "\tcomponents[\"" << component.first << "\"] = &ComponentFactory::Create" << component.first << ";\n";
	}


	creator.Empty()
		.AddComment("Creation time : " + GetTimeStamp())
		.AddDefine("ECSreflection_Version", ECS_Version)
		.AddLine()
		.AddLine("using namespace ECS;")
		.BeginClass()
		.Public()
		.AddDestructor()
		.Private()
		.AddLine("typedef ECS::Component*(ComponentFactory::*ComponentCreation)();")
		.Empty(1)
		.AddConstructor(constructor.str())
		.AddAtribute("std::unordered_map<std::string, ComponentCreation>", "components")
		.Empty(1);


	for (auto& component : componentsWithPaths) {

		std::stringstream method;

		method << "\t\treturn new ECS::" << component.first << "();\n";

		creator.AddMethod("ECS::Component*", "Create" + component.first, {}, method.str(), false);
	}

	std::stringstream factoryMethod;

	factoryMethod << "\tif(components.contains(comp))\n";
	factoryMethod << "\t\treturn (this->*components[comp])();\n";
	factoryMethod << "\treturn nullptr;";

	creator.Public().AddLine().AddMethod("ECS::Component*", "CreateComponent", { {"std::string const&", "comp"} }, factoryMethod.str(), false);

	creator.EndClass();


	for (auto& component : componentsWithPaths) {

		auto FileName = std::filesystem::path(component.second);

		std::string filename = FileName.filename().string();
		std::string root = filename == "Script.h" ? "Scripting" : "Components";

		creator.AddCppInclude(root + "/" + filename);
	}

	h << creator.Header();
	cpp << creator.Source();

	h.close();
	cpp.close();

	return *this;
}


#include "json.hpp"
using namespace nlohmann;




ECSReader& ECSReader::GenerateFunctionJSON()
{
	/*
		Ejemplo del JSON

			{
			"Transform" : {
				"Move" : {
					"input" = {
						Vector3 Move;
					},
					"return" : "int"
				}
			},
		}
	*/


	json root;

	for (auto& currentClass : methods) {

		root[currentClass.first] = {};

		for (auto& method : currentClass.second) {

			root[currentClass.first][method.methodName] =
			{
				{"return", method.returnType}
			};


			json inputJson;

			for (auto& input : method.input) {
				inputJson +=
				{
					{"type", input.type},
					{ "name", input.name }
				};
			}

			if (!inputJson.is_null())
				root[currentClass.first][method.methodName]["input"] = inputJson;
		}
	}

	std::ofstream fmJSON(output + "/Functions.json");

	fmJSON << root.dump(4) << std::endl;

	fmJSON.close();

	return *this;
}



ECSReader& ECSReader::GenerateAttributeJSON()
{
	/*
		Ejemplo del JSON

			{
			"Transform" : {

				{"type", "int"},
				{"name", "mass"},
				}
			}
		}
	*/


	json root;

	for (auto& currentClass : attributes) {

		root[currentClass.first] = nullptr;

		for (auto& attr : currentClass.second) {

			root[currentClass.first] += {

				{"type", attr.type},
				{ "name" , attr.name },
			};
		}
	}

	std::ofstream fmJSON(output + "/Attributes.json");

	fmJSON << root.dump(4) << std::endl;

	fmJSON.close();

	return *this;
}


std::string ECSReader::ProcessType(std::string const& input)
{
	if (input == "std::string" || input == "string" || input == "cstring" || input == "cString")
		return "Text";

	if (input == "float" || input == "int" || input == "double")
		return "Number";

	if (input == "Utilities::Vector2D" || input == "Vector2D" || input == "cVector2D")
		return "Pair";

	if (input == "char")
		return "Letter";

	if (input == "Color" || input == "Utilities::Color" || input == "cColor")
		return "Color";

	if (input == "bool")
		return "Toggle";

	if (input == "Entity*" || input == "ECS::Entity*")
		return "Entity";

	if (input == "void")
		return "void";

	if (input == "cVariable" || input == "Scripting::Variable")
		return "Any";

	std::cout << "Error con el tipo: " << input << std::endl;
	return input;
}


ECSReader& ECSReader::GenerateComponentsJSON()
{
	json root;

	for (auto& component : allComponents) {

		json cmp = nlohmann::ordered_json();

		if (attributes.contains(component)) {

			for (auto& attr : attributes[component]) {

				cmp["attributes"] += {

					{"type", ProcessType(attr.type)},
					{ "name" , attr.name }
				};
			}
		}


		if (methods.contains(component)) {

			for (auto& method : methods[component]) {

				json methodData = json::basic_json();

				methodData["return"] = ProcessType(method.returnType);
				methodData["name"] = method.methodName;

				json inputJson = nlohmann::ordered_json();

				inputJson += {
					{"type", "Entity"},
					{ "name", "entity" }
				};

				for (auto& input : method.input) {
					inputJson +=
					{
						{"type", ProcessType(input.type)},
						{ "name", input.name }
					};
				}

				methodData["input"] = inputJson;

				cmp["methods"] += methodData;
			}

		}

		root[component] = cmp;
	}

	std::ofstream fmJSON(output + "/Components.json");

	fmJSON << root.dump(4) << std::endl;

	fmJSON.close();

	return *this;
}




ECSReader& ECSReader::GenerateManagersJSON()
{
	json root = json::array();


	std::map<std::string, std::vector<Method>> correctedManagerMethods = managerMethods;


	correctedManagerMethods.erase("ScriptFunctionality");

	for (auto& method : managerMethods["ScriptFunctionality"])
	{
		size_t separator = method.methodName.find("_");

		std::string newClassName = method.methodName.substr(0, separator);
		std::string newMethodName = method.methodName.substr(separator + 1);


		Method newMethod;

		newMethod.className = newClassName;
		newMethod.methodName = newMethodName;
		newMethod.returnType = method.returnType;
		newMethod.input = method.input;

		correctedManagerMethods[newClassName].push_back(newMethod);

	}


	for (auto& manager : correctedManagerMethods)
	{
		json managerJson;

		managerJson["name"] = manager.first;
		managerJson["methods"] = json::array();

		for (auto& method : manager.second) {


			json methodJson;

			json inputJson = json::array();
			for (auto& input : method.input) {

				inputJson.push_back(
					{ {"name", input.name}, {"type", ProcessType(input.type)} });
			}

			methodJson["input"] = inputJson;
			methodJson["return"] = ProcessType(method.returnType);
			methodJson["name"] = method.methodName;

			managerJson["methods"].push_back(methodJson);
		}

		root.push_back(managerJson);
	}


	std::ofstream fmJSON(output + "/Managers.json");

	fmJSON << root.dump(4) << std::endl;

	fmJSON.close();

	return *this;
}
