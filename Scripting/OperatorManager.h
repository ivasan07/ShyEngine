#pragma once



namespace CFlat {

	class IBox;

	struct OperatorManager
	{
		enum Operations {
			Print, Add, Subtract, Multiply, If, Loop,


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
		};

		namespace IO {
			void Print(IBox* node);

		};
	}

}

