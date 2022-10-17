#include "OperatorManager.h"
#include "CFlat.h"
#include <iostream>

CFlat::OperatorManager::Funct CFlat::OperatorManager::operations[CFlat::OperatorManager::OperatorCount];

void CFlat::OperatorManager::Initialise()
{
	operations[Operations::Print] = BoxOperations:: IO::Print;
	operations[Operations::Add] = BoxOperations::Math::Add;
	operations[Operations::Subtract] = BoxOperations::Math::Subtract;
	operations[Operations::Multiply] = BoxOperations::Math::Multiply;
	operations[Operations::If] = BoxOperations::Logic::If;
	operations[Operations::Loop] = BoxOperations::Logic::Loop;
}


void CFlat::BoxOperations::IO::Print(IBox* node)
{
	Output* output = node->input[0]->output;
	switch (output->type) {
	case ParamType::_float:
		std::cout << output->value.floatValue;
		break;

	case ParamType::_int:
		std::cout << output->value.intValue;
		break;
	case ParamType::_string:
		std::cout << output->message;
		break;
	}

	std::cout << std::endl;
}


void CFlat::BoxOperations::Logic::If(IBox* node)
{
	

	bool condition = node->input[0]->output->value.boolean;

	if (condition) {

	}




}

void CFlat::BoxOperations::Logic::Loop(IBox* node)
{
	
}


void CFlat::BoxOperations::Logic::Equals(IBox* node)
{

}



void CFlat::OperatorManager::ProccessOperation(int operationIdx, IBox* node)
{
	operations[operationIdx](node);
}

void CFlat::BoxOperations::Math::Add(IBox* node)
{
	node->output = new Output();

	node->output->type = ParamType::_float;
	node->output->value.floatValue = node->input[0]->output->value.floatValue + node->input[1]->output->value.floatValue;
}

void CFlat::BoxOperations::Math::Subtract(IBox* node)
{
	node->output = new Output();

	node->output->type = ParamType::_float;
	node->output->value.floatValue = node->input[0]->output->value.floatValue - node->input[1]->output->value.floatValue;
}

void CFlat::BoxOperations::Math::Multiply(IBox* node)
{
	node->output = new Output();

	node->output->type = ParamType::_float;
	node->output->value.floatValue = node->input[0]->output->value.floatValue * node->input[1]->output->value.floatValue;
}
