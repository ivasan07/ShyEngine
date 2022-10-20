#include "OperatorManager.h"
#include "CFlat.h"
#include "CFlatBasics.h"
#include <iostream>
#include <math.h>

#include "Script.h"
#include "ScriptAttribute.h"

CFlat::OperatorManager::Funct CFlat::OperatorManager::operations[CFlat::OperatorManager::OperatorCount];

void CFlat::OperatorManager::Initialise()
{
	operations[Operations::Print] = BoxOperations::IO::Print;

	operations[Operations::If] = BoxOperations::Control::If;
	operations[Operations::Loop] = BoxOperations::Control::Loop;

	operations[Operations::Add] = BoxOperations::Math::Add;
	operations[Operations::Subtract] = BoxOperations::Math::Subtract;
	operations[Operations::Multiply] = BoxOperations::Math::Multiply;
	operations[Operations::Division] = BoxOperations::Math::Division;
	operations[Operations::Power] = BoxOperations::Math::Power;
	operations[Operations::SquareRoot] = BoxOperations::Math::SquareRoot;
	operations[Operations::Mod] = BoxOperations::Math::Mod;
	operations[Operations::PlusOne] = BoxOperations::Math::PlusOne;

	operations[Operations::Equal] = BoxOperations::Logic::Equals;
	operations[Operations::Different] = BoxOperations::Logic::Different;
	operations[Operations::Minor] = BoxOperations::Logic::Minor;
	operations[Operations::Greater] = BoxOperations::Logic::Greater;
	operations[Operations::MinorEqual] = BoxOperations::Logic::MinorEqual;
	operations[Operations::GreaterEqual] = BoxOperations::Logic::GreaterEqual;
	operations[Operations::And] = BoxOperations::Logic::And;
	operations[Operations::Or] = BoxOperations::Logic::Or;
	operations[Operations::Not] = BoxOperations::Logic::Not;

	operations[Operations::getInteger] = BoxOperations::Attributes::getInteger;
	operations[Operations::setInteger] = BoxOperations::Attributes::setInteger;
	operations[Operations::getDecimal] = BoxOperations::Attributes::getDecimal;
	operations[Operations::setDecimal] = BoxOperations::Attributes::setDecimal;
	operations[Operations::getBoolean] = BoxOperations::Attributes::getBoolean;
	operations[Operations::setBoolean] = BoxOperations::Attributes::setBoolean;
	operations[Operations::getString] = BoxOperations::Attributes::getString;
	operations[Operations::setString] = BoxOperations::Attributes::setString;

}



void CFlat::OperatorManager::ProccessOperation(int operationIdx, IBox* node)
{
	operations[operationIdx](node);
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


void CFlat::BoxOperations::Control::If(IBox* node)
{
	//TODO hacer bien la condicion del if
	//bool condition = node->input[0]->output.value.boolean;

	bool condition = node->input[0]->output.value.floatValue > 0;

	Split* split = static_cast<Split*>(node);

	node->nextBox = condition ? node->output.value.box : split->otherRoute;
}

void CFlat::BoxOperations::Control::Loop(IBox* node)
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
	node->output.type = ParamType::_boolean;

	switch (node->input[0]->output.type) {
	case ParamType::_boolean:
		node->output.value.boolean = node->input[0]->output.value.boolean == node->input[1]->output.value.boolean;
		break;
	case ParamType::_int:
		node->output.value.boolean = node->input[0]->output.value.intValue == node->input[1]->output.value.intValue;
		break;
	case ParamType::_float:
		node->output.value.boolean = node->input[0]->output.value.floatValue == node->input[1]->output.value.floatValue;
		break;
	case ParamType::_string:
		node->output.value.boolean = node->input[0]->output.message == node->input[1]->output.message;
		break;
	}
}


void CFlat::BoxOperations::Logic::Different(IBox* node)
{
	node->output.type = ParamType::_boolean;

	switch (node->input[0]->output.type) {
	case ParamType::_boolean:
		node->output.value.boolean = node->input[0]->output.value.boolean != node->input[1]->output.value.boolean;
		break;
	case ParamType::_int:
		node->output.value.boolean = node->input[0]->output.value.intValue != node->input[1]->output.value.intValue;
		break;
	case ParamType::_float:
		node->output.value.boolean = node->input[0]->output.value.floatValue != node->input[1]->output.value.floatValue;
		break;
	case ParamType::_string:
		node->output.value.boolean = node->input[0]->output.message != node->input[1]->output.message;
		break;
	}
}



void CFlat::BoxOperations::Logic::Minor(IBox* node)
{
	node->output.type = ParamType::_boolean;

	switch (node->input[0]->output.type) {
	case ParamType::_int:
		node->output.value.boolean = node->input[0]->output.value.intValue < node->input[1]->output.value.intValue;
		break;
	case ParamType::_float:
		node->output.value.boolean = node->input[0]->output.value.floatValue < node->input[1]->output.value.floatValue;
		break;
	}
}


