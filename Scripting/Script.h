#pragma once

#include <string>


namespace CFlat {

	class IBox;
}

using std::string;


/*
* 
*	Esto es lo que seria basicamente un componente
* 
*/

class Script
{

private:
	CFlat::IBox* initBox;
	CFlat::IBox* updateBox;

	void iteration(CFlat::IBox* startingBox);



public:

	void setupScript(std::string script);

	void init();

	void update();
};

