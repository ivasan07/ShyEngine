#include "CFlat.h"
#include "OperatorManager.h"

namespace CFlat {



	IBox::IBox(int op) {

		operation = op;
		output = nullptr;
		nextBox = nullptr;
		clearOutput = true;
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
		for (IBox* box : input) {
			if (box->output == nullptr)
				box->processBox();
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