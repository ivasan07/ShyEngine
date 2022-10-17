#include "CFlatBasics.h"

#include <iostream>

namespace CFlat {


	Split::Split(): IBox(-1)
	{

		constOutput = false;
	}

	Float::Float(float value): IBox(-1)
	{
		this->value = value;

		output = new Output();
		output->type = ParamType::_float;

		output->value.floatValue = value;

		constOutput = true;
	}

	String::String(std::string value) : IBox(-1)
	{
		this->value = value;

		output = new Output();
		output->type = ParamType::_string;

		output->message = value;

		constOutput = true;
	}
	
}
