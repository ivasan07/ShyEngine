#include "CFlat.h"


namespace CFlat {



	IBox::IBox() {

		output = nullptr;
		nextBox = nullptr;
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
			box->processBox();
		}
	}

	void IBox::processBox()
	{
		validateInput();
		operation();
	}

	void IBox::addInput(IBox* newInput) {
		input.push_back(newInput);
	}
}