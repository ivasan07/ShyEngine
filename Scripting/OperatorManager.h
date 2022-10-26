#pragma once



namespace CFlat {

	class IBox;

	struct OperatorManager
	{
		enum Operations {
			Print, If, Loop,
			
			//Math operations
			Add, Subtract, Multiply, Division, Power, SquareRoot, Mod, PlusOne,

			//Logic operations
			Equal, Different, Minor, Greater, MinorEqual, GreaterEqual, And, Or, Not,

			//Attributes
			getInteger, setInteger, getDecimal, setDecimal, getBoolean, setBoolean , getString, setString,



			OperatorCount
		};
		typedef void(*Funct)(IBox*);

		static Funct operations[OperatorCount];

		static void Initialise();

		static void ProccessOperation(int operationIdx, IBox* node);

		
	};

	namespace BoxOperations {


		namespace Control {

			void If(IBox* node);
			void Loop(IBox* node);
		}

		namespace Math {

			void Add(IBox* node);
			void Subtract(IBox* node);
			void Multiply(IBox* node);
			void Division(IBox* node);
			void Power(IBox* node);
			void SquareRoot(IBox* node);
			void Mod(IBox* node);
			void PlusOne(IBox* node);
		};

		namespace Logic {

			void Equals(IBox* node);
			void Different(IBox* node);
			void Minor(IBox* node);
			void Greater(IBox* node);
			void MinorEqual(IBox* node);
			void GreaterEqual(IBox* node);
			void And(IBox* node);
			void Or(IBox* node);
			void Not(IBox* node);
		}

		namespace Attributes {

			void getInteger(IBox* node);
			void getDecimal(IBox* node);
			void getBoolean(IBox* node);
			void getString(IBox* node);


			void setInteger(IBox* node);
			void setDecimal(IBox* node);
			void setBoolean(IBox* node);
			void setString(IBox* node);


		}

		namespace IO {
			void Print(IBox* node);

		};
	}

}

