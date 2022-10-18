#include "CFlatBasics.h"

#include <iostream>

CFlat::IBox* CFlat::Attributes::createAttribute(CFlat::ParamType type) {

	IBox* box = new IBox(-1);
	box->output.type = type;

	box->constOutput = true;
	return box;
}

CFlat::IBox* CFlat::Attributes::createFloat(float value) {

	CFlat::IBox* box = createAttribute(ParamType::_float);
	box->output.value.floatValue = value;
	return box;
}

CFlat::IBox* CFlat::Attributes::createInt(int value) {

	CFlat::IBox* box = createAttribute(ParamType::_int);
	box->output.value.intValue = value;
	return box;
}

CFlat::IBox* CFlat::Attributes::createString(std::string value)
{
	CFlat::IBox* box = CFlat::Attributes::createAttribute(CFlat::ParamType::_string);
	box->output.message = value;
	return box;
}


namespace CFlat {


	Split::Split(int op) : IBox(op)
	{
		constOutput = false;
		otherRoute = nullptr;
	}

	void Split::setOtherRoute(IBox* o)
	{
		output.type = ParamType::_box;
		output.value.box = nextBox;
		otherRoute = o;
	}
}
