#pragma once
#include "CFlat.h"



namespace CFlat {


	class Float : public IBox {

		float value;
	public:
		Float(float value);
		void operation() override;
	};

	class Add: public IBox {

	public:
		void operation() override;
	};


	class Subtract: public IBox {

	public:
		void operation() override;
	};


	class Multiply:public IBox
	{

	public:
		void operation() override;
	};


	class Print :public IBox
	{

	public:
		void operation() override;
	};
}

