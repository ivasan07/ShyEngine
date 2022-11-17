#pragma once

#include <vector>
#include <string>

class Script;

namespace CFlat {

	enum class ParamType {

		_null = 0,
		_int = 1,
		_float = 2,
		_vector2int = 4,
		_vector2float = 8,
		_vector3int = 16,
		_vector3float = 32,
		_string = 64,
		_boolean = 128,
		_box = 256
	};

	class IBox;

	struct Output {

		struct FloatVector
		{
			float a, b, c;
		};

		struct IntVector
		{
			float a, b, c;
		};

		union Values {
			bool boolean;
			int intValue;
			float floatValue;
			IntVector intVector;
			FloatVector floatVector;
			IBox* box;
		};

		std::string message;

		Values value;
		ParamType type;
	};


	class IBox {

	protected:
		void validateInput(std::ofstream& stream);

		int operation;
		int boxId;

	public:
		IBox(int operation, int id);
		~IBox();

		bool constOutput;

		IBox* nextBox;
		Output output;
		std::vector<IBox*> input;

		std::string boxName();
		std::string constValueOperation;

		Script* script;

		void processBox(std::ofstream& stream);
		void addInput(IBox* box);

		bool used;
	};

}



