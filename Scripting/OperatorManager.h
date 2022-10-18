#pragma once



namespace CFlat {

	class IBox;

	struct OperatorManager
	{
		enum Operations {
			Print, If, Loop,
			
			//Math operations
			Add, Subtract, Multiply, Division, Power, SquareRoot, Mod,


			OperatorCount
		};
		typedef void(*Funct)(IBox*);

		static Funct operations[OperatorCount];

		static void Initialise();

		static void ProccessOperation(int operationIdx, IBox* node);

		
	};

	namespace BoxOperations {


		namespace Logic {

			void If(IBox* node);
			void Loop(IBox* node);
			void Equals(IBox* node);
		}

		namespace Math {

			void Add(IBox* node);
			void Subtract(IBox* node);
			void Multiply(IBox* node);
			void Division(IBox* node);
			void Power(IBox* node);
			void SquareRoot(IBox* node);
			void Mod(IBox* node);
		};

		namespace IO {
			void Print(IBox* node);

		};
	}

}