void CFlat::BoxOperations::Logic::Greater(IBox* node)
{
	node->output.type = ParamType::_boolean;

	switch (node->input[0]->output.type) {
	case ParamType::_int:
		node->output.value.boolean = node->input[0]->output.value.intValue > node->input[1]->output.value.intValue;
		break;
	case ParamType::_float:
		node->output.value.boolean = node->input[0]->output.value.floatValue > node->input[1]->output.value.floatValue;
		break;
	}
}


void CFlat::BoxOperations::Logic::GreaterEqual(IBox* node)
{
	node->output.type = ParamType::_boolean;

	switch (node->input[0]->output.type) {
	case ParamType::_int:
		node->output.value.boolean = node->input[0]->output.value.intValue >= node->input[1]->output.value.intValue;
		break;
	case ParamType::_float:
		node->output.value.boolean = node->input[0]->output.value.floatValue >= node->input[1]->output.value.floatValue;
		break;
	}
}


void CFlat::BoxOperations::Logic::MinorEqual(IBox* node)
{
	node->output.type = ParamType::_boolean;

	switch (node->input[0]->output.type) {
	case ParamType::_int:
		node->output.value.boolean = node->input[0]->output.value.intValue <= node->input[1]->output.value.intValue;
		break;
	case ParamType::_float:
		node->output.value.boolean = node->input[0]->output.value.floatValue <= node->input[1]->output.value.floatValue;
		break;
	}
}


void CFlat::BoxOperations::Logic::And(IBox* node) {
	node->output.type = ParamType::_boolean;

	node->output.value.boolean = node->input[0]->output.value.boolean && node->input[1]->output.value.boolean;
}


void CFlat::BoxOperations::Logic::Or(IBox* node) {
	node->output.type = ParamType::_boolean;

	node->output.value.boolean = !node->input[0]->output.value.boolean || node->input[1]->output.value.boolean;
}

void CFlat::BoxOperations::Logic::Not(IBox* node) {
	node->output.type = ParamType::_boolean;

	node->output.value.boolean = !node->input[0]->output.value.boolean;
}



void CFlat::BoxOperations::Attributes::getInteger(IBox* node) {
	node->output.type = ParamType::_int;
	node->script->attributes.retreiveAttribute(node->input[0]->output.message, node->output.value.intValue);
	node->constOutput = false;
}

void CFlat::BoxOperations::Attributes::getDecimal(IBox* node) {
	node->output.type = ParamType::_float;
	node->script->attributes.retreiveAttribute(node->input[0]->output.message, node->output.value.floatValue);
	node->constOutput = false;
}

void CFlat::BoxOperations::Attributes::getBoolean(IBox* node) {
	node->output.type = ParamType::_boolean;
	node->script->attributes.retreiveAttribute(node->input[0]->output.message, node->output.value.boolean);
	node->constOutput = false;
}

void CFlat::BoxOperations::Attributes::getString(IBox* node) {
	node->output.type = ParamType::_string;
	node->script->attributes.retreiveAttribute(node->input[0]->output.message, node->output.message);
	node->constOutput = false;
}



void CFlat::BoxOperations::Attributes::setInteger(IBox* node) {
	node->output.type = ParamType::_null;

	node->script->attributes.updateAttribute(
		node->input[0]->output.message,
		node->input[1]->output.value.intValue
	);
}

void CFlat::BoxOperations::Attributes::setDecimal(IBox* node) {
	node->output.type = ParamType::_null;

	node->script->attributes.updateAttribute(
		node->input[0]->output.message,
		node->input[1]->output.value.floatValue
	);
}


void CFlat::BoxOperations::Attributes::setBoolean(IBox* node) {
	node->output.type = ParamType::_null;

	node->script->attributes.updateAttribute(
		node->input[0]->output.message,
		node->input[1]->output.value.boolean
	);
}


void CFlat::BoxOperations::Attributes::setString(IBox* node) {
	node->output.type = ParamType::_null;

	node->script->attributes.updateAttribute(
		node->input[0]->output.message,
		node->input[1]->output.message
	);
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


void CFlat::BoxOperations::Math::PlusOne(IBox* node)
{
	if (node->input[0]->output.type == ParamType::_float) {
		node->output.type = ParamType::_float;
		node->output.value.floatValue = node->input[0]->output.value.floatValue + 1;
	}

	else if (node->input[0]->output.type == ParamType::_int) {
		node->output.type = ParamType::_int;
		node->output.value.intValue = node->input[0]->output.value.intValue + 1;
	}
}


