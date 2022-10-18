#include "CFlat.h"
#include "OperatorManager.h"

namespace CFlat {


	IBox::IBox(int op) {

		operation = op;
		nextBox = nullptr;
		constOutput = true;

		output.type = ParamType::_null;
	}

	IBox::~IBox()
	{
		for (IBox* in : input) {
			delete in;
		}
		input.clear();
		nextBox = nullptr;
	}

	void IBox::validateInput()
	{
		//TODO en vez de hacer que el ouput se compruebe si es nullptr es mejor hacer que no se este borrando y creando el output

		for (IBox* box : input) {
			if (!box->constOutput || box->output.type == ParamType::_null)
			{
				box->processBox();
				constOutput &= box->constOutput;
			}
		}
	}

	void IBox::processBox()
	{
		validateInput();
		OperatorManager::ProccessOperation(operation, this);
	}

	void IBox::addInput(IBox* newInput) {
		input.push_back(newInput);
	}
}