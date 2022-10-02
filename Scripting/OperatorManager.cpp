#include "OperatorManager.h"
#include "CFlat.h"
#include <iostream>

CFlat::OperatorManager::Funct CFlat::OperatorManager::operations[CFlat::OperatorManager::OperatorCount];

void CFlat::OperatorManager::Initialise()
{
	operations[Operations::Print] = IO::Print;

	operations[Operations::Add] = Math::Add;
	operations[Operations::Subtract] = Math::Subtract;
	operations[Operations::Multiply] = Math::Multiply;
}


void CFlat::OperatorManager::IO::Print(IBox* node)
{
	Output* output = node->input[0]->output;
	switch (output->type) {
	case ParamType::_float:
		std::cout << output->value.floatValue;
		break;

	case ParamType::_int:
		std::cout << output->value.intValue;
		break;
	}
}


void CFlat::OperatorManager::ProccessOperation(int operationIdx, IBox* node)
{
	operations[operationIdx](node);
}

void CFlat::OperatorManager::Math::Add(IBox* node)
{
	node->output = new Output();

	node->output->type = ParamType::_float;
	node->output->value.floatValue = node->input[0]->output->value.floatValue + node->input[1]->output->value.floatValue;
}

void CFlat::OperatorManager::Math::Subtract(IBox* node)
{
	node->output = new Output();

	node->output->type = ParamType::_float;
	node->output->value.floatValue = node->input[0]->output->value.floatValue - node->input[1]->output->value.floatValue;
}

void CFlat::OperatorManager::Math::Multiply(IBox* node)
{
	node->output = new Output();

	node->output->type = ParamType::_float;
	node->output->value.floatValue = node->input[0]->output->value.floatValue * node->input[1]->output->value.floatValue;
}
