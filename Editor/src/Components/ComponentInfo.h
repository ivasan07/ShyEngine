#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json_fwd.hpp>

namespace Components {

	using cstring = std::string const&;

	enum class AttributesType {
		NONE,
		FLOAT,
		VECTOR2,
		STRING,
		BOOL,
		COLOR,
		CHAR,
		ENTITY
	};


	struct AttributeValue {

		union value
		{
			float valueFloat;
			struct {
				float x;
				float y;
			} valueVector2;
			bool valueBool;
			struct {
				float r;
				float g;
				float b;
			} valueColor;
			char valueChar;
			int entityIdx;
		} value;

		std::string valueString;
	};


	class Attribute {

	private:
		AttributesType type;
		std::string name;
		std::string typeStr;

		void SetDefaultValues();

	public:

		Attribute();
		Attribute(const std::string& name, const std::string& typeString);
		Attribute(const std::string& name);

		void SetType(const std::string& str, const AttributesType& type);
		void SetValue(const AttributeValue& value);
		void SetValueStr(const std::string& str);
		AttributesType GetType() const;

		const std::string& GetName() const;
		const std::string& GetTypeStr() const;

		static AttributesType GetAttributeTypeFromTypeStr(const std::string& typeStr);
		static std::string GetTypeStrFromAttributeType(AttributesType type);

		std::string ToJson();

		AttributeValue value;
	};

	using Variable = Attribute;

	class Function {

	private:

		std::string component;
		std::string name;

		Variable returnType;
		std::vector<Variable> input;
	public:

		Function();
		Function(Function const& other);
		Function(const std::string& name, const std::string& className);

		void SetReturn(const Variable& ret);
		void AddInput(const Variable& input);

		std::string getName() const;
		std::string getComponent() const;
		Variable getReturn() const;

		const std::vector<Variable>& getInput() const;
	};


	class Component {

	private:

		std::string name;

		std::unordered_map<std::string, Attribute> attributes;
		std::unordered_map<std::string, Function> functions;


		/*
			Guardamos los atributos y metodos en dos estructuras diferentes
			Los mapas los usamos para busar un elemento en concreto de forma rapida
			Por otro lado, los vectores los usamos para mantener el orden de insercion
			ya que en algunos componentes interesa dibujar esta informacion en ese orden
		*/

		std::vector<Attribute*> orderedAttributes;
		std::vector<Function*> orderedFunctions;

	public:

		Component();
		Component(const Component& other);
		Component(cstring name);

		cstring GetName();

		Attribute& GetAttribute(cstring name);
		Function& GetFunction(cstring name);

		std::unordered_map<std::string, Attribute>& GetAllAttributes();
		std::unordered_map<std::string, Function>& GetAllFunctions();

		std::vector<Attribute*>& GetAttributesOrdered();
		std::vector<Function*>& GetFunctionsOrdered();

		void AddAttribute(const Attribute& attribute);
		void AddFunction(const Function& function);

		nlohmann::ordered_json ToJson();

		static Component FromJson(std::string json);
	};


	class Script {

	private:

		std::string name;
		std::string path;


		std::unordered_map<std::string, Attribute> attributes;
		std::unordered_map<std::string, std::string> enums;

	public:

		Script(cstring name, cstring path);

		std::string GetName();

		void AddAttribute(const std::string& name, Attribute attr);
		std::unordered_map<std::string, Attribute>& GetAllAttributes();

		std::string ToJson();

		void AddEnumAttribute(const std::string& attribute, const std::string& enumCategory);

		bool IsEnum(const std::string& attribute);
		std::string GetEnum(const std::string& attribute);

		std::string GetPath();

		static Script fromJson(std::string name, std::string json);
	};

}
