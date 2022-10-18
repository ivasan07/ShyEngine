#pragma once
#include "CFlat.h"
#include <string>


namespace CFlat {

	//TODO no tiene que haber una clase float ni nada asi

	class Split : public IBox {

	public:
		Split(int op);
		IBox* otherRoute;
		void setOtherRoute(IBox*);
	};




	namespace Attributes {

		IBox* createAttribute(ParamType type);
		IBox* createFloat(float value);
		IBox* createInt(int value);
		IBox* createString(std::string value);
	}

}

