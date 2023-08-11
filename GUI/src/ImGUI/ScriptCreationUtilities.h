#pragma once
#include "Window.h"
#include <string>

#include <vector>
#include "ComponentInfo.h"
#include "nlohmann/json_fwd.hpp"

namespace PEditor {

	class ScriptCreation;

	namespace ScriptCreationUtilities {

		/*
			TODO:

			Desplegable con todos los metodos
			Dividir los metodos por categor�as
			Desplegables con el tipo de input
			Mostrar cuadro de input personalizado para cada tipo
			Mostar cuadros metodos con el nombre, input con nombre y output
			Mostar lineas de union entre input y output
			Dibujar lineas de continuacion de nodos
			Hacer que se puedan tener varios nodos con el mismo nombre
			Scroll por la escena
			Crear una barra de navegacion arriba para que se vea bien e incluir mas cosas
			Leer los managers
			Buscador de metodos
			Hacer que solo se pueda a�adir una flecha si ambas salidas coinciden
			Hacer que si un nodo retorna null no se pueda a�adir a otro
			Tener en cuenta que hay veces en las que hay metodos que reciben como entrada una cVariable
			Serializacion
			Lista desplegable de nodos que al hacer click te lleve al elemento seleccionado

			-Guardar el output de las casillas
			-Colorear el triangulo de salida cuando tenga al menos un input

			-Condicionales
			-Bucles
			-Implementar eventos (start, update)
			-Eliminar un nodo

			-Establecer formato de nombres para las cosas (llamar a todo o method o function)

			-Cambiar el foreground drawList por el drawlist de la ventana
			-Control z manager

			-Names: Es la forma de tener metodos en el scripting
			-Comentar todo el codigo y ver que cosas se pueden mejorar

			-Mover la serializacion y carga a script creation en vez de estar en utilities

			-Cambiar la serializacion de vector2 y color a array en vez de string
			-Los nodos se pintan por encima de la barra superior
			-Cuando el input esta fuera no se dibuja la linea
		*/

		class ScriptMethod;

		class ScriptNode {

		public:
			
			enum class Node {
				Method, Input, Fork
			};


			ScriptNode();

			ScriptNode* SetID(int id);
			ScriptNode* SetPosition(int x, int y);

			bool Render();

			int GetId();
			virtual std::string GetStringId();

			int GetY();
			int GetX();

			int GetW();
			int GetH();

			std::string GetOutputString();

			static ScriptNode* currentlySelected;
			void GetOutputNodePosition(float* x, float* y);

			virtual nlohmann::json ToJson();
			virtual void FromJson(nlohmann::json&);


			Node GetType();


			void AddOutput(ScriptMethod* output);
			bool RemoveOutput(ScriptMethod* output);

			virtual void OnRemoved();

		protected:

			int id;
			Node type;

			float x, y;
			float w, h;

			float nodeSize;
			bool ignoreOutput;

			std::string outputStr;

			std::vector<ScriptMethod*> outputConexions;

			virtual void render();

			void UpdatePosition(int scrollx, int scrolly);
			void ManageOutputNode();
			bool ManageCloseNode();
		};

		class ScriptInput : public ScriptNode {

			//Definir el tipo de input, representar y guardar el valor
		public:
			ScriptInput(::Components::AttributesType type);

			nlohmann::json ToJson() override;

			::Components::AttributeValue attrValue;

		protected:

			bool reflect; //TODO
			::Components::AttributesType attrType;

			void render() override;

		};

		class ScriptFlow  {

		protected:

			ScriptFlow* next;
		};



		class ScriptMethod : public ScriptNode, public ScriptFlow {

			::Components::Method& method;
			std::vector<ScriptNode*> input;

		public:

			ScriptMethod(::Components::Method&);

			nlohmann::json ToJson() override;

			void SetInput(int idx, ScriptNode* node);

			void OnRemoved() override;

			void OnInputRemoved(ScriptNode* node);

		protected:



			void render() override;
			std::string GetStringId() override;
		};


		class ScriptMenuBar {

		private:

			//Cambiar el nombre del script
			//Boton para guardar
			//Boton para cerrar
			//Desplegable con los distintos tipos de valores constantes
			//Barra de busqueda de componentes

			static const int CharBufferSize = 256;

			char nameBuffer[CharBufferSize]; //TODO: cambiar esto a string
			char nameSearch[CharBufferSize]; //TODO: cambiar esto a string

			bool showNodeSearch;
			bool showPopup;

			ScriptCreation* creator;

			void Close();
			void Save();

			void AddMatchingMethods(std::unordered_map<std::string, Components::Component>& v, int windowW, int windowH);
			Components::Method& GetMethodReference(const std::string& name);
		public:
			void Load();

			ScriptMenuBar(ScriptCreation* creator);

			void SetName(const std::string& name);
			void Render();

		};

		class ScriptDropdownSelection {

		private:
			ScriptCreation* creator;

			int mousex, mousey;

		public:

			ScriptDropdownSelection(ScriptCreation* creator);
			void Render();

			void AddValuesFromVector(std::unordered_map<std::string, Components::Component>& v);
		};



		class Bezier {


		private:

			static float thickness;
			static int pointCount;

		public:

			static void ResetThickness();
			static void SetThickness(float t);
			static void SetPointCount(int c);
			static void ResetPointCount();


			static void Draw(int x, int y, int x1, int y1);

		};


		class Grid {

		private:
			static int spacing;
			static float thickness;
			static int interval;
			static float intervalScale;

			static int x_offset;
			static int y_offset;

			static int r, g, b, a;

		public:

			static void SetSpacing(int spacing);
			static void ResetSpacing();
			static void SetOffset(int x, int y);
			static void ResetOffset();
			static void SetInterval(int interval);
			static void ResetInterval();

			static void SetColor(int r, int g, int b, int a);
			static void ResetColor();

			static void Draw();
		};

	}
};
