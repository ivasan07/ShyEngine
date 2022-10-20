#pragma once
#include "CFlat.h"
#include <string>


namespace CFlat {


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
		IBox* createBoolean(int value);
		IBox* createString(std::string value);
	}
}

