#include "CFlat.h"
#include "OperatorManager.h"

namespace CFlat {


	IBox::IBox(int op) {

		operation = op;
		output = nullptr;
		nextBox = nullptr;
		constOutput = true;
	}

	IBox::~IBox()
	{
		for (IBox* in : input) {
			delete in;
		}
		input.clear();

		if (output != nullptr)
			delete output;

		nextBox = nullptr;
	}

	void IBox::validateInput()
	{
		//TODO en vez de hacer que el ouput se compruebe si es nullptr es mejor hacer que no se este borrando y creando el output

		for (IBox* box : input) {
			if (!box->constOutput || box->output == nullptr)
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