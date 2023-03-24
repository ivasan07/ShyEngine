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

		if (it.is_directory() && it.path().filename() == "scriptingFunctionality") {
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

		//TODO: esto no hacerlo asi, mejor hacer que importe el fichero .h y ya
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

		//TODO: comprobar si pertecene a una clase exportada o a una clase manager

		bool forceAddClassName = false;
		std::string textToRemove = "";

		if (line.contains("class")) {

			if (line.contains(";")) //Forward declaration -> ignore
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

				methods[currentClassName];
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

			//TODO: ver si deberia separar la reflexion de la funcionalizacion a la hora de incluir ficheros
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

				//TODO: hacer algo asi para el manejo de errores con colores que puede quedar bastante chulo
				HANDLE hConsole;

				hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

				SetConsoleTextAttribute(hConsole, 12);
				std::cout << "Error: ";
				SetConsoleTextAttribute(hConsole, 15);

				std::cout << "Comportamiento no valido.Hay un atributo dentro de la marca " _2str(publish) << "\n";
				std::cout << "Linea: " + line << "\nEn la clase " + currentClassName << std::endl;
				continue;
			}

			if (!currentClassIsManager)
				methods[currentClassName].push_back(method);

			else
				managerMethods[currentClassName].push_back(method);


			//TODO: distinguir entre includes de componentes y de managers

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

	definition << "(" VECTOR "){" NEWLINE;

	definition << TAB << className << "* self = " CAST(className, "vec[0].value.pointer") ";" NEWLINE;

	definition << TAB;
	if (returnType != "void") {

		definition << returnType << " ret = ";
	}

	definition << "self->" << methodName << "(";

	int i = 1;
	for (auto& in : input) {

		definition << "vec[" << i << "]." << String2ScriptingVariable(in.type) << ", ";

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

		declaration << "vec[" << i << "]." << String2ScriptingVariable(in.type) << ", ";

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




//TODO: rellenar con el resto de valores
std::string ECSReader::Method::String2ScriptingVariable(std::string& in)
{
	if (in == "int")
		return "value.Int";
	if (in == "float")
		return "value.Float";
	if (in == "bool")
		return "value.Bool";
	if (in == "char")
		return "value.Char";

	return "value." + in;
}


std::string ECSReader::Attribute::TypeConversion(std::string const& convertName)
{
	if (type == "int") {
		return "std::stoi(" + convertName + ")";
	}

	if (type == "float") {
		return "std::stof(" + convertName + ")";
	}

	if (type == "bool") {

		return convertName + " == \"true\" ? true : false";
	}

	if (type == "string") {
		return convertName;
	}

	if (type == "Utilities::Vector2D" || type == "Vector2D") {

		return convertName;
	}

	return convertName;
}


ECSReader& ECSReader::CreateFunctionManagerHeader()
{
	std::ofstream h(output + "/FunctionManager.h");

	h << ClassCreator("FunctionManager")
		.IncludeAbsolutes({ "map", "string", "vector" })
		.IncludeRelative("Scripting/Variable.h")
		.Empty(3)
		.AddDefine("ECSfunc_Version", ECS_Version)
		.AddComment("Creation time : " __TIMESTAMP__)
		.Empty()
		.AddLine("typedef Scripting::Variable(*CallableFunction)(std::vector<Scripting::Variable> const&);")
		.Empty()
		.BeginClass()
		.Public()
		.AddMethod("void", "CreateFunctionMap", { {"std::map<std::string, CallableFunction>&", "map"} }, "", true)
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

ECSReader& ECSReader::CreateFunctionManagerSource()
{
	std::ofstream cpp(output + "/FunctionManager.cpp");

	cpp << R"(#include "FunctionManager.h"

)";

	cpp << "//Creation time: " << __TIMESTAMP__ << "\n";

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



void FunctionManager::CreateFunctionMap(std::map<std::string, CallableFunction>& map){

)";

	for (auto& currentClass : methods) {

		for (auto& method : currentClass.second) {

			std::string functionName = method.FunctionName();
			cpp << "\tmap.emplace(\"" << functionName << "\"," << functionName << ");\n";
		}
	}

	for (auto& currentClass : managerMethods) {

		for (auto& method : currentClass.second) {

			std::string functionName = method.FunctionName();
			cpp << "\tmap.emplace(\"" << functionName << "\"," << functionName << ");\n";
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


ECSReader& ECSReader::ClassReflection()
{
	std::ofstream h(output + "/ClassReflection.h  ");
	std::ofstream cpp(output + "/ClassReflection.cpp");

	ClassCreator creator = ClassCreator("ClassReflection");

	creator.IncludeAbsolutes({ "string" , "map" });
	creator.AddLine("namespace ECS { class Component; }");

	creator.Empty()
		.AddComment("Creation time : " __TIMESTAMP__)
		.AddDefine("ECSreflection_Version", ECS_Version)
		.AddLine()
		.AddLine("using namespace ECS;")
		.BeginClass()
		.AddLine("typedef void(ClassReflection::*ReflectionMethod)(ECS::Component*, std::map<std::string, std::string> const&);")
		.Empty(1)
		//TODO: mapa a funciones para simplificar aun mas el proceso.AddAtribute("std::map<std::string, ")
		.AddAtribute("std::map<std::string, ReflectionMethod>", "reflectionMethods")
		.Empty(1)
		.Public();


	for (auto& className : attributes) {

		std::stringstream method;
		method << "\t\t" << className.first << "* self = static_cast<" << className.first << "*>(selfComp);\n";

		for (auto& attribute : className.second) {
			//TODO hacer un metodo que convierta de string a un valor


			method << "\t\tif(map.contains(\"" << attribute.name << "\"))\n";

			method << "\t\t\tself->" << attribute.name << "= " << attribute.TypeConversion("map.at(\"" + attribute.name + "\")") << ";\n";
		}


		creator.AddMethod("void", "Reflect" + className.first, { {"ECS::Component*", "selfComp"},
			{"std::map<std::string, std::string> const&", "map"} }, method.str(), true);

		creator.AddLine();
	}


	//TODO: Buscar las clases que tengo que a�adir para reflexionar

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



#include "json.hpp"
using namespace nlohmann;

ECSReader& ECSReader::Convert2JSON()
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

	std::ofstream fmJSON(output + "/FunctionManager.json");

	fmJSON << root.dump(4) << std::endl;

	fmJSON.close();

	return *this;
}