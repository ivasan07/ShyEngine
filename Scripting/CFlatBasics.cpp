#include "CFlatBasics.h"

#include <iostream>

namespace CFlat {


	Split::Split(int op) : IBox(op)
	{
		constOutput = false;
		otherRoute = nullptr;
	}

	void Split::setOtherRoute(IBox* o)
	{
		output = new Output();
		output->type = ParamType::_box;
		output->value.box = nextBox;
		otherRoute = o;
	}


	Float::Float(float value) : IBox(-1)
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
