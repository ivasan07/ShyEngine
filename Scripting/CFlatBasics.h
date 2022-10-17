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

	class Float : public IBox {

		float value;
	public:
		Float(float value);
	};


	class String : public IBox {

		std::string value;
	public:
		String(std::string value);
	};

}

