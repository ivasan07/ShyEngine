#include "CFlatBasics.h"

#include <iostream>

namespace CFlat {

	void CFlat::Add::operation()
	{
		output = new Output();

		output->type = ParamType::_float;
		output->value.floatValue = input[0]->output->value.floatValue + input[1]->output->value.floatValue;
	}

	void CFlat::Subtract::operation()
	{
		output = new Output();

		output->type = ParamType::_float;
		output->value.floatValue = input[0]->output->value.floatValue - input[1]->output->value.floatValue;
	}

	void CFlat::Multiply::operation()
	{
		output = new Output();

		output->type = ParamType::_float;
		output->value.floatValue = input[0]->output->value.floatValue * input[1]->output->value.floatValue;
	}

	void Print::operation()
	{
		Output* output = input[0]->output;
		switch (output->type) {
		case ParamType::_float:
			std::cout << output->value.floatValue;
			break;

		case ParamType::_int:
			std::cout << output->value.intValue;
			break;
		}
	}
	Float::Float(float value)
	{
		this->value = value;

	}
	void Float::operation()
	{
		output = new Output();
		output->type = ParamType::_float;

		output->value.floatValue = value;
	}
}
