#include "CFlatBasics.h"

#include <iostream>

namespace CFlat {

	Float::Float(float value): IBox(-1)
	{
		this->value = value;

		output = new Output();
		output->type = ParamType::_float;

		output->value.floatValue = value;

		constExpression = true;
	}

	String::String(std::string value) : IBox(-1)
	{
		this->value = value;

		output = new Output();
		output->type = ParamType::_string;

		output->message = value;

		constExpression = true;
	}
}
