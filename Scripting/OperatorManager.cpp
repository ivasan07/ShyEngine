#include "OperatorManager.h"
#include "CFlat.h"
#include "CFlatBasics.h"
#include <iostream>
#include <math.h>

CFlat::OperatorManager::Funct CFlat::OperatorManager::operations[CFlat::OperatorManager::OperatorCount];

void CFlat::OperatorManager::Initialise()
{
	operations[Operations::Print] = BoxOperations::IO::Print;
	operations[Operations::Add] = BoxOperations::Math::Add;
	operations[Operations::Subtract] = BoxOperations::Math::Subtract;
	operations[Operations::Multiply] = BoxOperations::Math::Multiply;
	operations[Operations::If] = BoxOperations::Logic::If;
	operations[Operations::Loop] = BoxOperations::Logic::Loop;
}


void CFlat::BoxOperations::IO::Print(IBox* node)
{
	Output output = node->input[0]->output;
	switch (output.type) {
	case ParamType::_float:
		std::cout << output.value.floatValue;
		break;

	case ParamType::_int:
		std::cout << output.value.intValue;
		break;
	case ParamType::_string:
		std::cout << output.message;
		break;
	}

	std::cout << std::endl;
}


void CFlat::BoxOperations::Logic::If(IBox* node)
{
	//TODO hacer bien la condicion del if
	//bool condition = node->input[0]->output.value.boolean;

	bool condition = node->input[0]->output.value.floatValue > 0;

	Split* split = static_cast<Split*>(node);

	node->nextBox = condition ? node->output.value.box : split->otherRoute;
}

void CFlat::BoxOperations::Logic::Loop(IBox* node)
{
	CFlat::Split* split = static_cast<CFlat::Split*>(node);
	CFlat::IBox* conditionNode = node->input[0];

	bool range = false;
	if (conditionNode->output.type == ParamType::_int) {

		range = true;
	}

	int n = 0;
	while ((range && n < conditionNode->output.value.intValue) || (!range && conditionNode->output.value.boolean)) {

		CFlat::IBox* box = split->otherRoute;

		while (box != nullptr) {

			box->processBox();
			box = box->nextBox;
		}

		if (!range)
			conditionNode->processBox();

		n++;
	}

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
	node->output.type = ParamType::_float;
	node->output.value.floatValue = node->input[0]->output.value.floatValue + node->input[1]->output.value.floatValue;
}

void CFlat::BoxOperations::Math::Subtract(IBox* node)
{
	node->output.type = ParamType::_float;
	node->output.value.floatValue = node->input[0]->output.value.floatValue - node->input[1]->output.value.floatValue;
}

void CFlat::BoxOperations::Math::Multiply(IBox* node)
{
	node->output.type = ParamType::_float;
	node->output.value.floatValue = node->input[0]->output.value.floatValue * node->input[1]->output.value.floatValue;
}


void CFlat::BoxOperations::Math::Division(IBox* node)
{
	node->output.type = ParamType::_float;
	node->output.value.floatValue = node->input[0]->output.value.floatValue / node->input[1]->output.value.floatValue;
}


void CFlat::BoxOperations::Math::Power(IBox* node)
{
	node->output.type = ParamType::_float;
	node->output.value.floatValue = powf(node->input[0]->output.value.floatValue, node->input[1]->output.value.floatValue);
}



void CFlat::BoxOperations::Math::SquareRoot(IBox* node)
{
	node->output.type = ParamType::_float;
	node->output.value.floatValue = sqrtf(node->input[0]->output.value.floatValue);
}



void CFlat::BoxOperations::Math::Mod(IBox* node)
{
	node->output.type = ParamType::_float;
	node->output.value.floatValue = node->input[0]->output.value.floatValue * node->input[1]->output.value.floatValue;
}

