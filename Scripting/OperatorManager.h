#pragma once



namespace CFlat {

	class IBox;

	struct OperatorManager
	{
		enum Operations {
			Print, Add, Subtract, Multiply,


			OperatorCount
		};
		typedef void(*Funct)(IBox*);

		static Funct operations[OperatorCount];

		static void Initialise();

		static void ProccessOperation(int operationIdx, IBox* node);

		
	};

	namespace BoxOperations {

		namespace Math {

			static void Add(IBox* node);
			static void Subtract(IBox* node);
			static void Multiply(IBox* node);
		};

		namespace IO {
			static void Print(IBox* node);

		};
	}

}

